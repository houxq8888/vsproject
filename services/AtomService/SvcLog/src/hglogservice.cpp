#include "hglogservice.h"
#include "log4cplus_backend.h"
#include "zlog_backend.h"
#include "file_backend.h"
#include "hgxml.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

namespace HGMACHINE {

#define LOG_PATH_RELATIVE "logs"

std::string getFullLogPath(const std::string& relativePath) {
    std::string programPath = getAppDir();
    
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    char fullPath[MAX_PATH];
    PathCombineA(fullPath, programPath.c_str(), relativePath.c_str());
    return std::string(fullPath);
#else
    return programPath + "/" + relativePath;
#endif
}

std::string HGLogService::getLogPath() {
    return getFullLogPath(LOG_PATH_RELATIVE);
}

HGLogService::HGLogService() : m_currentBackend(LogBackendType::DEFAULT), m_output(LogOutput::FILE) {
}

HGLogService::~HGLogService() {
    logDeInit();
}

void HGLogService::logInit(const std::string& logDir, LogOutput output, LogBackendType backendType) {
    m_logDir = logDir;
    m_output = output;

    std::string actualLogDir = logDir;
    if (logDir.empty() || logDir == "./logs") {
        actualLogDir = getFullLogPath(LOG_PATH_RELATIVE);
    }

    if (backendType == LogBackendType::DEFAULT) {
        backendType = LogBackendType::LOG4CPLUS;
    }

    if (m_currentBackend != backendType || !m_backend) {
        setBackend(backendType);
    }

    if (m_backend) {
        m_backend->logInit(actualLogDir, output);
    }
}

void HGLogService::logDeInit() {
    if (m_backend) {
        m_backend->logDeInit();
    }
}

void HGLogService::logInfo(const std::string& content) {
    if (m_backend) {
        m_backend->logInfo(content);
    }
}

void HGLogService::logDebug(const std::string& content) {
    if (m_backend) {
        m_backend->logDebug(content);
    }
}

void HGLogService::logError(const std::string& content) {
    if (m_backend) {
        m_backend->logError(content);
    }
}

void HGLogService::logWarn(const std::string& content) {
    if (m_backend) {
        m_backend->logWarn(content);
    }
}

void HGLogService::log(const std::string& content, LogLevel level) {
    if (m_backend) {
        m_backend->log(content, level);
    }
}

HGLogService* HGLogService::getInstance(const std::string& logDir, LogOutput output, LogBackendType backendType) {
    static HGLogService instance;
    
    std::string actualLogDir = logDir;
    if (logDir.empty() || logDir == "./logs") {
        actualLogDir = getFullLogPath(LOG_PATH_RELATIVE);
    }
    
    instance.logInit(actualLogDir, output, backendType);
    return &instance;
}

void HGLogService::setBackend(LogBackendType backendType) {
    if (backendType == LogBackendType::DEFAULT) {
        backendType = LogBackendType::LOG4CPLUS;
    }

    if (m_currentBackend == backendType && m_backend) {
        return;
    }

    logDeInit();
    createBackend(backendType);
    m_currentBackend = backendType;

    if (m_backend) {
        std::string actualLogDir = m_logDir;
        if (m_logDir.empty() || m_logDir == "./logs") {
            actualLogDir = getFullLogPath(LOG_PATH_RELATIVE);
        }
        m_backend->logInit(actualLogDir, m_output);
    }
}

LogBackendType HGLogService::getCurrentBackend() const {
    return m_currentBackend;
}

void HGLogService::createBackend(LogBackendType backendType) {
    switch (backendType) {
        case LogBackendType::LOG4CPLUS:
            m_backend = std::make_unique<Log4cplusBackend>();
            break;
        case LogBackendType::ZLOG:
            m_backend = std::make_unique<ZlogBackend>();
            break;
        case LogBackendType::FILE:
            m_backend = std::make_unique<FileBackend>();
            break;
        case LogBackendType::DEFAULT:
            m_backend = std::make_unique<Log4cplusBackend>();
            break;
        default:
            // Use file backend as fallback for unknown backend types
            m_backend = std::make_unique<FileBackend>();
            printf("Warning: Unknown log backend type, using FileBackend as fallback\n");
            break;
    }
}

}
