#ifndef CAMERARECOGNIZEINTERFACE_H
#define CAMERARECOGNIZEINTERFACE_H
/*!
\mainpage CameraRecognizeInterface 文档

## 📌 项目简介
CameraRecognizeInterface 是一个视频检测集成模块，集成了摄像头控制和杯子检测功能。

---

## 📚 文档版本说明

| 版本号   | 日期       | 作者      | 说明                                                         |
|--------|------------|----------|--------------------------------------------------------------|
| v1.0.0 | 2025-10-17 | Xiaoqin Hou | 🎉 初始版本。生成了基础的 API 文档，涵盖全部 .h/.cpp 接口说明 |
| v2.0.0 | 2026-01-20 | Xiaoqin Hou | 🔧 重构版本。分离设备控制和业务逻辑，集成 CameraControlInterface 和 HGCupDetInterface |
| v3.0.0 | 2026-01-20 | Xiaoqin Hou | 🎯 面向对象版本。添加 C++ 类接口，移除全局变量 |
| v4.0.0 | 2026-01-20 | Xiaoqin Hou | 🚀 纯面向对象版本。移除全局函数，只保留 C++ 类接口 |
| v5.0.0 | 2026-01-20 | Xiaoqin Hou | 🛡️ 错误处理版本。添加统一的错误处理机制，提高可调试性和可靠性 |


---
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "HGMacroData.h"
#include "opencv2/opencv.hpp"
#include "CameraControlInterface.h"
#include "HGCupDetInterface.h"
#include "HGError.h"

namespace HGMACHINE{

class CameraRecognizeInterface
{
public:
    CameraRecognizeInterface();
    ~CameraRecognizeInterface();

    std::vector<std::string> getCameraList(const std::string& type);
    void openCamera(const std::string &type="USB", const std::string& name="video=0");
    HGImg2D getImgOneShot(const std::string &type="USB", const std::string& name="video=0");
    cv::Mat getImgOneShotMat(const std::string &type="USB", const std::string& name="video=0");
    void closeCamera(const std::string &type="USB", const std::string& name="video=0");

    void detCupExistence(const HGImg2D &img, const HGRect2D &roi);
    void detCircle(const HGImg2D &img, const HGRect2D &roi);
    bool getAbsenseFlag();
    HGImg2D getDst();
    int getTargetPosX();

    std::string saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect);
    std::string saveCameraTemplateMat(const cv::Mat& img, const HGRect2D& rect);
    MatchResult2D recognizeCameraTemplate(const HGImg2D& img, const HGRect2D& rect, const float& score, const std::string &templateName="default");
    MatchResult2D recognizeCameraTemplateMat(const cv::Mat& img, const HGRect2D &rect, const float &score, const std::string &templateName="default");

    ErrorInfo getLastError() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CAMERARECOGNIZEINTERFACE_H