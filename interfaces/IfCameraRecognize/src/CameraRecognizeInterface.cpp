#include "CameraRecognizeInterface.h"
#include "CameraControlInterface.h"
#include "HGCupDetInterface.h"
#include "ISvcError.h"
#include "SvcErrorAdapter.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace HGMACHINE{

class CameraRecognizeInterface::Impl {
public:
    Impl() : m_cameraControl(new CameraControlInterface()), m_cupDet(new HGCupDetInterface()) {}
    ~Impl() {
        delete m_cameraControl;
        delete m_cupDet;
    }

    SvcErrorAdapter m_lastError;

    void setError(HGErrorCode code, HGErrorSeverity severity, const std::string& message) {
        m_lastError.set(code, severity, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    void propagateError() {
        if (m_cameraControl->hasError()) {
            m_lastError.set(HGErrorCode::CAMERA_CONTROL_CAMERA_NOT_FOUND, 
                          HGErrorSeverity::ERROR,
                          m_cameraControl->getErrorMessage());
        }
        if (m_cupDet->hasError()) {
            m_lastError.set(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                          HGErrorSeverity::ERROR,
                          m_cupDet->getErrorMessage());
        }
    }

    std::vector<std::string> getCameraList(const std::string& type) {
        clearError();
        
        try {
            auto result = m_cameraControl->getCameraList(type);
            propagateError();
            return result;
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_NOT_FOUND, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to get camera list: ") + e.what());
            return std::vector<std::string>();
        }
    }

    void openCamera(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            m_cameraControl->openCamera(type, name);
            propagateError();
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_OPEN_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to open camera: ") + e.what());
        }
    }

    void closeCamera(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            m_cameraControl->closeCamera(type, name);
            propagateError();
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_CAMERA_CLOSE_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to close camera: ") + e.what());
        }
    }

    cv::Mat getImgOneShot(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            auto result = m_cameraControl->getImgOneShot(type, name);
            propagateError();
            return result;
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to get image: ") + e.what());
            return cv::Mat();
        }
    }

    void detCupExistence(const cv::Mat &img, int x, int y, int width, int height) {
        clearError();
        
        try {
            m_cupDet->detCupExistence(img, x, y, width, height);
            propagateError();
        } catch (const std::exception& e) {
            setError(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Detection failed: ") + e.what());
        }
    }

    void detCircle(const cv::Mat &img, int x, int y, int width, int height) {
        clearError();
        
        try {
            m_cupDet->detCircle(img, x, y, width, height);
            propagateError();
        } catch (const std::exception& e) {
            setError(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Detection failed: ") + e.what());
        }
    }

    bool getAbsenseFlag() {
        clearError();
        return m_cupDet->getAbsenseFlag();
    }

    cv::Mat getDst() {
        clearError();
        return m_cupDet->getDst();
    }

    int getTargetPosX() {
        clearError();
        return m_cupDet->getTargetPosX();
    }

    std::string saveCameraTemplate(const cv::Mat& img, int x, int y, int width, int height) {
        clearError();
        
        try {
            std::string result = m_cupDet->saveTemplate(img, x, y, width, height, "template");
            propagateError();
            return result;
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_RECOGNIZE_TEMPLATE_SAVE_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to save template: ") + e.what());
            return "failed";
        }
    }

    std::string saveCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height) {
        clearError();
        
        try {
            if (img.empty()) {
                setError(HGErrorCode::CAMERA_RECOGNIZE_TEMPLATE_SAVE_FAILED, 
                        HGErrorSeverity::ERROR,
                        "Input image is empty");
                return "failed";
            }
            
            cv::Mat mat = img.clone();
            
            if (mat.channels() == 3) {
                cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            }
            
            std::string result = m_cupDet->saveTemplate(mat, x, y, width, height, "template");
            propagateError();
            return result;
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_RECOGNIZE_TEMPLATE_SAVE_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Failed to save template: ") + e.what());
            return "failed";
        }
    }

    MatchResult2D recognizeCameraTemplate(const cv::Mat& img, int x, int y, int width, int height, const float& score, const std::string &templateName) {
        clearError();
        
        MatchResult2D result;
        
        try {
            if (templateName==""){
                m_cupDet->detCupExistence(img, x, y, width, height);
                propagateError();
                result.flag = m_cupDet->getAbsenseFlag();
                result.score = result.flag ? 1.0f : 0.0f;
                result.name = templateName;
            } else {
                m_cupDet->matchTemplate(img, x, y, width, height, templateName);
                propagateError();
                result.flag = m_cupDet->getMatchFlag();
                result.score = m_cupDet->getMatchScore();
                result.name = templateName;
                result.rect = m_cupDet->getRect();
                result.dstMat = m_cupDet->getDst();
            }
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_RECOGNIZE_RECOGNITION_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Recognition failed: ") + e.what());
            result.flag = false;
            result.score = 0.0f;
        }
        
        return result;
    }

    MatchResult2D recognizeCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height, const float &score, const std::string &templateName) {
        clearError();
        
        MatchResult2D result;
        
        try {
            if (templateName==""){
                m_cupDet->detCupExistence(img, x, y, width, height);
                propagateError();
                result.flag = m_cupDet->getAbsenseFlag();
                result.score = result.flag ? 1.0f : 0.0f;
                result.name = templateName;
            } else {
                m_cupDet->matchTemplate(img, x, y, width, height, templateName);
                propagateError();
                result.flag = m_cupDet->getMatchFlag();
                result.score = m_cupDet->getMatchScore();
                result.name = templateName;
                result.rect = m_cupDet->getRect();  
                result.dstMat = m_cupDet->getDst();
            }
        } catch (const std::exception& e) {
            setError(HGErrorCode::CAMERA_RECOGNIZE_RECOGNITION_FAILED, 
                    HGErrorSeverity::ERROR,
                    std::string("Recognition failed: ") + e.what());
            result.flag = false;
            result.score = 0.0f;
        }
        
        return result;
    }

