#include "dimentionVux.h"

namespace HXQACHINE {

HxqDimentionVux::HxqDimentionVux()
{

}
HxqDimentionVux::~HxqDimentionVux()
{

}


float HxqDimentionVux::detect(cv::Mat& frame,const cv::Rect& roi)
{
    cv::Mat dst;
    int width = roi.width < frame.cols/2 ? roi.width : frame.cols/2;
    int height = roi.height < frame.rows/2 ? roi.height : frame.rows/2;
    cv::Rect calRoi = cv::Rect(frame.cols/2-width/2,frame.rows/2-height/2,width,height);
    cv::Mat roiMat = frame(calRoi);
    cv::Mat gray;
    if (roiMat.channels() != 1) {
        frame.copyTo(dst);
        cv::cvtColor(roiMat, gray,cv::COLOR_BGR2GRAY);
    } else {
        cv::cvtColor(frame,dst,cv::COLOR_GRAY2BGR);
        roiMat.copyTo(gray);
    }

   cv::Mat hist;
   cv::equalizeHist(gray, hist);
   cv::imwrite("hist.jpg", hist);

    cv::Mat gaussianSrc = _gaussian_filter(gray);
    int apertureSize = 3;
    bool L2gradient = true;
    cv::Mat dx, dy, magnitudes, angles;
    _sobel_gradient(gaussianSrc, dx, dy, magnitudes, angles, apertureSize, L2gradient);

    //非极大值抑制计算高低阈值
    int low, high;
    cv::Mat NMSImage;
    _calculate_hysteresis_threshold_value(dx, dy, magnitudes, angles, NMSImage, low, high);

    cv::Mat map;
    std::deque<int> mapIndicesX, mapIndicesY;
    _non_maximum_suppression(NMSImage, map, mapIndicesX, mapIndicesY, low, high);

    _hysteresis_thresholding(mapIndicesX, mapIndicesY, map);
//    cv::Mat adaptiveCanny = _get_canny_result(map);

    //利用计算出来的low和high传入opencv Canny进行对比
    cv::Mat opencvCanny;
    cv::Canny(gray, opencvCanny, low, high, apertureSize, L2gradient);

   imwrite("canny.jpg",opencvCanny);

    cv::Point minLoc,maxLoc;
    double minVal,maxVal;
    cv::minMaxLoc(opencvCanny,&minVal,&maxVal,&minLoc,&maxLoc);

    std::ostringstream content;
    bool flag=false;
    content<<"min:"<<minVal<<",max:"<<maxVal;
    if ((maxVal-128.00001) > 0.00001) {
        content << "Y";
        flag=true;
        cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,255,0),-1);
    } else {
        content << "N";
        flag=false;
        cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,0,255),-1);
    }
    cv::rectangle(dst, cv::Point(calRoi.x,calRoi.y), cv::Point(calRoi.x+calRoi.width,calRoi.y+calRoi.height),cv::Scalar(0,0,255));
    cv::putText(dst,content.str().c_str(),cv::Point(calRoi.x,calRoi.y),cv::FONT_HERSHEY_COMPLEX,1.0,cv::Scalar(0,255,0));
    dst.copyTo(frame);
    return flag;
}

}
