#include "HGError.h"
#include <mutex>
#include <fstream>
#include <iostream>

namespace HGMACHINE {

// 错误处理管理器
class ErrorHandler {
public:
    static ErrorHandler& getInstance() {
        static ErrorHandler instance;
        return instance;
    }
    
    // 记录异常到日志
    void logException(const HGException& ex) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
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
    }
    
    // 记录错误信息到日志
    void logErrorInfo(const ErrorInfo& info) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (!info.hasError()) {
            return; // 没有错误则不记录
        }
        
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
    }
    
    // 设置错误处理回调函数
    void setErrorCallback(std::function<void(const HGException&)> callback) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_errorCallback = callback;
    }
    
    // 调用错误处理回调函数
    void callErrorCallback(const HGException& ex) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_errorCallback) {
            try {
                m_errorCallback(ex);
            } catch (...) {
                std::cerr << "Error in error callback function" << std::endl;
            }
        }
    }
    
private:
    ErrorHandler() = default;
    ~ErrorHandler() = default;
    
    std::mutex m_mutex;
    std::function<void(const HGException&)> m_errorCallback;
};

// 全局异常处理函数
void handleException(const HGException& ex) {
    auto& handler = ErrorHandler::getInstance();
    
    // 记录异常到日志
    handler.logException(ex);
    
    // 调用错误回调函数
    handler.callErrorCallback(ex);
    
    // 如果是严重错误，可以考虑终止程序
    if (ex.severity() == ErrorSeverity::CRITICAL) {
        std::cerr << "Critical error occurred, application may need to terminate" << std::endl;
        // 在实际应用中，可以根据需要决定是否终止程序
        // std::terminate();
    }
}

// 全局错误信息处理函数
void handleErrorInfo(const ErrorInfo& info) {
    auto& handler = ErrorHandler::getInstance();
    
    // 记录错误信息到日志
    handler.logErrorInfo(info);
    
    // 如果是严重错误，可以转换为异常处理
    if (info.isCritical()) {
        HGException ex(info.code(), info.severity(), info.message());
        handleException(ex);
    }
}

// 设置错误处理回调函数
void setErrorCallback(std::function<void(const HGException&)> callback) {
    ErrorHandler::getInstance().setErrorCallback(callback);
}

} // namespace HGMACHINE