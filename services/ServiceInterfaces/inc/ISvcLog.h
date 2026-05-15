#ifndef ISVC_LOG_H
#define ISVC_LOG_H

#include <string>

class ISvcLog {
public:
    virtual ~ISvcLog() = default;

    virtual void LogInit(const std::string& logDir = "./logs") = 0;
    virtual void LogDeInit() = 0;
    
    virtual void LogInfo(const std::string& content) = 0;
    virtual void LogDebug(const std::string& content) = 0;
    virtual void LogError(const std::string& content) = 0;
    virtual void LogWarning(const std::string& content) = 0;
    
    virtual void Log(const std::string& content, int level = 0) = 0;
    
    virtual std::string GetLogPath() = 0;
};

#endif // ISVC_LOG_H
