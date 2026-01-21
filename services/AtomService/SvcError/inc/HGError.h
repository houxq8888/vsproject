#ifndef HGERROR_H
#define HGERROR_H

#include <string>
#include <stdexcept>
#include <memory>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <functional>

namespace HGMACHINE {

// 错误严重级别
enum class ErrorSeverity {
    INFO,       // 信息
    WARNING,    // 警告
    ERROR,      // 错误
    CRITICAL    // 严重错误
};

// 错误类别
enum class ErrorCategory {
    SUCCESS = 0,
    
    // 基础系统错误 (1000-1999)
    SYSTEM_ERROR_BASE = 1000,
    SYSTEM_INIT_FAILED = 1001,
    SYSTEM_RESOURCE_UNAVAILABLE = 1002,
    SYSTEM_PERMISSION_DENIED = 1003,
    SYSTEM_TIMEOUT = 1004,
    SYSTEM_IO_ERROR = 1005,
    
    // 日志系统错误 (2000-2999)
    LOG_ERROR_BASE = 2000,
    LOG_INIT_FAILED = 2001,
    LOG_WRITE_FAILED = 2002,
    LOG_FILE_NOT_FOUND = 2003,
    LOG_INVALID_CONFIG = 2004,
    
    // 数据库错误 (3000-3999)
    DATABASE_ERROR_BASE = 3000,
    DATABASE_CONNECTION_FAILED = 3001,
    DATABASE_QUERY_FAILED = 3002,
    DATABASE_TRANSACTION_FAILED = 3003,
    DATABASE_CONSTRAINT_VIOLATION = 3004,
    DATABASE_DATA_CORRUPTION = 3005,
    
    // 网络通信错误 (4000-4999)
    NETWORK_ERROR_BASE = 4000,
    NETWORK_CONNECTION_FAILED = 4001,
    NETWORK_TIMEOUT = 4002,
    NETWORK_PROTOCOL_ERROR = 4003,
    NETWORK_DATA_CORRUPTION = 4004,
    
    // 硬件设备错误 (5000-5999)
    HARDWARE_ERROR_BASE = 5000,
    HARDWARE_DEVICE_NOT_FOUND = 5001,
    HARDWARE_DEVICE_BUSY = 5002,
    HARDWARE_DEVICE_ERROR = 5003,
    HARDWARE_DEVICE_OFFLINE = 5004,
    
    // 图像处理错误 (6000-6999)
    IMAGE_ERROR_BASE = 6000,
    IMAGE_INVALID_FORMAT = 6001,
    IMAGE_PROCESSING_FAILED = 6002,
    IMAGE_CAPTURE_FAILED = 6003,
    IMAGE_ANALYSIS_FAILED = 6004,
    
    // 用户界面错误 (7000-7999)
    UI_ERROR_BASE = 7000,
    UI_INIT_FAILED = 7001,
    UI_RESOURCE_NOT_FOUND = 7002,
    UI_LAYOUT_ERROR = 7003,
    UI_EVENT_HANDLER_ERROR = 7004,
    
    // 业务逻辑错误 (8000-8999)
    BUSINESS_ERROR_BASE = 8000,
    BUSINESS_INVALID_OPERATION = 8001,
    BUSINESS_INVALID_STATE = 8002,
    BUSINESS_DATA_VALIDATION_FAILED = 8003,
    BUSINESS_RULE_VIOLATION = 8004,
    
    // 第三方库错误 (9000-9999)
    THIRD_PARTY_ERROR_BASE = 9000,
    THIRD_PARTY_INIT_FAILED = 9001,
    THIRD_PARTY_API_ERROR = 9002,
    THIRD_PARTY_LICENSE_ERROR = 9003,
    THIRD_PARTY_VERSION_INCOMPATIBLE = 9004,
    
