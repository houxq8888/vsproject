#include "ErrorAdapter.h"
#include <sstream>

namespace HGMACHINE {

ErrorAdapter::ErrorAdapter() 
    : m_code(HGErrorCode::SUCCESS)
    , m_severity(HGErrorSeverity::INFO)
    , m_message("")
{
}

ErrorAdapter::ErrorAdapter(HGErrorCode code, HGErrorSeverity severity, const std::string& message)
    : m_code(code)
    , m_severity(severity)
    , m_message(message)
{
}

HGErrorCode ErrorAdapter::code() const {
    return m_code;
}

HGErrorSeverity ErrorAdapter::severity() const {
    return m_severity;
}

const std::string& ErrorAdapter::message() const {
    return m_message;
}

bool ErrorAdapter::hasError() const {
    return m_code != HGErrorCode::SUCCESS;
}

bool ErrorAdapter::isCritical() const {
    return m_severity == HGErrorSeverity::CRITICAL;
}

void ErrorAdapter::clear() {
    m_code = HGErrorCode::SUCCESS;
    m_severity = HGErrorSeverity::INFO;
    m_message = "";
}

void ErrorAdapter::set(HGErrorCode code, HGErrorSeverity severity, const std::string& message) {
    m_code = code;
    m_severity = severity;
    m_message = message;
}

std::string ErrorAdapter::toString() const {
    return "[" + errorSeverityToString(m_severity) + ":" + std::to_string(static_cast<int>(m_code)) + "] " + m_message;
}

std::string errorCodeToString(HGErrorCode code) {
    switch (code) {
        case HGErrorCode::SUCCESS:
            return "Success";
        case HGErrorCode::SYSTEM_INIT_FAILED:
            return "System initialization failed";
        case HGErrorCode::SYSTEM_RESOURCE_UNAVAILABLE:
            return "System resource unavailable";
        case HGErrorCode::SYSTEM_PERMISSION_DENIED:
            return "System permission denied";
        case HGErrorCode::SYSTEM_TIMEOUT:
            return "System timeout";
        case HGErrorCode::SYSTEM_IO_ERROR:
            return "System I/O error";
        case HGErrorCode::LOG_INIT_FAILED:
            return "Log initialization failed";
        case HGErrorCode::LOG_WRITE_FAILED:
            return "Log write failed";
        case HGErrorCode::LOG_FILE_NOT_FOUND:
            return "Log file not found";
        case HGErrorCode::LOG_INVALID_CONFIG:
            return "Invalid log configuration";
        case HGErrorCode::DATABASE_CONNECTION_FAILED:
            return "Database connection failed";
        case HGErrorCode::DATABASE_QUERY_FAILED:
            return "Database query failed";
        case HGErrorCode::DATABASE_TRANSACTION_FAILED:
            return "Database transaction failed";
        case HGErrorCode::DATABASE_CONSTRAINT_VIOLATION:
            return "Database constraint violation";
        case HGErrorCode::DATABASE_DATA_CORRUPTION:
            return "Database data corruption";
        case HGErrorCode::NETWORK_CONNECTION_FAILED:
            return "Network connection failed";
        case HGErrorCode::NETWORK_TIMEOUT:
            return "Network timeout";
        case HGErrorCode::NETWORK_PROTOCOL_ERROR:
            return "Network protocol error";
        case HGErrorCode::NETWORK_DATA_CORRUPTION:
            return "Network data corruption";
        case HGErrorCode::HARDWARE_DEVICE_NOT_FOUND:
            return "Hardware device not found";
        case HGErrorCode::HARDWARE_DEVICE_BUSY:
            return "Hardware device busy";
        case HGErrorCode::HARDWARE_DEVICE_ERROR:
            return "Hardware device error";
        case HGErrorCode::HARDWARE_DEVICE_OFFLINE:
            return "Hardware device offline";
        case HGErrorCode::IMAGE_INVALID_FORMAT:
            return "Invalid image format";
        case HGErrorCode::IMAGE_PROCESSING_FAILED:
            return "Image processing failed";
        case HGErrorCode::IMAGE_CAPTURE_FAILED:
            return "Image capture failed";
        case HGErrorCode::IMAGE_ANALYSIS_FAILED:
            return "Image analysis failed";
        case HGErrorCode::CAMERA_CONTROL_INVALID_CAMERA_TYPE:
            return "Invalid camera type";
        case HGErrorCode::CAMERA_CONTROL_INVALID_CAMERA_NAME:
            return "Invalid camera name";
        case HGErrorCode::CAMERA_CONTROL_CAMERA_NOT_FOUND:
            return "Camera not found";
        case HGErrorCode::CAMERA_CONTROL_CAMERA_OPEN_FAILED:
            return "Camera open failed";
        case HGErrorCode::CAMERA_CONTROL_CAMERA_CLOSE_FAILED:
            return "Camera close failed";
        case HGErrorCode::CAMERA_CONTROL_GET_FRAME_FAILED:
            return "Get frame failed";
        case HGErrorCode::CAMERA_RECOGNIZE_TEMPLATE_SAVE_FAILED:
            return "Template save failed";
        case HGErrorCode::CAMERA_RECOGNIZE_RECOGNITION_FAILED:
            return "Recognition failed";
        case HGErrorCode::HG_CUP_DET_DETECTION_FAILED:
            return "Cup detection failed";
        case HGErrorCode::HG_CUP_DET_INVALID_IMAGE:
            return "Invalid image";
        case HGErrorCode::HG_CUP_DET_INVALID_ROI:
            return "Invalid ROI";
        case HGErrorCode::HG_CUP_DET_TEMPLATE_SAVE_FAILED:
            return "Template save failed";
        case HGErrorCode::UI_INIT_FAILED:
            return "UI initialization failed";
        case HGErrorCode::UI_RESOURCE_NOT_FOUND:
            return "UI resource not found";
        case HGErrorCode::UI_LAYOUT_ERROR:
            return "UI layout error";
        case HGErrorCode::UI_EVENT_HANDLER_ERROR:
            return "UI event handler error";
        case HGErrorCode::BUSINESS_INVALID_OPERATION:
            return "Invalid business operation";
        case HGErrorCode::BUSINESS_INVALID_STATE:
            return "Invalid business state";
        case HGErrorCode::BUSINESS_DATA_VALIDATION_FAILED:
            return "Business data validation failed";
        case HGErrorCode::BUSINESS_RULE_VIOLATION:
            return "Business rule violation";
        case HGErrorCode::THIRD_PARTY_INIT_FAILED:
            return "Third party library initialization failed";
        case HGErrorCode::THIRD_PARTY_API_ERROR:
            return "Third party library API error";
        case HGErrorCode::THIRD_PARTY_LICENSE_ERROR:
            return "Third party library license error";
        case HGErrorCode::THIRD_PARTY_VERSION_INCOMPATIBLE:
            return "Third party library version incompatible";
        case HGErrorCode::UNKNOWN_ERROR:
        default:
            return "Unknown error";
    }
}

std::string errorSeverityToString(HGErrorSeverity severity) {
    switch (severity) {
        case HGErrorSeverity::INFO:
            return "INFO";
        case HGErrorSeverity::WARNING:
            return "WARNING";
        case HGErrorSeverity::ERROR:
            return "ERROR";
        case HGErrorSeverity::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

}
