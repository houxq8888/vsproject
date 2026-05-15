#ifndef CAMERARECOGNIZEINTERFACE_H
#define CAMERARECOGNIZEINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#include "HGCommonTypes.h"

namespace HGMACHINE{

class CameraRecognizeInterface
{
public:
    CameraRecognizeInterface();
    ~CameraRecognizeInterface();

    std::vector<std::string> getCameraList(const std::string& type);
    void openCamera(const std::string &type="USB", const std::string& name="video=0");
    cv::Mat getImgOneShot(const std::string &type="USB", const std::string& name="video=0");
    void closeCamera(const std::string &type="USB", const std::string& name="video=0");

    void detCupExistence(const cv::Mat &img, int x, int y, int width, int height);
    void detCircle(const cv::Mat &img, int x, int y, int width, int height);
    bool getAbsenseFlag();
    cv::Mat getDst();
    int getTargetPosX();

    std::string saveCameraTemplate(const cv::Mat& img, int x, int y, int width, int height);
    std::string saveCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height);
    MatchResult2D recognizeCameraTemplate(const cv::Mat& img, int x, int y, int width, int height, const float& score, const std::string &templateName="default");
    MatchResult2D recognizeCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height, const float &score, const std::string &templateName="default");

    bool hasError() const;
    std::string getErrorMessage() const;
    HGErrorDetail getErrorDetail() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CAMERARECOGNIZEINTERFACE_H