#include "CameraRecognizeInterface.h"
#include "hgcommonutility.h"
#include "hgcupdet.h"
#include "rwDb.h"

namespace HGMACHINE{

class CameraRecognizeInterface::Impl {
public:
    Impl() : m_cameraControl(new CameraControlInterface()), m_cupDet(new HGCupDetInterface()) {}
    ~Impl() {
        delete m_cameraControl;
        delete m_cupDet;
    }

    ErrorInfo m_lastError;

    void setError(ErrorCode code, const std::string& message) {
        m_lastError.set(code, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    void propagateError(const ErrorInfo& error) {
        if (error.hasError()) {
            m_lastError = error;
        }
    }

    std::vector<std::string> getCameraList(const std::string& type) {
        clearError();
        
        try {
            auto result = m_cameraControl->getCameraList(type);
            propagateError(m_cameraControl->getLastError());
            return result;
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_CAMERA_NOT_FOUND, 
                    std::string("Failed to get camera list: ") + e.what());
            return std::vector<std::string>();
        }
    }

    void openCamera(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            m_cameraControl->openCamera(type, name);
            propagateError(m_cameraControl->getLastError());
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_CAMERA_OPEN_FAILED, 
                    std::string("Failed to open camera: ") + e.what());
        }
    }

    void closeCamera(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            m_cameraControl->closeCamera(type, name);
            propagateError(m_cameraControl->getLastError());
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_CAMERA_CLOSE_FAILED, 
                    std::string("Failed to close camera: ") + e.what());
        }
    }

    HGImg2D getImgOneShot(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            auto result = m_cameraControl->getImgOneShot(type, name);
            propagateError(m_cameraControl->getLastError());
            return result;
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                    std::string("Failed to get image: ") + e.what());
            return HGImg2D();
        }
    }

    cv::Mat getImgOneShotMat(const std::string &type, const std::string& name) {
        clearError();
        
        try {
            auto result = m_cameraControl->getImgOneShotMat(type, name);
            propagateError(m_cameraControl->getLastError());
            return result;
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraControl_GET_FRAME_FAILED, 
                    std::string("Failed to get image: ") + e.what());
            return cv::Mat();
        }
    }

    void detCupExistence(const HGImg2D &img, const HGRect2D &roi) {
        clearError();
        
        try {
            m_cupDet->detCupExistence(img, roi);
            propagateError(m_cupDet->getLastError());
        } catch (const std::exception& e) {
            setError(ErrorCode::HGCupDet_DETECTION_FAILED, 
                    std::string("Detection failed: ") + e.what());
        }
    }

    void detCircle(const HGImg2D &img, const HGRect2D &roi) {
        clearError();
        
        try {
            m_cupDet->detCircle(img, roi);
            propagateError(m_cupDet->getLastError());
        } catch (const std::exception& e) {
            setError(ErrorCode::HGCupDet_DETECTION_FAILED, 
                    std::string("Detection failed: ") + e.what());
        }
    }

    bool getAbsenseFlag() {
        clearError();
        return m_cupDet->getAbsenseFlag();
    }

    HGImg2D getDst() {
        clearError();
        return m_cupDet->getDst();
    }

    int getTargetPosX() {
        clearError();
        return m_cupDet->getTargetPosX();
    }

    std::string saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect) {
        clearError();
        
        try {
            HGMkDir(RWDb::readCurDirPath()+"/template");
            cv::Mat mat(img.height,img.width,img.type,(uchar*)img.data);
            int x = static_cast<int>(rect.x1);
            int y = static_cast<int>(rect.y1);
            int width = static_cast<int>(rect.x2 - rect.x1);
            int height = static_cast<int>(rect.y2 - rect.y1);
            
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x + width > mat.cols) width = mat.cols - x;
            if (y + height > mat.rows) height = mat.rows - y;
            
            if (width <= 0 || height <= 0) {
                setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, "Invalid template dimensions");
                return "failed";
            }
            
            cv::Mat roi = mat(cv::Rect(x, y, width, height));
            std::string name=RWDb::readCurDirPath()+"/template/"+"default"+getFileNameFromTime()+".bmp";
            
            if (!cv::imwrite(name.c_str(),roi)) {
                setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, "Failed to save template file");
                return "failed";
            }
            
            return name;
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, 
                    std::string("Failed to save template: ") + e.what());
            return "failed";
        }
    }

    std::string saveCameraTemplateMat(const cv::Mat& img, const HGRect2D& rect) {
        clearError();
        
        try {
            HGMkDir(RWDb::readCurDirPath()+"/template");
            if (img.empty()) {
                setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, "Input image is empty");
                return "failed";
            }
            
            cv::Mat mat=img.clone();
            
            if (mat.channels() == 3) {
                cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            }
            
            cv::Mat roi = mat(cv::Rect(rect.x1,rect.y1,rect.x2-rect.x1,rect.y2-rect.y1));
            if (roi.empty()) {
                setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, "Invalid template ROI");
                return "failed";
            }
            
            std::string name=RWDb::readCurDirPath()+"/template/"+"default"+getFileNameFromTime()+".bmp";
            
            if (!cv::imwrite(name.c_str(),roi)) {
                setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, "Failed to save template file");
                return "failed";
            }
            
            return name;
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraRecognize_TEMPLATE_SAVE_FAILED, 
                    std::string("Failed to save template: ") + e.what());
            return "failed";
        }
    }

    MatchResult2D recognizeCameraTemplate(const HGImg2D& img, const std::string &templateName) {
        clearError();
        
        MatchResult2D result;
        
        try {
            HGCupDet hgcupDet;
            
            if (templateName==""){
                hgcupDet.detCupExistence(img,HGRect2D(0,0,img.width-1,img.height-1));
                result.flag = hgcupDet.getAbsenseFlag();
                result.dst = hgcupDet.getDst();
                result.name = templateName;
                result.score = (result.flag?1:0);
            } else {
                hgcupDet.matchTemplate(img,HGRect2D(0,0,img.width-1,img.height-1),templateName);
                result.flag = hgcupDet.getMatchFlag();
                result.dst = hgcupDet.getDst();
                result.name = templateName;
                result.score = hgcupDet.getMatchScore();
                result.rect = hgcupDet.getRect();
                result.dstMat = hgcupDet.getDstMat();
            }
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraRecognize_RECOGNITION_FAILED, 
                    std::string("Recognition failed: ") + e.what());
            result.flag = false;
            result.score = 0.0f;
        }
        
        return result;
    }

    MatchResult2D recognizeCameraTemplateMat(const cv::Mat& img, const HGRect2D &rect, const float &score, const std::string &templateName) {
        clearError();
        
        MatchResult2D result;
        
        try {
            HGImg2D hgImg;
            hgImg.data = img.data;
            hgImg.width = img.cols;
            hgImg.height = img.rows;
            hgImg.type = img.type();
            
            HGCupDet hgcupDet;
            
            if (templateName==""){
                hgcupDet.detCupExistence(hgImg,HGRect2D(0,0,hgImg.width-1,hgImg.height-1));
                result.flag = hgcupDet.getAbsenseFlag();
                result.dst = hgcupDet.getDst();
                result.name = templateName;
                result.score = (result.flag?1:0);
            } else {
                hgcupDet.matchTemplate(hgImg,HGRect2D(0,0,hgImg.width-1,hgImg.height-1),templateName);
                result.flag = hgcupDet.getMatchFlag();
                result.dst = hgcupDet.getDst();
                result.name = templateName;
                result.score = hgcupDet.getMatchScore();
                result.rect = hgcupDet.getRect();
                result.dstMat = hgcupDet.getDstMat();
            }
        } catch (const std::exception& e) {
            setError(ErrorCode::CameraRecognize_RECOGNITION_FAILED, 
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

HGImg2D CameraRecognizeInterface::getImgOneShot(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShot(type, name);
}

cv::Mat CameraRecognizeInterface::getImgOneShotMat(const std::string &type, const std::string& name) {
    return m_impl->getImgOneShotMat(type, name);
}

void CameraRecognizeInterface::detCupExistence(const HGImg2D &img, const HGRect2D &roi) {
    m_impl->detCupExistence(img, roi);
}

void CameraRecognizeInterface::detCircle(const HGImg2D &img, const HGRect2D &roi) {
    m_impl->detCircle(img, roi);
}

bool CameraRecognizeInterface::getAbsenseFlag() {
    return m_impl->getAbsenseFlag();
}

HGImg2D CameraRecognizeInterface::getDst() {
    return m_impl->getDst();
}

int CameraRecognizeInterface::getTargetPosX() {
    return m_impl->getTargetPosX();
}

std::string CameraRecognizeInterface::saveCameraTemplate(const HGImg2D& img, const HGRect2D& rect) {
    return m_impl->saveCameraTemplate(img, rect);
}

std::string CameraRecognizeInterface::saveCameraTemplateMat(const cv::Mat& img, const HGRect2D& rect) {
    return m_impl->saveCameraTemplateMat(img, rect);
}

MatchResult2D CameraRecognizeInterface::recognizeCameraTemplate(const HGImg2D& img, const HGRect2D& rect, const float& score, const std::string &templateName) {
    return m_impl->recognizeCameraTemplate(img, templateName);
}

MatchResult2D CameraRecognizeInterface::recognizeCameraTemplateMat(const cv::Mat& img, const HGRect2D &rect, const float &score, const std::string &templateName) {
    return m_impl->recognizeCameraTemplateMat(img, rect, score, templateName);
}

ErrorInfo CameraRecognizeInterface::getLastError() const {
    return m_impl->m_lastError;
}

void CameraRecognizeInterface::clearError() {
    m_impl->clearError();
}

}
