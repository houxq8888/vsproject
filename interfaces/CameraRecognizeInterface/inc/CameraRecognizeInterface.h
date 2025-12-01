#ifndef CAMERARECOGNIZEINTERFACE_H
#define CAMERARECOGNIZEINTERFACE_H
/*!
\mainpage CameraRecognizeInterface 文档

## 📌 项目简介
CameraRecognizeInterface 是一个用于将数据保存到数据库的模块，支持 SQLite，MySQL 等数据库驱动，具备线程安全、事务支持、批处理等特性。

---

## 📚 文档版本说明

| 版本号   | 日期       | 作者      | 说明                                                         |
|--------|------------|----------|--------------------------------------------------------------|
| v1.0.0 | 2025-10-17 | Xiaoqin Hou | 🎉 初始版本。生成了基础的 API 文档，涵盖全部 .h/.cpp 接口说明 |


---
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "HGMacroData.h"
#include "opencv2/opencv.hpp"

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE{

    /**
     * @brief 获取摄像头列表
     * @param type 摄像头类型, "USB", "IP"
     * @return 摄像头列表
     */
    std::vector<std::string> getCameraList(const std::string& type);
    
    /**
     * @brief 打开摄像头
     * @param type 摄像头类型, "USB", "IP"
     * @param name 摄像头名称
     */
    void openCamera(const std::string &type, const std::string& name);

cv::Mat getImgOneShotMat(const std::string &type,const std::string& name);
std::string saveCameraTemplateMat(const cv::Mat& img, const HGRect2D& rect);
    /**
     * @brief 关闭摄像头
     * @param type 摄像头类型, "USB", "IP"
     * @param name 摄像头名称
     */
    void closeCamera(const std::string &type,const std::string& name);
    /**
     * @brief 获取摄像头图片
     * @param type 摄像头类型, "USB", "IP"
     * @param name 摄像头名称
     * @return 摄像头图片
     */
    HGImg2D getImgOneShot(const std::string &type,const std::string& name);

    /**
     * @brief 保存摄像头模板
     * @param img 摄像头图片
     * @param rect 模板区域, (left,top,right,bottom)
     * @return 模板名称 default+time
     * @note 模板名称默认为 "default"
     */
    std::string saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect);

    /**
     * @brief 识别摄像头模板
     * @param img 摄像头图片
     * @param templateName 模板名称
     * @return 识别结果
      * @note 模板名称默认为 "default"
     */
    MatchResult2D recognizeCameraTemplate(const HGImg2D& img, const std::string &templateName="default");
}

#ifdef __cplusplus
}
#endif
#endif // CAMERARECOGNIZEINTERFACE_H