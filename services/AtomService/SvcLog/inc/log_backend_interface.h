#ifndef LOG_BACKEND_INTERFACE_H
#define LOG_BACKEND_INTERFACE_H

#include <string>

namespace HGMACHINE {

enum class LogLevel {
    INFO,
    DEBUG,
    LOG_ERROR,
    WARN
};

enum class LogOutput {
    FILE,
    CONSOLE,
    DATABASE,
    ALL
};

class LogBackendInterface {
public:
    virtual ~LogBackendInterface() = default;

    virtual void logInit(const std::string& logDir, LogOutput output) = 0;
    virtual void logDeInit() = 0;

    virtual void logInfo(const std::string& content) = 0;
    virtual void logDebug(const std::string& content) = 0;
    virtual void logError(const std::string& content) = 0;
    virtual void logWarn(const std::string& content) = 0;

    virtual void log(const std::string& content, LogLevel level) = 0;
};

}

#endif // LOG_BACKEND_INTERFACE_H
