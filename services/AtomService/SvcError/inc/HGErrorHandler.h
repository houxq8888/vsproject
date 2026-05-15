#ifndef HGERRORHANDLER_H
#define HGERRORHANDLER_H

#include "HGError.h"
#include <string>
#include <memory>

namespace HGMACHINE {

// 错误处理器接口类
class IErrorHandler {
public:
    virtual ~IErrorHandler() = default;
    
    // 处理异常
    virtual void handleException(const HGException& ex) = 0;
    
    // 处理错误信息
    virtual void handleErrorInfo(const ErrorInfo& info) = 0;
    
    // 设置错误回调
    virtual void setErrorCallback(std::function<void(const HGException&)> callback) = 0;
};

// 标准错误处理器实现
class StandardErrorHandler : public IErrorHandler {
public:
    StandardErrorHandler();
    ~StandardErrorHandler() override;
    
    // 处理异常
    void handleException(const HGException& ex) override;
    
    // 处理错误信息
    void handleErrorInfo(const ErrorInfo& info) override;
    
    // 设置错误回调
    void setErrorCallback(std::function<void(const HGException&)> callback) override;
    
    // 获取单例实例
    static StandardErrorHandler& getInstance();
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// 错误处理器基类，供其他模块继承
class ErrorHandlerBase {
public:
    ErrorHandlerBase();
    virtual ~ErrorHandlerBase() = default;
    
protected:
    // 处理异常
    void handleException(const HGException& ex);
    
    // 处理错误信息
    void handleErrorInfo(const ErrorInfo& info);
    
    // 抛出异常并处理
    void throwAndHandleException(ErrorCategory code, ErrorSeverity severity, const std::string& message,
                                const std::string& func = "", const std::string& file = "", int line = 0);
    
    // 设置错误信息并处理
    void setAndHandleErrorInfo(ErrorInfo& info, ErrorCategory code, ErrorSeverity severity, const std::string& message);
    
    // 检查错误信息并处理
    void checkAndHandleErrorInfo(const ErrorInfo& info);
    
    // 获取错误处理器
    IErrorHandler& getErrorHandler();
    
private:
    std::shared_ptr<IErrorHandler> m_errorHandler;
};

// 便捷宏，用于在类中处理异常
#define HG_HANDLE_EXCEPTION(code, severity, message) \
    throwAndHandleException(code, severity, message, __FUNCTION__, __FILE__, __LINE__)

#define HG_SET_AND_HANDLE_ERROR(info, code, severity, message) \
    setAndHandleErrorInfo(info, code, severity, message)

#define HG_CHECK_AND_HANDLE_ERROR(info) \
    checkAndHandleErrorInfo(info)

} // namespace HGMACHINE

#endif // HGERRORHANDLER_H