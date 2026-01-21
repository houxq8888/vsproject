#include "hgdetectcircle.h"

namespace HGMACHINE {

HGCircleDetect::HGCircleDetect()
{

}
HGCircleDetect::~HGCircleDetect()
{

}
bool HGCircleDetect::detectBottleCap2(const cv::Mat &gray){
    cv::Mat normalized;
    // 2. 灰度图归一化到 0~255，消除光照影响
    cv::normalize(gray, normalized, 0, 255, cv::NORM_MINMAX);
    cv::Mat binary;
    cv::threshold(normalized, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imwrite("binary.bmp",binary);
    return true;
}
// 函数用于检测瓶盖是否存在
bool HGCircleDetect::detectBottleCap(const cv::Mat &gray,const cv::Rect& calRoi,cv::Mat &dst) {
    cv::Mat normalized;
    // 2. 灰度图归一化到 0~255，消除光照影响
    cv::normalize(gray, normalized, 0, 255, cv::NORM_MINMAX);


    cv::Mat gaussianSrc = _gaussian_filter(normalized);
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

    low*=2;
    high*=4;
    //利用计算出来的low和high传入opencv Canny进行对比
    printf("canny (%d,%d)\n",low,high);
    cv::Mat opencvCanny;
    cv::Canny(normalized, opencvCanny, low, high, apertureSize, L2gradient);

    imwrite("normal.jpg",normalized);
    imwrite("canny.jpg",opencvCanny);

    // cv::Point minLoc,maxLoc;
    // double minVal,maxVal;
    // cv::minMaxLoc(opencvCanny,&minVal,&maxVal,&minLoc,&maxLoc);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    // 查找轮廓
    cv::findContours(opencvCanny, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 计算轮廓总面积
    double totalArea = 0;
    for (const auto& contour : contours) {
        totalArea += contourArea(contour);
    }

    // 根据实际情况设置合适的阈值，这里只是示例
    double areaThreshold = 300; 
    printf("totalArea:%.2f\n",totalArea);

    std::ostringstream content;
    bool flag=false;
    // content<<"min:"<<minVal<<",max:"<<maxVal<<",totalArea:"<<totalArea;
    content<<"totalArea:"<<totalArea;
    cv::Scalar color(0,255,0);
    // if ((maxVal-128.00001) > 0.00001) 
    if (totalArea > areaThreshold)
    {
        content << ",[Y]";
        flag=true;
        color = cv::Scalar(0,255,0);
        cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,255,0),-1);
    } else {
        content << ",[N]";
        flag=false;
        color = cv::Scalar(0,0,255);
        cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,0,255),-1);
    }
    cv::rectangle(dst, cv::Point(calRoi.x,calRoi.y), cv::Point(calRoi.x+calRoi.width,calRoi.y+calRoi.height),cv::Scalar(0,0,255));
    cv::putText(dst,content.str().c_str(),cv::Point(calRoi.x,calRoi.y+calRoi.height*0.1),
        cv::FONT_HERSHEY_COMPLEX,1.0,color);

    return flag;
}
// 比较结构，用于排序
struct ContourWithArea {
    double area;
    std::vector<cv::Point> contour;
};
void HGCircleDetect::detectDistance(cv::Mat &frame,cv::Mat& dst, cv::Mat &gray,int &targetPosX){
    // 分离 BGR 通道
    std::vector<cv::Mat> channels;

    if (frame.channels() != 1)
    {
        dst=frame.clone();
        // frame.copyTo(dst);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::split(frame, channels);

        // 分别显示三通道图像
        cv::imwrite("B.bmp", channels[0]);  // 蓝色通道
        cv::imwrite("G.bmp", channels[1]); // 绿色通道
        cv::imwrite("R.bmp", channels[2]);   // 红色通道
    }
    else
    {
        cv::cvtColor(frame,dst,cv::COLOR_GRAY2BGR);
        frame.copyTo(gray);
    }
    cv::Mat diff_r_g;
    cv::subtract(channels[2],channels[1],diff_r_g);
    cv::imwrite("diff_r_g.bmp",diff_r_g);
    cv::Mat diffNormalized;
    cv::normalize(diff_r_g, diffNormalized, 0, 255, cv::NORM_MINMAX);
    cv::imwrite("diffNormalized.bmp",diffNormalized);

    cv::Mat binary;
    cv::threshold(diffNormalized, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imwrite("diff_r_g_binary.bmp",binary);
    std::vector<std::vector<cv::Point>> contoursDiff;
    std::vector<cv::Vec4i> hierarchyDiff;
    cv::findContours(binary, contoursDiff, hierarchyDiff, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 提取面积信息
    std::vector<ContourWithArea> contourAreas;
    for (const auto& c : contoursDiff) {
        double area = cv::contourArea(c);
        contourAreas.push_back({area, c});
    }

    // 按面积降序排序
    std::sort(contourAreas.begin(), contourAreas.end(), [](const ContourWithArea& a, const ContourWithArea& b) {
        return a.area > b.area;
    });

    // 绘制面积最大的两个轮廓及其外接矩形
    int lineX=0;
    for (int i = 0; i < std::min(2, (int)contourAreas.size()); ++i) {
        const auto& c = contourAreas[i].contour;
        // 外接矩形
        cv::Rect rect = cv::boundingRect(c);
        cv::rectangle(dst, rect, cv::Scalar(0, 255, 0), 2);
        std::ostringstream text;
        text<<c.size() << "," << rect.x << "," << rect.y << "," << rect.width << "," << rect.height;
        cv::putText(dst,text.str().c_str(),cv::Point(rect.x,rect.y),cv::FONT_HERSHEY_COMPLEX,1.0,cv::Scalar(0,255,0));
        // 也可以绘制轮廓本身
        cv::drawContours(dst, std::vector<std::vector<cv::Point>>{c}, -1, cv::Scalar(255, 0, 0), 2);
        lineX+=(rect.x+rect.width/2);
    }
    lineX/=2;
    cv::line(dst,cv::Point(lineX,0),cv::Point(lineX,dst.rows),cv::Scalar(0,255,0));

    targetPosX=lineX;
    cv::imwrite("detect.bmp",dst);
}
bool HGCircleDetect::detect(cv::Mat& frame,const cv::Rect& roi,int &targetPosX)
{
    cv::imwrite("frame.bmp",frame);
    cv::Mat dst;
    cv::Mat gray;
    int width = roi.width < frame.cols/2 ? roi.width : frame.cols/2;
    int height = roi.height < frame.rows/2 ? roi.height : frame.rows/2;
    cv::Rect calRoi = cv::Rect(frame.cols/2-width/2,frame.rows/2-height/2,width,height);
    calRoi=cv::Rect(frame.cols/3,frame.rows/2,frame.cols/3,frame.rows/2);
    cv::Mat roiMat = frame(calRoi);

    // detect distance
    detectDistance(frame,dst,gray,targetPosX);
    // detect existance
    bool flag=detectBottleCap(gray(calRoi),calRoi,dst);
    
    detectBottleCap2(gray);
    dst.copyTo(frame);
    return flag;
}

}
