#ifndef CAMERACONTROLINTERFACE_H
#define CAMERACONTROLINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "HGMacroData.h"
#include "opencv2/opencv.hpp"
#include "CameraControlInterface_global.h"
#include "HGError.h"

namespace HGMACHINE{

class CameraControlInterface
{
public:
    CameraControlInterface();
    ~CameraControlInterface();

    std::vector<std::string> getCameraList(const std::string& type);
    void openCamera(const std::string &type="USB", const std::string& name="video=0");
    HGImg2D getImgOneShot(const std::string &type="USB", const std::string& name="video=0");
    cv::Mat getImgOneShotMat(const std::string &type="USB", const std::string& name="video=0");
    void closeCamera(const std::string &type="USB", const std::string& name="video=0");

    ErrorInfo getLastError() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CAMERACONTROLINTERFACE_H
