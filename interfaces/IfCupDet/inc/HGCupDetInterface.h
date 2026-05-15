#ifndef HGCUPDETINTERFACE_H
#define HGCUPDETINTERFACE_H

#include "HGCupDetInterface_global.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "HGCommonTypes.h"

namespace HGMACHINE {

class HGCupDetInterface
{
public:
    HGCupDetInterface();
    ~HGCupDetInterface();

    void detCupExistence(const cv::Mat &img, int x, int y, int width, int height);
    void detCircle(const cv::Mat &img, int x, int y, int width, int height);
    void matchTemplate(const cv::Mat &img, int x, int y, int width, int height, const std::string &templateName);
    bool getAbsenseFlag();
    float getMatchScore();
    bool getMatchFlag();
    HGRect2D getRect();
    cv::Mat getDst();
    int getTargetPosX();
    std::string saveTemplate(const cv::Mat& img, int x, int y, int width, int height, const std::string& templateDir = "template");

    bool hasError() const;
    std::string getErrorMessage() const;
    HGErrorDetail getErrorDetail() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // HGCUPDETINTERFACE_H
