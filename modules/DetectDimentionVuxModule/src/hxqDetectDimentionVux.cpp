#include "hxqDetectDimentionVux.h"
#include <opencv2/opencv.hpp>
#include "hglog4cplus.h"
#include "hgcommonutility.h"
#include "hxqDetectDimentionVux_global.h"

namespace HXQACHINE {

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
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
    cv::Rect inputRoi(cv::Point(roi.x1,roi.y1),cv::Point(roi.x2,roi.y2));

    ss.str("");
    ss<<press.str()<<"input ROI["<<inputRoi.tl().x<<","<<inputRoi.tl().y<<","<<inputRoi.br().x<<","<<inputRoi.br().y
     <<"],img info["<<img.width<<","<<img.height<<","<<img.type;
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);

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
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
}
}
