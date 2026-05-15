#ifndef LOG4CPLUS_BACKEND_H
#define LOG4CPLUS_BACKEND_H

#include "log_backend_interface.h"
#include <string>

#if !defined(_MSC_VER) && !defined(WIN64) && !defined(_WIN64) && !defined(__WIN64__) && !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

#endif

namespace HGMACHINE {

class Log4cplusBackend : public LogBackendInterface {
public:
    Log4cplusBackend();
    ~Log4cplusBackend() override;

    void logInit(const std::string& logDir, LogOutput output) override;
    void logDeInit() override;

    void logInfo(const std::string& content) override;
    void logDebug(const std::string& content) override;
    void logError(const std::string& content) override;
    void logWarn(const std::string& content) override;

    void log(const std::string& content, LogLevel level) override;

private:
#if !defined(_MSC_VER) && !defined(WIN64) && !defined(_WIN64) && !defined(__WIN64__) && !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
    Logger m_logger;
#endif
    std::string m_logDir;
    LogOutput m_output;

    void initLogger(const std::string& logDir, LogOutput output);
};

}

#endif // LOG4CPLUS_BACKEND_H
