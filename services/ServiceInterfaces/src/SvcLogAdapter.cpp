#include "SvcLogAdapter.h"
#include "hglogservice.h"

class SvcLogAdapter::Impl {
public:
    HGMACHINE::HGLogService* logService;
    
    Impl() : logService(new HGMACHINE::HGLogService()) {}
    
    ~Impl() {
        if (logService) {
            delete logService;
            logService = nullptr;
        }
    }
};

SvcLogAdapter::SvcLogAdapter() : m_impl(new Impl()) {
}

SvcLogAdapter::~SvcLogAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void SvcLogAdapter::LogInit(const std::string& logDir) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logInit(logDir);
}

void SvcLogAdapter::LogDeInit() {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logDeInit();
}

void SvcLogAdapter::LogInfo(const std::string& content) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logInfo(content);
}

void SvcLogAdapter::LogWarning(const std::string& content) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logWarn(content);
}

void SvcLogAdapter::LogError(const std::string& content) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logError(content);
}

void SvcLogAdapter::LogDebug(const std::string& content) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->logDebug(content);
}

void SvcLogAdapter::Log(const std::string& content, int level) {
    if (!m_impl || !m_impl->logService) return;
    m_impl->logService->log(content, static_cast<HGMACHINE::LogLevel>(level));
}

std::string SvcLogAdapter::GetLogPath() {
    if (!m_impl || !m_impl->logService) return "";
    return HGMACHINE::HGLogService::getLogPath();
}
