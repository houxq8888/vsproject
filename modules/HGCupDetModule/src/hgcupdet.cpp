#include "hgcupdet.h"
#include <opencv2/opencv.hpp>
// #include "hglog4cplus.h"
#include "hgcommonutility.h"
#include "hgdetectcircle.h"

namespace HGMACHINE {

HGCupDet::HGCupDet()
{

}
HGCupDet::~HGCupDet()
{

}
void HGCupDet::detCircle(const HGImg2D &img, const HGRect2D &roi){
    cv::Mat mat(img.height,img.width,img.type,(uchar*)img.data);
    cv::Rect inputRoi(cv::Point(roi.x1,roi.y1),cv::Point(roi.x2,roi.y2));

    HGCircleDetect circleDet;
    m_absenseFlag=circleDet.detect(mat,inputRoi,m_targetPosX);
    m_dst.data=mat.data;
    m_dst.type=mat.type();
    m_dst.width=mat.cols;
    m_dst.height=mat.rows;
}
int HGCupDet::getTargetPosX(){
    return m_targetPosX;
}
void HGCupDet::matchTemplate(const HGImg2D &img, const HGRect2D &roi,
    const std::string &templateName)
{
    cv::Mat templ = cv::imread(templateName.c_str());
    cv::Mat imgMat(img.height, img.width, img.type, (uchar *)img.data);
    if (imgMat.empty() || templ.empty()) {
        std::cerr << "加载图像失败！" << std::endl;
        m_matchFlag = false;
        return;
    }

    // 定义匹配方法
    int match_method = cv::TM_CCOEFF_NORMED;  // 使用归一化的相关系数匹配方法

    double threshold = 0.8;
    double bestMatchScore = -1.0;  // 存储最好的匹配分数
    cv::Point bestMatchLocation;   // 存储最好的匹配位置
    cv::Mat bestMatchRegion;       // 存储最好的匹配区域

    // 设置多尺度匹配参数
    double scaleFactor = 1.1;  // 每次缩小的比例
    int minTemplateSize = 30;  // 模板最小尺寸
    int maxTemplateSize = std::min(imgMat.cols, imgMat.rows);  // 最大尺寸限制为图像的最小边

    // 对模板进行多尺度匹配
    for (double scale = 1.0; scale * templ.cols <= maxTemplateSize && scale * templ.rows <= maxTemplateSize; scale *= scaleFactor) {
        // 缩放模板图像
        cv::Mat scaledTemplate;
        cv::resize(templ, scaledTemplate, cv::Size(), scale, scale, cv::INTER_LINEAR);

        // 进行模板匹配
        cv::Mat result;
        cv::matchTemplate(imgMat, scaledTemplate, result, match_method);

        // 获取最大值和位置
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

        // 判断是否为最佳匹配
        if (maxVal > bestMatchScore) {
            bestMatchScore = maxVal;
            bestMatchLocation = maxLoc;
            bestMatchRegion = imgMat(cv::Rect(bestMatchLocation.x, bestMatchLocation.y, scaledTemplate.cols, scaledTemplate.rows));
        }
    }

    // 更新匹配结果
    m_matchScore = bestMatchScore;

    // 判断是否匹配成功
    if (bestMatchScore >= threshold) {
        m_matchFlag = true;
        m_dst.data = bestMatchRegion.data;
        m_dst.type = bestMatchRegion.type();
        m_dst.width = bestMatchRegion.cols;
        m_dst.height = bestMatchRegion.rows;
        std::cout << "匹配成功！匹配位置: " << bestMatchLocation << ", 匹配值: " << bestMatchScore << std::endl;
    } else {
        m_matchFlag = false;
        std::cout << "匹配失败，最大匹配值: " << bestMatchScore << std::endl;
    }
}
void HGCupDet::detCupExistence(const HGImg2D &img, const HGRect2D &roi)
{
    std::ostringstream ss, press;
    press << HGCUPDETMODULENAME<<"|"<<HGCUPDETMODULEAUTHOR<<"|";
    ss<<press.str()<<"start det cup existence";
    // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
    cv::Rect inputRoi(cv::Point(roi.x1,roi.y1),cv::Point(roi.x2,roi.y2));

    ss.str("");
    ss<<press.str()<<"input ROI["<<inputRoi.tl().x<<","<<inputRoi.tl().y<<","<<inputRoi.br().x<<","<<inputRoi.br().y
     <<"],img info["<<img.width<<","<<img.height<<","<<img.type;
    // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);

    cv::Mat mat(img.height,img.width,img.type,(uchar*)img.data);

    HGExactTime start=HGGetTime();
    m_absenseFlag=m_plasticAbsenseDet.detect(mat,inputRoi);
    HGExactTime end=HGGetTime();
    m_dst.data=mat.data;
    m_dst.type=mat.type();
    m_dst.width=mat.cols;
    m_dst.height=mat.rows;
    ss.str("");
    ss<<press.str()<<"elapsed time:"<<HGCalTimeElapsed(start,end)<<" ms";
    // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
}
}
