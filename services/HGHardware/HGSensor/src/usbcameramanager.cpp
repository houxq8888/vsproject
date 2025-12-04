#include "usbcameramanager.h"

#define MAX_CAMERA_NUM 10

namespace HGMACHINE{
    USBCameraManager::USBCameraManager() {
    // 初始化时可以做一些准备工作（如果有的话）
}

USBCameraManager::~USBCameraManager() {
    closeAll();  // 析构时关闭所有摄像头
}
USBCameraManager& USBCameraManager::instance() {
    static USBCameraManager instance;
    return instance;
}
// 打开指定名称的摄像头
bool USBCameraManager::openCamera(const std::string& name) {
    // 如果摄像头已经打开，返回 false
    if (m_cameras.find(name) != m_cameras.end()) {
        return false;
    }

    // 查找摄像头名称对应的索引
    int index = 0;

    // 在获取的摄像头列表中查找对应的索引
    if (m_cameralist.find(name) != m_cameralist.end()) index=m_cameralist[name];

    if (index == -1) {
        return false;  // 找不到该名称对应的摄像头
    }

    // 通过索引打开摄像头
    auto camera = std::unique_ptr<HGCapture2DFromUSB>(new HGCapture2DFromUSB());
    if (camera->open(index)) {
        m_cameras[name] = std::move(camera);  // 存储摄像头
        return true;
    }

    return false;  // 无法打开摄像头
}

// 关闭指定名称的摄像头
void USBCameraManager::closeCamera(const std::string& name) {
    auto it = m_cameras.find(name);
    if (it != m_cameras.end()) {
        it->second->close();  // 调用摄像头的关闭方法
        m_cameras.erase(it);  // 从容器中移除该摄像头
    }
}

// 获取所有打开的摄像头列表
std::vector<std::string> USBCameraManager::getCameraList() const {
    std::vector<std::string> cameraList;
    for (const auto& entry : m_cameras) {
        cameraList.push_back(entry.first);  // 使用摄像头名称
    }
    return cameraList;
}

// 获取指定摄像头的图像
int USBCameraManager::getFrameOne(const std::string& name, cv::Mat& frame) {
    auto it = m_cameras.find(name);
    if (it != m_cameras.end()) {
        return it->second->getFrameOne(frame);
    }
    return -1;  // 找不到该摄像头
}

// 获取所有可用的摄像头名称
std::vector<std::string> USBCameraManager::discoverCameras() {
    std::vector<std::string> availableCameras;

    #ifdef _WIN32
    // Windows 平台
    // 使用 DirectShow API 发现摄像头（可以使用 OpenCV 的 dshow 或 FFmpeg）
    for (int i = 0; i < MAX_CAMERA_NUM; ++i) {
        try {
            std::cout << "Attempting to open camera with index: " << i << std::endl;

            // Windows 上使用 dshow（DirectShow）
            std::string cameraPath = "video=" + std::to_string(i);
            // cv::VideoCapture cap("dshow://" + cameraPath);
            cv::VideoCapture cap(i);

            if (!cap.isOpened()) {
                std::cerr << "Unable to open camera with index " << i << std::endl;
                continue;
            }

            std::cout << "Successfully opened camera with index: " << i << std::endl;
            availableCameras.push_back(cameraPath);
            m_cameralist[cameraPath] = i;
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error while trying to open camera " << i << ": " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception while trying to open camera " << i << ": " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error while trying to open camera " << i << std::endl;
        }
    }

    #else
    // Linux 平台
    for (int i = 0; i < MAX_CAMERA_NUM; ++i) {
        try {
            std::cout << "Attempting to open camera with index: " << i << std::endl;

            // Linux 下使用 /dev/videoX 来打开摄像头
            std::string cameraPath = "/dev/video" + std::to_string(i);
            cv::VideoCapture cap(cameraPath);

            if (!cap.isOpened()) {
                std::cerr << "Unable to open camera with index " << i << std::endl;
                continue;
            }

            std::cout << "Successfully opened camera with index: " << i << std::endl;
            availableCameras.push_back(cameraPath);
            m_cameralist[cameraPath] = i;
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV error while trying to open camera " << i << ": " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception while trying to open camera " << i << ": " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error while trying to open camera " << i << std::endl;
        }
    }

    #endif

    return availableCameras;
}

// 关闭所有摄像头
void USBCameraManager::closeAll() {
    for (auto& entry : m_cameras) {
        entry.second->close();  // 关闭每个摄像头
    }
    m_cameras.clear();  // 清空容器
}
}