#include "CameraControlInterface.h"
#include "usbcameramanager.h"
#include "ipcameramanager.h"

namespace HGMACHINE{

class CameraControlInterface::Impl {
public:
    ErrorInfo m_lastError;

    void setError(ErrorCategory category, ErrorSeverity severity, const std::string& message) {
        m_lastError.set(category, severity, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    bool validateCameraType(const std::string& type) {
        if (type != "USB" && type != "IP") {
            setError(ErrorCode::CameraControl_INVALID_CAMERA_TYPE, 
                    "Invalid camera type: " + type + ". Must be 'USB' or 'IP'");
            return false;
        }
        return true;
    }

    bool validateCameraName(const std::string& name) {
        if (name.empty()) {
            setError(ErrorCode::CameraControl_INVALID_CAMERA_NAME, "Camera name cannot be empty");
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
            setError(ErrorCode::CameraControl_CAMERA_NOT_FOUND, 
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
            setError(ErrorCode::CameraControl_CAMERA_OPEN_FAILED, 
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
            setError(ErrorCode::CameraControl_CAMERA_CLOSE_FAILED, 
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
                        setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                                "Failed to get frame from USB camera");
                        return img;
                    }
                } else if (type == "IP") {
                    if (-1 == IPCameraManager::instance().getFrameOne(name, mat)) {
                        setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                                "Failed to get frame from IP camera");
                        return img;
                    }
                }
            }
            
            if (mat.depth() != CV_8U) {
                setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
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
            setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
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
                    setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                            "Failed to get frame from USB camera");
                    return mat;
                }
            } else if (type == "IP") {
                if (-1 == IPCameraManager::instance().getFrameOne(name, mat)) {
                    setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                            "Failed to get frame from IP camera");
                    return mat;
                }
            }
            
            if (mat.depth() != CV_8U) {
                setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                        "Only 8-bit image supported in HGImg2D");
                return cv::Mat();
            }
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
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

HGImg2D CameraControlInterface::getImgOneShot(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShot(type, name);
}

cv::Mat CameraControlInterface::getImgOneShotMat(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShotMat(type, name);
}

ErrorInfo CameraControlInterface::getLastError() const {
    return m_impl->m_lastError;
}

void CameraControlInterface::clearError() {
    m_impl->clearError();
}

}
