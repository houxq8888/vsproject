#ifndef HGLOGSERVICE_H
#define HGLOGSERVICE_H

#include "HGLogService_global.h"
#include "log_backend_interface.h"
#include <string>
#include <memory>

namespace HGMACHINE {

enum class LogBackendType {
    LOG4CPLUS,
    ZLOG,
    FILE,
    DEFAULT
};

class HGLOGSERVICE_EXPORT HGLogService
{
public:
    HGLogService();
    ~HGLogService();

    void logInit(const std::string& logDir = "./logs", LogOutput output = LogOutput::FILE, LogBackendType backendType = LogBackendType::DEFAULT);
    void logDeInit();

    void logInfo(const std::string& content);
    void logDebug(const std::string& content);
    void logError(const std::string& content);
    void logWarn(const std::string& content);

    void log(const std::string& content, LogLevel level = LogLevel::INFO);

    static HGLogService* getInstance(const std::string& logDir = "./logs", LogOutput output = LogOutput::FILE, LogBackendType backendType = LogBackendType::DEFAULT);

    static std::string getLogPath();

    void setBackend(LogBackendType backendType);
    LogBackendType getCurrentBackend() const;

private:
    std::unique_ptr<LogBackendInterface> m_backend;
    LogBackendType m_currentBackend;
    std::string m_logDir;
    LogOutput m_output;

    void createBackend(LogBackendType backendType);
};

}

#endif // HGLOGSERVICE_H
