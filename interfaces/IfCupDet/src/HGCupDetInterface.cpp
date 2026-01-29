#include "HGCupDetInterface.h"
#include "hgcupdet.h"
#include "ISvcError.h"
#include "SvcErrorAdapter.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace HGMACHINE {

class HGCupDetInterface::Impl {
public:
    HGCupDet hgcupDet;
    SvcErrorAdapter m_lastError;

    void setError(HGErrorCode code, HGErrorSeverity severity, const std::string& message) {
        m_lastError.set(code, severity, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    bool validateImage(const cv::Mat& img) {
        if (img.empty()) {
            setError(HGErrorCode::HG_CUP_DET_INVALID_IMAGE, 
                    HGErrorSeverity::ERROR,
                    "Image is empty");
            return false;
        }
        return true;
    }

    bool validateROI(int x, int y, int width, int height) {
        if (x < 0 || y < 0 || width <= 0 || height <= 0) {
            setError(HGErrorCode::HG_CUP_DET_INVALID_ROI, 
                    HGErrorSeverity::ERROR,
                    "Invalid ROI coordinates");
            return false;
        }
        return true;
    }
};

HGCupDetInterface::HGCupDetInterface() : m_impl(new Impl()) {
}

HGCupDetInterface::~HGCupDetInterface() {
    delete m_impl;
}

void HGCupDetInterface::detCupExistence(const cv::Mat &img, int x, int y, int width, int height) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return;
    }
    
    if (!m_impl->validateROI(x, y, width, height)) {
        return;
    }
    
    try {
        HGImg2D hgImg;
        hgImg.data = img.data;
        hgImg.width = img.cols;
        hgImg.height = img.rows;
        hgImg.type = img.type();
        
        HGRect2D roi(x, y, x + width, y + height);
        m_impl->hgcupDet.detCupExistence(hgImg, roi);
    } catch (const std::exception& e) {
        m_impl->setError(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                        HGErrorSeverity::ERROR,
                        std::string("Detection failed: ") + e.what());
    }
}

void HGCupDetInterface::detCircle(const cv::Mat &img, int x, int y, int width, int height) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return;
    }
    
    if (!m_impl->validateROI(x, y, width, height)) {
        return;
    }
    
    try {
        HGImg2D hgImg;
        hgImg.data = img.data;
        hgImg.width = img.cols;
        hgImg.height = img.rows;
        hgImg.type = img.type();
        
        HGRect2D roi(x, y, x + width, y + height);
        m_impl->hgcupDet.detCircle(hgImg, roi);
    } catch (const std::exception& e) {
        m_impl->setError(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                        HGErrorSeverity::ERROR,
                        std::string("Detection failed: ") + e.what());
    }
}

void HGCupDetInterface::matchTemplate(const cv::Mat &img, int x, int y, int width, int height, const std::string &templateName) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return;
    }
    
    if (!m_impl->validateROI(x, y, width, height)) {
        return;
    }
    
    try {
        HGImg2D hgImg;
        hgImg.data = img.data;
        hgImg.width = img.cols;
        hgImg.height = img.rows;
        hgImg.type = img.type();
        
        HGRect2D roi(x, y, x + width, y + height);
        m_impl->hgcupDet.matchTemplate(hgImg, roi, templateName);
    } catch (const std::exception& e) {
        m_impl->setError(HGErrorCode::HG_CUP_DET_DETECTION_FAILED, 
                        HGErrorSeverity::ERROR,
                        std::string("Template matching failed: ") + e.what());
    }
}

bool HGCupDetInterface::getAbsenseFlag() {
    return m_impl->hgcupDet.getAbsenseFlag();
}

float HGCupDetInterface::getMatchScore() {
    return m_impl->hgcupDet.getMatchScore();
}

bool HGCupDetInterface::getMatchFlag() {
    return m_impl->hgcupDet.getMatchFlag();
}

HGRect2D HGCupDetInterface::getRect() {
    return m_impl->hgcupDet.getRect();
}

cv::Mat HGCupDetInterface::getDst() {
    HGImg2D hgImg = m_impl->hgcupDet.getDst();
    if (hgImg.data == nullptr || hgImg.width <= 0 || hgImg.height <= 0) {
        return cv::Mat();
    }
    return cv::Mat(hgImg.height, hgImg.width, hgImg.type, const_cast<unsigned char*>(hgImg.data));
}

int HGCupDetInterface::getTargetPosX() {
    return m_impl->hgcupDet.getTargetPosX();
}

std::string HGCupDetInterface::saveTemplate(const cv::Mat& img, int x, int y, int width, int height, const std::string& templateDir) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return "failed";
    }
    
    try {
        std::string result = m_impl->hgcupDet.saveTemplate(img, x, y, width, height, templateDir);
        if (result == "failed") {
            m_impl->setError(HGErrorCode::HG_CUP_DET_TEMPLATE_SAVE_FAILED, 
                            HGErrorSeverity::ERROR,
                            "Failed to save template");
        }
        return result;
    } catch (const std::exception& e) {
        m_impl->setError(HGErrorCode::HG_CUP_DET_TEMPLATE_SAVE_FAILED, 
                        HGErrorSeverity::ERROR,
                        std::string("Failed to save template: ") + e.what());
        return "failed";
    }
}

bool HGCupDetInterface::hasError() const {
    return m_impl->m_lastError.hasError();
}

std::string HGCupDetInterface::getErrorMessage() const {
    return m_impl->m_lastError.toString();
}

HGErrorDetail HGCupDetInterface::getErrorDetail() const {
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

void HGCupDetInterface::clearError() {
    m_impl->clearError();
}

}
