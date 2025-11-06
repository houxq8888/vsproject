#include "CameraRecognizeInterface.h"
#include "usbcameramanager.h"
#include "ipcameramanager.h"
#include "hgcommonutility.h"
#include "hgcupdet.h"

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
    cv::Mat mat=cv::Mat::zeros(760,1280,CV_8UC3);
    // if (type=="USB") {
    //     if (-1==USBCameraManager::instance().getFrameOne(name,mat)) {

    //         // return img;
    //     }
    // } else if (type=="IP") {
    //     if (-1==IPCameraManager::instance().getFrameOne(name,mat)) return img;
    // }
    // 检查类型是否是 uchar（8-bit），不是就报错
    if (mat.depth() != CV_8U) {
        throw std::runtime_error("Only 8-bit image supported in HGImg2D");
    }
    img.data = mat.data;  // 指向图像数据首地址
    img.width = mat.cols;                  // 宽度（列）
    img.height = mat.rows;                 // 高度（行）
    img.type = mat.type();

    cv::imwrite("oneshot.bmp",mat);
    printf("type:%d\n",mat.channels());
    return img;
}

cv::Mat getImgOneShotMat(const std::string &type,const std::string& name){
    cv::Mat mat;
    mat=cv::imread("Screenshot from 2025-10-15 10-12-58.png");
    // if (type=="USB") {
    //     if (-1==USBCameraManager::instance().getFrameOne(name,mat)) {

    //         // return mat;
    //     }
    // } else if (type=="IP") {
    //     if (-1==IPCameraManager::instance().getFrameOne(name,mat)) return mat;
    // }
    // 检查类型是否是 uchar（8-bit），不是就报错
    if (mat.depth() != CV_8U) {
        throw std::runtime_error("Only 8-bit image supported in HGImg2D");
    }

    return mat;
}
std::string saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect){
    cv::Mat mat(img.height,img.width,img.type,(uchar*)img.data);
    if (mat.empty()) return "failed";
    cv::Mat roi = mat(cv::Rect(rect.x1,rect.y1,rect.x2-rect.x1,rect.y2-rect.y1));
    if (roi.empty()) return "failed";
    std::string name="default"+getStandardCurTime()+".bmp";
    cv::imwrite(name.c_str(),roi);
    return name;
}
std::string saveCameraTemplateMat(const cv::Mat& mat, const HGRect2D& rect){
    if (mat.empty()) return "failed";
    cv::Mat roi = mat(cv::Rect(rect.x1,rect.y1,rect.x2-rect.x1,rect.y2-rect.y1));
    if (roi.empty()) return "failed";
    std::string name="default"+getStandardCurTime()+".bmp";
    cv::imwrite(name.c_str(),roi);
    return name;
}
MatchResult2D recognizeCameraTemplate(const HGImg2D& img, const std::string &templateName){
    MatchResult2D result;
    HGCupDet hgcupDet;
    if (templateName==""){
        hgcupDet.detCupExistence(img,HGRect2D(0,img.width,0,img.height));
        result.flag = hgcupDet.getAbsenseFlag();
        result.dst = hgcupDet.getDst();
        result.name = templateName;
        result.score = (result.flag?0:1);
    } else {
        hgcupDet.matchTemplate(img,HGRect2D(0,img.width,0,img.height),templateName);
        result.flag = hgcupDet.getMatchFlag();
        result.dst = hgcupDet.getDst();
        result.name = templateName;
        result.score = hgcupDet.getMatchScore();
    }
    return result;
}
}