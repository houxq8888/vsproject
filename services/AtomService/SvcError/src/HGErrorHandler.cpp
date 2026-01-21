#include "HGErrorHandler.h"
#include "HGErrorTracker.h"
#include <mutex>
#include <fstream>
#include <iostream>

namespace HGMACHINE {

// StandardErrorHandler的内部实现类
class StandardErrorHandler::Impl {
public:
    Impl() = default;
    ~Impl() = default;
    
    // 处理异常
    void handleException(const HGException& ex) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // 追踪异常
        HGMACHINE::trackException(ex);
        
        // 尝试写入错误日志文件
        try {
            std::ofstream errorFile("error.log", std::ios::app);
            if (errorFile.is_open()) {
                errorFile << ex.fullMessage() << std::endl;
                errorFile.close();
            }
        } catch (...) {
            // 如果连文件写入都失败，输出到标准错误
            std::cerr << "Failed to write to error log file" << std::endl;
            std::cerr << ex.fullMessage() << std::endl;
        }
        
        // 如果是严重错误，额外记录到专门的严重错误文件
        if (ex.severity() == ErrorSeverity::CRITICAL) {
            try {
                std::ofstream criticalFile("critical_error.log", std::ios::app);
                if (criticalFile.is_open()) {
                    criticalFile << ex.fullMessage() << std::endl;
                    criticalFile.close();
                }
            } catch (...) {
                std::cerr << "Failed to write to critical error log file" << std::endl;
            }
        }
        
        // 调用错误回调函数
        if (m_errorCallback) {
            try {
                m_errorCallback(ex);
            } catch (...) {
                std::cerr << "Error in error callback function" << std::endl;
            }
        }
        
        // 如果是严重错误，可以考虑终止程序
        if (ex.severity() == ErrorSeverity::CRITICAL) {
            std::cerr << "Critical error occurred, application may need to terminate" << std::endl;
            // 在实际应用中，可以根据需要决定是否终止程序
            // std::terminate();
        }
    }
    
    // 处理错误信息
    void handleErrorInfo(const ErrorInfo& info) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (!info.hasError()) {
            return; // 没有错误则不记录
        }
        
        // 追踪错误信息
        HGMACHINE::trackErrorInfo(info);
        
        // 尝试写入错误日志文件
        try {
            std::ofstream errorFile("error.log", std::ios::app);
            if (errorFile.is_open()) {
                errorFile << info.toString() << std::endl;
                errorFile.close();
            }
        } catch (...) {
            std::cerr << "Failed to write to error log file" << std::endl;
            std::cerr << info.toString() << std::endl;
        }
        
        // 如果是严重错误，可以转换为异常处理
        if (info.isCritical()) {
            HGException ex(info.code(), info.severity(), info.message());
            handleException(ex);
        }
    }
    
    // 设置错误回调
    void setErrorCallback(std::function<void(const HGException&)> callback) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_errorCallback = callback;
    }
    
private:
    std::mutex m_mutex;
    std::function<void(const HGException&)> m_errorCallback;
};

// StandardErrorHandler实现
StandardErrorHandler::StandardErrorHandler() : pImpl(std::make_unique<Impl>()) {
}

StandardErrorHandler::~StandardErrorHandler() = default;

void StandardErrorHandler::handleException(const HGException& ex) {
    pImpl->handleException(ex);
}

void StandardErrorHandler::handleErrorInfo(const ErrorInfo& info) {
    pImpl->handleErrorInfo(info);
}

void StandardErrorHandler::setErrorCallback(std::function<void(const HGException&)> callback) {
    pImpl->setErrorCallback(callback);
}

StandardErrorHandler& StandardErrorHandler::getInstance() {
    static StandardErrorHandler instance;
    return instance;
}

// ErrorHandlerBase实现
ErrorHandlerBase::ErrorHandlerBase() 
    : m_errorHandler(&StandardErrorHandler::getInstance()) {
}

void ErrorHandlerBase::handleException(const HGException& ex) {
    m_errorHandler->handleException(ex);
}

void ErrorHandlerBase::handleErrorInfo(const ErrorInfo& info) {
    m_errorHandler->handleErrorInfo(info);
}

void ErrorHandlerBase::throwAndHandleException(ErrorCategory code, ErrorSeverity severity, const std::string& message,
                                              const std::string& func, const std::string& file, int line) {
    HGException ex(code, severity, message, func, file, line);
    handleException(ex);
    throw ex; // 重新抛出异常
}

void ErrorHandlerBase::setAndHandleErrorInfo(ErrorInfo& info, ErrorCategory code, ErrorSeverity severity, const std::string& message) {
    info.set(code, severity, message);
    handleErrorInfo(info);
}

void ErrorHandlerBase::checkAndHandleErrorInfo(const ErrorInfo& info) {
    if (info.hasError()) {
        handleErrorInfo(info);
    }
}

IErrorHandler& ErrorHandlerBase::getErrorHandler() {
    return *m_errorHandler;
}

} // namespace HGMACHINE