#include "CameraControlInterface.h"
#include "usbcameramanager.h"
#include "ipcameramanager.h"
#include "ISvcError.h"
#include "SvcErrorAdapter.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace HGMACHINE{

class CameraControlInterface::Impl {
public:
    SvcErrorAdapter m_lastError;

    void setError(HGErrorCode code, HGErrorSeverity severity, const std::string& message) {
        m_lastError.set(code, severity, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    bool validateCameraType(const std::string& type) {
        if (type != "USB" && type != "IP") {
            setError(HGErrorCode::CAMERA_CONTROL_INVALID_CAMERA_TYPE, 
                    HGErrorSeverity::ERROR,
                    "Invalid camera type: " + type + ". Must be 'USB' or 'IP'");
            return false;
        }
        return true;
    }

    bool validateCameraName(const std::string& name) {
        if (name.empty()) {
            setError(HGErrorCode::CAMERA_CONTROL_INVALID_CAMERA_NAME, 
                    HGErrorSeverity::ERROR,
                    "Camera name cannot be empty");
            return false;
        }
        return true;
    }

    std::vector<std::string> getCameraList(const std::string& type) {
        clearError();
        
        if (!validateCameraType(type)) {
            return std::vector<std::string>();
        }
        
        try {
            if (type == "USB")
                return USBCameraManager::instance().discoverCameras();
            else if (type == "IP")
                return IPCameraManager::instance().discoverCameras();
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_NOT_FOUND, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to discover cameras: ") + e.what());
        }
        
        return std::vector<std::string>();
    }

    void openCamera(const std::string &type, const std::string& name) {
        clearError();
        
        if (!validateCameraType(type)) {
            return;
        }
        
        if (!validateCameraName(name)) {
            return;
        }
        
        try {
            if (type == "USB")
                USBCameraManager::instance().openCamera(name);
            else if (type == "IP")
                IPCameraManager::instance().openCamera(name);
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_OPEN_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to open camera: ") + e.what());
        }
    }

    void closeCamera(const std::string &type, const std::string& name) {
        clearError();
        
        if (!validateCameraType(type)) {
            return;
        }
        
        if (!validateCameraName(name)) {
            return;
        }
        
        try {
            if (type == "USB")
                USBCameraManager::instance().closeCamera(name);
            else if (type == "IP")
                IPCameraManager::instance().closeCamera(name);
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_CLOSE_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to close camera: ") + e.what());
        }
    }

    HGImg2D getImgOneShot(const std::string &type, const std::string& name) {
        clearError();
        
        if (!validateCameraType(type)) {
            return HGImg2D();
        }
        
        if (!validateCameraName(name)) {
            return HGImg2D();
        }
        
        HGImg2D img;
        cv::Mat mat;
        
        try {
            for (int i = 0; i < 5; i++) {
                if (type == "USB") {
                    if (-1 == USBCameraManager::instance().getFrameOne(name, mat)) {
                        setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                                HGErrorSeverity::ERROR,
                                "Failed to get frame from USB camera");
                        return img;
                    }
                } else if (type == "IP") {
                    if (-1 == IPCameraManager::instance().getFrameOne(name, mat)) {
                        setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                                HGErrorSeverity::ERROR,
                                "Failed to get frame from IP camera");
                        return img;
                    }
                }
            }
            
            if (mat.depth() != CV_8U) {
                setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                        HGErrorSeverity::ERROR,
                        "Only 8-bit image supported in HGImg2D");
                return img;
            }
            
            img.data = mat.data;
            img.width = mat.cols;
            img.height = mat.rows;
            img.type = mat.type();

            cv::imwrite("oneshot.bmp", mat);
            printf("type:%d\n", mat.type());
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to get frame: ") + e.what());
        }
        
        return img;
    }

    cv::Mat getImgOneShotMat(const std::string &type, const std::string& name) {
        clearError();
        
        if (!validateCameraType(type)) {
            return cv::Mat();
        }
        
        if (!validateCameraName(name)) {
            return cv::Mat();
        }
        
        cv::Mat mat;
        
        try {
            if (type == "USB") {
                if (-1 == USBCameraManager::instance().getFrameOne(name, mat)) {
                    setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                            HGErrorSeverity::ERROR,
                            "Failed to get frame from USB camera");
                    return mat;
                }
            } else if (type == "IP") {
                if (-1 == IPCameraManager::instance().getFrameOne(name, mat)) {
                    setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                            HGErrorSeverity::ERROR,
                            "Failed to get frame from IP camera");
                    return mat;
                }
            }
            
            if (mat.depth() != CV_8U) {
                setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                        HGErrorSeverity::ERROR,
                        "Only 8-bit image supported in HGImg2D");
                return cv::Mat();
            }
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to get frame: ") + e.what());
        }

        return mat;
    }
};

CameraControlInterface::CameraControlInterface() : m_impl(new Impl()) {
}

CameraControlInterface::~CameraControlInterface() {
    delete m_impl;
}

std::vector<std::string> CameraControlInterface::getCameraList(const std::string& type) {
    return m_impl->getCameraList(type);
}

void CameraControlInterface::openCamera(const std::string &type, const std::string& name) {
    m_impl->openCamera(type, name);
}

void CameraControlInterface::closeCamera(const std::string &type, const std::string& name) {
    m_impl->closeCamera(type, name);
}

cv::Mat CameraControlInterface::getImgOneShot(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShotMat(type, name);
}

bool CameraControlInterface::hasError() const {
    return m_impl->m_lastError.hasError();
}

std::string CameraControlInterface::getErrorMessage() const {
    return m_impl->m_lastError.toString();
}

HGErrorDetail CameraControlInterface::getErrorDetail() const {
    HGErrorDetail detail;
    detail.code = static_cast<int>(m_impl->m_lastError.code());
    detail.category = errorCodeToString(m_impl->m_lastError.code());
    detail.message = m_impl->m_lastError.message();
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    detail.timestamp = ss.str();
    
    return detail;
}

void CameraControlInterface::clearError() {
    m_impl->clearError();
}

}