private:
    CameraControlInterface* m_cameraControl;
    HGCupDetInterface* m_cupDet;
};

CameraRecognizeInterface::CameraRecognizeInterface() : m_impl(new Impl()) {
}

CameraRecognizeInterface::~CameraRecognizeInterface() {
    delete m_impl;
}

std::vector<std::string> CameraRecognizeInterface::getCameraList(const std::string& type) {
    return m_impl->getCameraList(type);
}

void CameraRecognizeInterface::openCamera(const std::string &type, const std::string& name) {
    m_impl->openCamera(type, name);
}

void CameraRecognizeInterface::closeCamera(const std::string &type, const std::string& name) {
    m_impl->closeCamera(type, name);
}

cv::Mat CameraRecognizeInterface::getImgOneShot(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShot(type, name);
}

void CameraRecognizeInterface::detCupExistence(const cv::Mat &img, int x, int y, int width, int height) {
    m_impl->detCupExistence(img, x, y, width, height);
}

void CameraRecognizeInterface::detCircle(const cv::Mat &img, int x, int y, int width, int height) {
    m_impl->detCircle(img, x, y, width, height);
}

bool CameraRecognizeInterface::getAbsenseFlag() {
    return m_impl->getAbsenseFlag();
}

cv::Mat CameraRecognizeInterface::getDst() {
    return m_impl->getDst();
}

int CameraRecognizeInterface::getTargetPosX() {
    return m_impl->getTargetPosX();
}

std::string CameraRecognizeInterface::saveCameraTemplate(const cv::Mat& img, int x, int y, int width, int height) {
    return m_impl->saveCameraTemplate(img, x, y, width, height);
}

std::string CameraRecognizeInterface::saveCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height) {
    return m_impl->saveCameraTemplateMat(img, x, y, width, height);
}

MatchResult2D CameraRecognizeInterface::recognizeCameraTemplate(const cv::Mat& img, int x, int y, int width, int height, const float& score, const std::string &templateName) {
    return m_impl->recognizeCameraTemplate(img, x, y, width, height, score, templateName);
}

MatchResult2D CameraRecognizeInterface::recognizeCameraTemplateMat(const cv::Mat& img, int x, int y, int width, int height, const float &score, const std::string &templateName) {
    return m_impl->recognizeCameraTemplateMat(img, x, y, width, height, score, templateName);
}

bool CameraRecognizeInterface::hasError() const {
    return m_impl->m_lastError.hasError();
}

std::string CameraRecognizeInterface::getErrorMessage() const {
    return m_impl->m_lastError.toString();
}

HGErrorDetail CameraRecognizeInterface::getErrorDetail() const {
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

void CameraRecognizeInterface::clearError() {
    m_impl->clearError();
}

}
