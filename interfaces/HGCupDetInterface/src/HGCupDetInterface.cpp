#include "HGCupDetInterface.h"
#include "hgcupdet.h"
#include "hgdetectcircle.h"
#include "hgcapture2dfromusb.h"

namespace HGMACHINE {
HGCupDet hgcupDet;
HGCapture2DFromUSB hgcapture2dFromUSB;

void openUSBCamera(const int & index){
    // hgcapture2dFromUSB.open(index);
}
HGImg2D getImgFromUSBOneShot(){
    HGImg2D img;
    cv::Mat mat;
    hgcapture2dFromUSB.getFrameOne(mat);
    // 检查类型是否是 uchar（8-bit），不是就报错
    if (mat.depth() != CV_8U) {
        throw std::runtime_error("Only 8-bit image supported in HGImg2D");
    }
    img.data = mat.ptr<unsigned char>(0);  // 指向图像数据首地址
    img.width = mat.cols;                  // 宽度（列）
    img.height = mat.rows;                 // 高度（行）
    img.channels = mat.channels();         // 通道数
    img.steps = static_cast<int>(mat.step); // 每行字节数（包含通道和对齐）
    img.type = mat.type();

    cv::imwrite("oneshot.bmp",mat);
    printf("type:%d\n",mat.channels());
    return img;
}
void closeUSBCamera(){
    hgcapture2dFromUSB.close();
}
void detCircle(const HGImg2D &img, const HGRect2D &roi){
    hgcupDet.detCircle(img,roi);
}
int getTargetPosX(){
    return hgcupDet.getTargetPosX();
}
void detCupExistence(const HGImg2D &img, const HGRect2D &roi)
{
    hgcupDet.detCupExistence(img,roi);
}
bool getAbsenseFlag()
{
    return hgcupDet.getAbsenseFlag();
}
HGImg2D getDst()
{
    return hgcupDet.getDst();
}
}
