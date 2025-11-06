#ifndef USBCAMERAMANAGER_H
#define USBCAMERAMANAGER_H

#include "hgcapture2dfromusb.h"
#include <mutex>
#include <map>
#include <vector>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {
class USBCameraManager
{
public:
    static USBCameraManager& instance();
    /**
     * @brief getCameraList
     * @return
      * 获取所有打开的摄像头列表
     */
    std::vector<std::string> getCameraList() const;
    /**
     * @brief discoverCameras
     * @return
      * 获取所有可用的摄像头名称
     */
    std::vector<std::string> discoverCameras();
    /**
     * @brief openCamera
     * @param name
     * @return
      * 打开指定名称的摄像头
     */
    bool openCamera(const std::string &name);
    /**
     * @brief closeCamera
     * @param name
     * @return
      * 关闭指定名称的摄像头
     */
    void closeCamera(const std::string &name);
    /**
     * @brief getFrameOne
     * @param name
     * @param frame
     * @return
      * 获取指定名称的摄像头的一帧图像
     */
    int getFrameOne(const std::string &name,cv::Mat &frame);
    /**
     * @brief closeAll
     * @return
      * 关闭所有摄像头
     */
    void closeAll();

private:
    std::map<std::string, std::unique_ptr<HGCapture2DFromUSB>> m_cameras;  // 存储多个摄像头实例，按名称映射
    std::map<std::string,int> m_cameralist;
private:
    USBCameraManager ();
    ~USBCameraManager (); // 析构函数，关闭所有摄像头
    USBCameraManager(const USBCameraManager&) = delete;
    USBCameraManager& operator=(const USBCameraManager&)=delete;
};
}
#ifdef __cplusplus
}
#endif
#endif // USBCAMERAMANAGER_H
