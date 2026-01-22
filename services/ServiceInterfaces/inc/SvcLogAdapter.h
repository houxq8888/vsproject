#ifndef SVC_LOG_ADAPTER_H
#define SVC_LOG_ADAPTER_H

#include "ISvcLog.h"
#include "ServiceInterfaces_global.h"
#include <string>
#include <memory>

class SERVICEINTERFACES_EXPORT SvcLogAdapter : public ISvcLog {
private:
    class Impl;
    Impl* m_impl;
    
public:
    SvcLogAdapter();
    virtual ~SvcLogAdapter();
    
    void LogInit(const std::string& logDir = "./logs") override;
    void LogDeInit() override;
    
    void LogInfo(const std::string& content) override;
    void LogDebug(const std::string& content) override;
    void LogError(const std::string& content) override;
    void LogWarning(const std::string& content) override;
    
    void Log(const std::string& content, int level = 0) override;
    
    std::string GetLogPath() override;
};

#endif // SVC_LOG_ADAPTER_H
