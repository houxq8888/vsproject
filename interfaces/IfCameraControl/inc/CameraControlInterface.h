#ifndef CAMERACONTROLINTERFACE_H
#define CAMERACONTROLINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#include "CameraControlInterface_global.h"
#include "HGCommonTypes.h"

namespace HGMACHINE{

class CameraControlInterface
{
public:
    CameraControlInterface();
    ~CameraControlInterface();

    std::vector<std::string> getCameraList(const std::string& type);
    void openCamera(const std::string &type="USB", const std::string& name="video=0");
    cv::Mat getImgOneShot(const std::string &type="USB", const std::string& name="video=0");
    void closeCamera(const std::string &type="USB", const std::string& name="video=0");

    bool hasError() const;
    std::string getErrorMessage() const;
    HGErrorDetail getErrorDetail() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CAMERACONTROLINTERFACE_H
