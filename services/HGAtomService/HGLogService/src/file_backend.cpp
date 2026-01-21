#include "file_backend.h"
#include "hgcommonutility.h"
#include <cstdio>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace HGMACHINE {

FileBackend::FileBackend() : m_initialized(false) {
}

FileBackend::~FileBackend() {
    closeLogFile();
}

void FileBackend::logInit(const std::string& logDir, LogOutput output) {
    if (m_logDir != logDir || m_output != output || !m_initialized) {
        m_logDir = logDir;
        m_output = output;
        openLogFile();
    }
}

void FileBackend::logDeInit() {
    closeLogFile();
}

void FileBackend::logInfo(const std::string& content) {
    log(content, LogLevel::INFO);
}

void FileBackend::logDebug(const std::string& content) {
    log(content, LogLevel::DEBUG);
}

void FileBackend::logError(const std::string& content) {
    log(content, LogLevel::LOG_ERROR);
}

void FileBackend::logWarn(const std::string& content) {
    log(content, LogLevel::WARN);
}

void FileBackend::log(const std::string& content, LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::string logMessage = getCurrentTime() + " [" + levelToString(level) + "] " + content + "\n";

    if (m_output == LogOutput::CONSOLE || m_output == LogOutput::ALL) {
        printf("%s", logMessage.c_str());
    }

    if ((m_output == LogOutput::FILE || m_output == LogOutput::ALL) && m_logFile.is_open()) {
        m_logFile << logMessage;
        m_logFile.flush();
    }
}

void FileBackend::openLogFile() {
    closeLogFile();

    int ret = HGMkDir(m_logDir);
    if (ret == 0) {
        std::ostringstream name;
        name << m_logDir << "mkdir success\n";
        printf(name.str().c_str());
    } else {
        std::ostringstream name;
        name << m_logDir << "mkdir failed\n";
        printf(name.str().c_str());
    }

    if (m_output == LogOutput::FILE || m_output == LogOutput::ALL) {
        m_logFilePath = m_logDir + "/" + HGGetTimeSlice() + ".log";
        m_logFile.open(m_logFilePath, std::ios::out | std::ios::app);
        if (m_logFile.is_open()) {
            m_initialized = true;
            printf("File backend opened log file: %s\n", m_logFilePath.c_str());
        } else {
            printf("File backend failed to open log file: %s\n", m_logFilePath.c_str());
        }
    } else {
        m_initialized = true;
        printf("File backend initialized (console only)\n");
    }
}

void FileBackend::closeLogFile() {
    if (m_logFile.is_open()) {
        m_logFile.close();
        printf("File backend closed log file\n");
    }
    m_initialized = false;
}

std::string FileBackend::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm tm;
    
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    return oss.str();
}

std::string FileBackend::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::LOG_ERROR:
            return "ERROR";
        case LogLevel::WARN:
            return "WARN";
        default:
            return "UNKNOWN";
    }
}

}
