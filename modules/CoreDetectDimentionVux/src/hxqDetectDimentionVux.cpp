#include "hxqDetectDimentionVux.h"
#include <opencv2/opencv.hpp>
#include "hglogservice.h"
#include "hgcommonutility.h"
#include "hxqDetectDimentionVux_global.h"

namespace HGMACHINE {

HXQDetectDimentionVux::HXQDetectDimentionVux()
{

}
HXQDetectDimentionVux::~HXQDetectDimentionVux()
{

}
void HXQDetectDimentionVux::detDimentionVux(const HGImg2D &img, const HGRect2D &roi)
{
    std::ostringstream ss, press;
    press << HXQDETECTDIIMENTIONVUXNAME<<"|"<<HXQDETECTDIIMENTIONVUXAUTHOR<<"|";
    ss<<press.str()<<"start det dimention vux";
    HGLogService::getInstance(HGLogService::getLogPath())->logInfo(ss.str());
    cv::Rect inputRoi(cv::Point(roi.x1,roi.y1),cv::Point(roi.x2,roi.y2));

    ss.str("");
    ss<<press.str()<<"input ROI["<<inputRoi.tl().x<<","<<inputRoi.tl().y<<","<<inputRoi.br().x<<","<<inputRoi.br().y
     <<"],img info["<<img.width<<","<<img.height<<","<<img.type;
    HGLogService::getInstance(HGLogService::getLogPath())->logInfo(ss.str());

    cv::Mat mat(img.height,img.width,CV_8UC1,(uchar*)img.data);

    HGExactTime start=HGGetTime();
    m_dimentionValue= m_dimentionVux.detect(mat,inputRoi);
    HGExactTime end=HGGetTime();
    m_dst.data=mat.data;
    m_dst.type=mat.type();
    m_dst.width=mat.cols;
    m_dst.height=mat.rows;
    ss.str("");
    ss<<press.str()<<"elapsed time:"<<HGCalTimeElapsed(start,end)<<" ms";
    HGLogService::getInstance(HGLogService::getLogPath())->logInfo(ss.str());
}
}