    UNKNOWN_ERROR = 9999
};

// 错误码转换函数
inline std::string errorCategoryToString(ErrorCategory code) {
    switch (code) {
        case ErrorCategory::SUCCESS:
            return "Success";
        case ErrorCategory::SYSTEM_INIT_FAILED:
            return "System initialization failed";
        case ErrorCategory::SYSTEM_RESOURCE_UNAVAILABLE:
            return "System resource unavailable";
        case ErrorCategory::SYSTEM_PERMISSION_DENIED:
            return "System permission denied";
        case ErrorCategory::SYSTEM_TIMEOUT:
            return "System timeout";
        case ErrorCategory::SYSTEM_IO_ERROR:
            return "System I/O error";
        case ErrorCategory::LOG_INIT_FAILED:
            return "Log initialization failed";
        case ErrorCategory::LOG_WRITE_FAILED:
            return "Log write failed";
        case ErrorCategory::LOG_FILE_NOT_FOUND:
            return "Log file not found";
        case ErrorCategory::LOG_INVALID_CONFIG:
            return "Invalid log configuration";
        case ErrorCategory::DATABASE_CONNECTION_FAILED:
            return "Database connection failed";
        case ErrorCategory::DATABASE_QUERY_FAILED:
            return "Database query failed";
        case ErrorCategory::DATABASE_TRANSACTION_FAILED:
            return "Database transaction failed";
        case ErrorCategory::DATABASE_CONSTRAINT_VIOLATION:
            return "Database constraint violation";
        case ErrorCategory::DATABASE_DATA_CORRUPTION:
            return "Database data corruption";
        case ErrorCategory::NETWORK_CONNECTION_FAILED:
            return "Network connection failed";
        case ErrorCategory::NETWORK_TIMEOUT:
            return "Network timeout";
        case ErrorCategory::NETWORK_PROTOCOL_ERROR:
            return "Network protocol error";
        case ErrorCategory::NETWORK_DATA_CORRUPTION:
            return "Network data corruption";
        case ErrorCategory::HARDWARE_DEVICE_NOT_FOUND:
            return "Hardware device not found";
        case ErrorCategory::HARDWARE_DEVICE_BUSY:
            return "Hardware device busy";
        case ErrorCategory::HARDWARE_DEVICE_ERROR:
            return "Hardware device error";
        case ErrorCategory::HARDWARE_DEVICE_OFFLINE:
            return "Hardware device offline";
        case ErrorCategory::IMAGE_INVALID_FORMAT:
            return "Invalid image format";
        case ErrorCategory::IMAGE_PROCESSING_FAILED:
            return "Image processing failed";
        case ErrorCategory::IMAGE_CAPTURE_FAILED:
            return "Image capture failed";
        case ErrorCategory::IMAGE_ANALYSIS_FAILED:
            return "Image analysis failed";
        case ErrorCategory::UI_INIT_FAILED:
            return "UI initialization failed";
        case ErrorCategory::UI_RESOURCE_NOT_FOUND:
            return "UI resource not found";
        case ErrorCategory::UI_LAYOUT_ERROR:
            return "UI layout error";
        case ErrorCategory::UI_EVENT_HANDLER_ERROR:
            return "UI event handler error";
        case ErrorCategory::BUSINESS_INVALID_OPERATION:
            return "Invalid business operation";
        case ErrorCategory::BUSINESS_INVALID_STATE:
            return "Invalid business state";
        case ErrorCategory::BUSINESS_DATA_VALIDATION_FAILED:
            return "Business data validation failed";
        case ErrorCategory::BUSINESS_RULE_VIOLATION:
            return "Business rule violation";
        case ErrorCategory::THIRD_PARTY_INIT_FAILED:
            return "Third party library initialization failed";
        case ErrorCategory::THIRD_PARTY_API_ERROR:
            return "Third party library API error";
        case ErrorCategory::THIRD_PARTY_LICENSE_ERROR:
            return "Third party library license error";
        case ErrorCategory::THIRD_PARTY_VERSION_INCOMPATIBLE:
            return "Third party library version incompatible";
        case ErrorCategory::UNKNOWN_ERROR:
        default:
            return "Unknown error";
    }
}

inline std::string errorSeverityToString(ErrorSeverity severity) {
    switch (severity) {
        case ErrorSeverity::INFO:
            return "INFO";
        case ErrorSeverity::WARNING:
            return "WARNING";
        case ErrorSeverity::ERROR:
            return "ERROR";
        case ErrorSeverity::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

// 错误追踪信息
struct ErrorTrace {
    std::string functionName;
    std::string fileName;
    int lineNumber;
    std::chrono::system_clock::time_point timestamp;
    
    ErrorTrace(const std::string& func, const std::string& file, int line)
        : functionName(func), fileName(file), lineNumber(line), 
          timestamp(std::chrono::system_clock::now()) {}
          
    std::string toString() const {
        auto time_t = std::chrono::system_clock::to_time_t(timestamp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamp.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
        ss << functionName << " (" << fileName << ":" << lineNumber << ")";
        return ss.str();
    }
};

// 统一异常类
class HGException : public std::runtime_error {
public:
    HGException(ErrorCategory code, ErrorSeverity severity, const std::string& message,
               const std::string& func = "", const std::string& file = "", int line = 0)
        : std::runtime_error(message), 
          m_code(code), 
          m_severity(severity), 
          m_message(message),
          m_errorId(generateErrorId()) {
        
        if (!func.empty()) {
            m_traceStack.emplace_back(func, file, line);
        }
    }
    
    ErrorCategory code() const { return m_code; }
    ErrorSeverity severity() const { return m_severity; }
    const std::string& message() const { return m_message; }
    const std::string& errorId() const { return m_errorId; }
    const std::vector<ErrorTrace>& traceStack() const { return m_traceStack; }
    
    void addTrace(const std::string& func, const std::string& file, int line) {
        m_traceStack.emplace_back(func, file, line);
    }
    
    std::string fullMessage() const {
        std::stringstream ss;
        ss << "Error ID: " << m_errorId << "\n";
        ss << "Category: " << errorCategoryToString(m_code) << " (" << static_cast<int>(m_code) << ")\n";
        ss << "Severity: " << errorSeverityToString(m_severity) << "\n";
        ss << "Message: " << m_message << "\n";
        
        if (!m_traceStack.empty()) {
            ss << "Trace Stack:\n";
            for (const auto& trace : m_traceStack) {
                ss << "  " << trace.toString() << "\n";
            }
        }
        
        return ss.str();
    }
    
private:
    ErrorCategory m_code;
    ErrorSeverity m_severity;
    std::string m_message;
    std::string m_errorId;
    std::vector<ErrorTrace> m_traceStack;
    
    static std::string generateErrorId() {
        static int counter = 0;
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << "ERR-" << std::put_time(std::localtime(&time_t), "%Y%m%d%H%M%S");
        ss << "-" << std::setfill('0') << std::setw(3) << ms.count();
        ss << "-" << std::setfill('0') << std::setw(4) << ++counter;
        return ss.str();
    }
};

// 错误信息类（非异常情况下的错误传递）
class ErrorInfo {
public:
    ErrorInfo() : m_code(ErrorCategory::SUCCESS), m_severity(ErrorSeverity::INFO), m_message("") {}
    
    ErrorInfo(ErrorCategory code, ErrorSeverity severity, const std::string& message)
        : m_code(code), m_severity(severity), m_message(message) {}
    
    ErrorCategory code() const { return m_code; }
    ErrorSeverity severity() const { return m_severity; }
    const std::string& message() const { return m_message; }
    bool hasError() const { return m_code != ErrorCategory::SUCCESS; }
    bool isCritical() const { return m_severity == ErrorSeverity::CRITICAL; }
    
    void clear() {
        m_code = ErrorCategory::SUCCESS;
        m_severity = ErrorSeverity::INFO;
        m_message.clear();
    }
    
    void set(ErrorCategory code, ErrorSeverity severity, const std::string& message) {
        m_code = code;
        m_severity = severity;
        m_message = message;
    }
    
    std::string toString() const {
        std::stringstream ss;
        ss << "[" << errorSeverityToString(m_severity) << "] ";
        ss << errorCategoryToString(m_code) << " (" << static_cast<int>(m_code) << "): ";
        ss << m_message;
        return ss.str();
    }
    
private:
    ErrorCategory m_code;
    ErrorSeverity m_severity;
    std::string m_message;
};

// 便捷宏定义
#define HG_THROW_ERROR(code, message) \
    throw HGMACHINE::HGException(code, HGMACHINE::ErrorSeverity::ERROR, message, __FUNCTION__, __FILE__, __LINE__)

#define HG_THROW_CRITICAL(code, message) \
    throw HGMACHINE::HGException(code, HGMACHINE::ErrorSeverity::CRITICAL, message, __FUNCTION__, __FILE__, __LINE__)

#define HG_THROW_WARNING(code, message) \
    throw HGMACHINE::HGException(code, HGMACHINE::ErrorSeverity::WARNING, message, __FUNCTION__, __FILE__, __LINE__)

#define HG_SET_ERROR(info, code, severity, message) \
    info.set(code, severity, message)

#define HG_CHECK_ERROR(info) \
    if (info.hasError()) { \
        throw HGMACHINE::HGException(info.code(), info.severity(), info.message(), __FUNCTION__, __FILE__, __LINE__); \
    }

} // namespace HGMACHINE

#endif // HGERROR_H