#include "log4cplus_backend.h"
#include "hgcommonutility.h"
#include <sstream>
#include <cstdio>

namespace HGMACHINE {

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

Log4cplusBackend::Log4cplusBackend() {
}

Log4cplusBackend::~Log4cplusBackend() {
}

void Log4cplusBackend::logInit(const std::string& logDir, LogOutput output) {
    m_logDir = logDir;
    m_output = output;
}

void Log4cplusBackend::logDeInit() {
}

void Log4cplusBackend::logInfo(const std::string& content) {
    printf("[INFO] %s\n", content.c_str());
}

void Log4cplusBackend::logDebug(const std::string& content) {
    printf("[DEBUG] %s\n", content.c_str());
}

void Log4cplusBackend::logError(const std::string& content) {
    printf("[ERROR] %s\n", content.c_str());
}

void Log4cplusBackend::logWarn(const std::string& content) {
    printf("[WARN] %s\n", content.c_str());
}

void Log4cplusBackend::log(const std::string& content, LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            logInfo(content);
            break;
        case LogLevel::DEBUG:
            logDebug(content);
            break;
        case LogLevel::LOG_ERROR:
            logError(content);
            break;
        case LogLevel::WARN:
            logWarn(content);
            break;
    }
}

#else

Log4cplusBackend::Log4cplusBackend() {
}

Log4cplusBackend::~Log4cplusBackend() {
    printf("release log4cplus backend\n");
}

void Log4cplusBackend::logInit(const std::string& logDir, LogOutput output) {
    if (m_logDir != logDir || m_output != output) {
        m_logDir = logDir;
        m_output = output;
        initLogger(logDir, output);
    }
}

void Log4cplusBackend::logDeInit() {
}

void Log4cplusBackend::logInfo(const std::string& content) {
    LOG4CPLUS_INFO(m_logger, content.c_str());
}

void Log4cplusBackend::logDebug(const std::string& content) {
    LOG4CPLUS_DEBUG(m_logger, content.c_str());
}

void Log4cplusBackend::logError(const std::string& content) {
    LOG4CPLUS_ERROR(m_logger, content.c_str());
}

void Log4cplusBackend::logWarn(const std::string& content) {
    LOG4CPLUS_WARN(m_logger, content.c_str());
}

void Log4cplusBackend::log(const std::string& content, LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            logInfo(content);
            break;
        case LogLevel::DEBUG:
            logDebug(content);
            break;
        case LogLevel::LOG_ERROR:
            logError(content);
            break;
        case LogLevel::WARN:
            logWarn(content);
            break;
    }
}

void Log4cplusBackend::initLogger(const std::string& logDir, LogOutput output) {
    int ret = HGMkDir(logDir);
    if (ret == 0) {
        std::ostringstream name;
        name << logDir << "mkdir success\n";
        printf(name.str().c_str());
    } else {
        std::ostringstream name;
        name << logDir << "mkdir failed\n";
        printf(name.str().c_str());
    }

    std::ostringstream logpath;
    logpath << logDir << "/" << HGGetTimeSlice() << ".log";

    SharedAppenderPtr appendPtr(new DailyRollingFileAppender(LOG4CPLUS_STRING_TO_TSTRING(logpath.str()), HOURLY, true));
    appendPtr->setName(LOG4CPLUS_TEXT("filelogtest"));

    std::string pattern = "%D{%Y/%m/%d %X-%Q}->%m%n";
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::auto_ptr<Layout> layout(new PatternLayout(LOG4CPLUS_STRING_TO_TSTRING(pattern)));
    appendPtr->setLayout(layout);
#else
    appendPtr->setLayout(std::unique_ptr<Layout>(new PatternLayout(LOG4CPLUS_STRING_TO_TSTRING(pattern))));
#endif

    m_logger = Logger::getInstance(LOG4CPLUS_TEXT("test.subtestof_filelog"));
    m_logger.addAppender(appendPtr);
    m_logger.setLogLevel(ALL_LOG_LEVEL);

    if (output == LogOutput::CONSOLE || output == LogOutput::ALL) {
        SharedAppenderPtr consoleAppender(new ConsoleAppender());
        consoleAppender->setName(LOG4CPLUS_TEXT("consolelogtest"));
        consoleAppender->setLayout(std::unique_ptr<Layout>(new PatternLayout(LOG4CPLUS_STRING_TO_TSTRING(pattern))));
        m_logger.addAppender(consoleAppender);
    }
}

#endif

}
