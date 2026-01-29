#include "SvcErrorAdapter.h"
#include "HGError.h"
#include <sstream>

namespace HGMACHINE {

SvcErrorAdapter::SvcErrorAdapter() 
    : m_code(HGErrorCode::SUCCESS)
    , m_severity(HGErrorSeverity::INFO)
    , m_message("")
{
}

SvcErrorAdapter::SvcErrorAdapter(HGErrorCode code, HGErrorSeverity severity, const std::string& message)
    : m_code(code)
    , m_severity(severity)
    , m_message(message)
{
}

HGErrorCode SvcErrorAdapter::code() const {
    return m_code;
}

HGErrorSeverity SvcErrorAdapter::severity() const {
    return m_severity;
}

const std::string& SvcErrorAdapter::message() const {
    return m_message;
}

bool SvcErrorAdapter::hasError() const {
    return m_code != HGErrorCode::SUCCESS;
}

bool SvcErrorAdapter::isCritical() const {
    return m_severity == HGErrorSeverity::CRITICAL;
}

void SvcErrorAdapter::clear() {
    m_code = HGErrorCode::SUCCESS;
    m_severity = HGErrorSeverity::INFO;
    m_message = "";
}

void SvcErrorAdapter::set(HGErrorCode code, HGErrorSeverity severity, const std::string& message) {
    m_code = code;
    m_severity = severity;
    m_message = message;
}

std::string SvcErrorAdapter::toString() const {
    return "[" + errorSeverityToString(m_severity) + ":" + std::to_string(static_cast<int>(m_code)) + "] " + m_message;
}

std::string errorCodeToString(HGErrorCode code) {
    return errorCategoryToString(static_cast<ErrorCategory>(code));
}

std::string errorSeverityToString(HGErrorSeverity severity) {
    return HGMACHINE::errorSeverityToString(static_cast<ErrorSeverity>(severity));
}

}
