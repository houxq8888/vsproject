#include "HGCupDetInterface.h"
#include "hgcupdet.h"
#include "hgdetectcircle.h"

namespace HGMACHINE {

class HGCupDetInterface::Impl {
public:
    HGCupDet hgcupDet;
    ErrorInfo m_lastError;

    void setError(ErrorCode code, const std::string& message) {
        m_lastError.set(code, message);
    }

    void clearError() {
        m_lastError.clear();
    }

    bool validateImage(const HGImg2D& img) {
        if (img.data == nullptr) {
            setError(ErrorCode::HGCupDet_INVALID_IMAGE, "Image data is null");
            return false;
        }
        if (img.width <= 0 || img.height <= 0) {
            setError(ErrorCode::HGCupDet_INVALID_IMAGE, "Invalid image dimensions");
            return false;
        }
        return true;
    }

    bool validateROI(const HGRect2D& roi) {
        if (roi.x1 < 0 || roi.y1 < 0 || roi.x2 <= roi.x1 || roi.y2 <= roi.y1) {
            setError(ErrorCode::HGCupDet_INVALID_ROI, "Invalid ROI coordinates");
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

void HGCupDetInterface::detCupExistence(const HGImg2D &img, const HGRect2D &roi) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return;
    }
    
    if (!m_impl->validateROI(roi)) {
        return;
    }
    
    try {
        m_impl->hgcupDet.detCupExistence(img, roi);
    } catch (const std::exception& e) {
        m_impl->setError(ErrorCode::HGCupDet_DETECTION_FAILED, std::string("Detection failed: ") + e.what());
    }
}

void HGCupDetInterface::detCircle(const HGImg2D &img, const HGRect2D &roi) {
    m_impl->clearError();
    
    if (!m_impl->validateImage(img)) {
        return;
    }
    
    if (!m_impl->validateROI(roi)) {
        return;
    }
    
    try {
        m_impl->hgcupDet.detCircle(img, roi);
    } catch (const std::exception& e) {
        m_impl->setError(ErrorCode::HGCupDet_DETECTION_FAILED, std::string("Detection failed: ") + e.what());
    }
}

bool HGCupDetInterface::getAbsenseFlag() {
    return m_impl->hgcupDet.getAbsenseFlag();
}

HGImg2D HGCupDetInterface::getDst() {
    return m_impl->hgcupDet.getDst();
}

int HGCupDetInterface::getTargetPosX() {
    return m_impl->hgcupDet.getTargetPosX();
}

ErrorInfo HGCupDetInterface::getLastError() const {
    return m_impl->m_lastError;
}

void HGCupDetInterface::clearError() {
    m_impl->clearError();
}

}
