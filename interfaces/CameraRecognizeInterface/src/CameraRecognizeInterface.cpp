#include "CameraRecognizeInterface.h"
#include "usbcameramanager.h"
#include "ipcameramanager.h"
#include "hgcommonutility.h"
#include "hgcupdet.h"
#include "rwDb.h"

namespace HGMACHINE{
std::vector<std::string> getCameraList(const std::string& type){
    if (type=="USB")
        return USBCameraManager::instance().discoverCameras();
    else if (type=="IP")
        return IPCameraManager::instance().discoverCameras();
}
void openCamera(const std::string &type, const std::string& name){
    if (type=="USB")
        USBCameraManager::instance().openCamera(name);
    else if (type=="IP")
        IPCameraManager::instance().openCamera(name);
}
void closeCamera(const std::string &type,const std::string& name){
    if (type=="USB")
        USBCameraManager::instance().closeCamera(name);
    else if (type=="IP")
        IPCameraManager::instance().closeCamera(name);
}
HGImg2D getImgOneShot(const std::string &type,const std::string& name){
    HGImg2D img;
    cv::Mat mat; //cv::Mat::zeros(760,1280,CV_8UC3);
    for (int i=0;i<5;i++){
        if (type == "USB")
        {
            if (-1 == USBCameraManager::instance().getFrameOne(name, mat))
            {
                return img;
            }
        }
        else if (type == "IP")
        {
            if (-1 == IPCameraManager::instance().getFrameOne(name, mat))
                return img;
        }
    }
    // 检查类型是否是 uchar（8-bit），不是就报错
    if (mat.depth() != CV_8U) {
        throw std::runtime_error("Only 8-bit image supported in HGImg2D");
    }
    img.data = mat.data;  // 指向图像数据首地址
    img.width = mat.cols;                  // 宽度（列）
    img.height = mat.rows;                 // 高度（行）
    img.type = mat.type();

    cv::imwrite("oneshot.bmp",mat);
    printf("type:%d\n",mat.type());
    return img;
}

cv::Mat getImgOneShotMat(const std::string &type,const std::string& name){
    cv::Mat mat;
    
    // 如果摄像头类型为USB
    if (type=="USB") {
        if (-1==USBCameraManager::instance().getFrameOne(name,mat)) return mat;
    } else if (type=="IP") {
        if (-1==IPCameraManager::instance().getFrameOne(name,mat)) return mat;
    }
    
    // 检查类型是否是 uchar（8-bit），如果不是就转换
    if (mat.depth() != CV_8U) {
        throw std::runtime_error("Only 8-bit image supported in HGImg2D");
    }

    return mat;
}
std::string saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect){
    HGMkDir(RWDb::readCurDirPath()+"/template");
    cv::Mat mat(img.height,img.width,img.type,(uchar*)img.data);
    cv::Mat roi = mat(cv::Rect(rect.x1,rect.y1,rect.x2-rect.x1,rect.y2-rect.y1));
    std::string name=RWDb::readCurDirPath()+"/template/"+"default"+getFileNameFromTime()+".bmp";
    cv::imwrite(name.c_str(),roi);
    return name;
}
std::string saveCameraTemplateMat(const cv::Mat& img, const HGRect2D& rect){
    HGMkDir(RWDb::readCurDirPath()+"/template");
    if (img.empty()) return "failed";
    cv::Mat mat=img.clone();
    
    // 检查图像通道数，如果是3通道(BGR)，转换为RGB格式
    if (mat.channels() == 3) {
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    }
    
    cv::Mat roi = mat(cv::Rect(rect.x1,rect.y1,rect.x2-rect.x1,rect.y2-rect.y1));
    if (roi.empty()) return "failed";
    std::string name=RWDb::readCurDirPath()+"/template/"+"default"+getFileNameFromTime()+".bmp";
    cv::imwrite(name.c_str(),roi);
    return name;
}
MatchResult2D recognizeCameraTemplate(const HGImg2D& img, const std::string &templateName){
    MatchResult2D result;
    HGCupDet hgcupDet;
    if (templateName==""){
        hgcupDet.detCupExistence(img,HGRect2D(0,0,img.width-1,img.height-1));
        result.flag = hgcupDet.getAbsenseFlag();
        result.dst = hgcupDet.getDst();
        result.name = templateName;
        result.score = (result.flag?1:0);
    } else {
        hgcupDet.matchTemplate(img,HGRect2D(0,0,img.width-1,img.height-1),templateName);
        result.flag = hgcupDet.getMatchFlag();
        result.dst = hgcupDet.getDst();
        result.name = templateName;
        result.score = hgcupDet.getMatchScore();
        result.rect = hgcupDet.getRect();
        result.dstMat = hgcupDet.getDstMat();
    }
    return result;
}
}