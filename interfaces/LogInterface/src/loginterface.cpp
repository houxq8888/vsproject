#include "loginterface.h"
#include "logmodule.h"

namespace HGMACHINE {

class LogInterface::Impl {
public:
    std::unique_ptr<LogModule> m_logModule;

    bool initialize() {
        m_logModule = std::make_unique<LogModule>();
        return m_logModule->initialize("");
    }

    void shutdown() {
        if (m_logModule) {
            m_logModule->shutdown();
        }
    }

    void logInfo(const std::string& content) {
        if (m_logModule) {
            m_logModule->logInfo(content);
        }
    }

    void logDebug(const std::string& content) {
        if (m_logModule) {
            m_logModule->logDebug(content);
        }
    }

    void logError(const std::string& content) {
        if (m_logModule) {
            m_logModule->logError(content);
        }
    }

    void logWarn(const std::string& content) {
        if (m_logModule) {
            m_logModule->logWarn(content);
        }
    }

    void log(const std::string& content, int level) {
        if (m_logModule) {
            m_logModule->log(content, level);
        }
    }

    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName) {
        if (m_logModule) {
            return m_logModule->getAuditTrailLogs(tableName);
        }
        return std::vector<std::map<std::string, std::string>>();
    }

    std::vector<std::map<std::string, std::string>> getRunLogs(
        const std::string& logPath) {
        if (m_logModule) {
            return m_logModule->getRunLogs(logPath);
        }
        return std::vector<std::map<std::string, std::string>>();
    }

    std::vector<std::map<std::string, std::string>> searchLogs(
        const std::string& keyword,
        const std::string& timeFrom,
        const std::string& timeTo,
        int page,
        int pageSize,
        int* totalCount) {
        if (m_logModule) {
            return m_logModule->searchLogs(keyword, timeFrom, timeTo, page, pageSize, totalCount);
        }
        return std::vector<std::map<std::string, std::string>>();
    }

    int getLogCount(const std::string& tableName) {
        if (m_logModule) {
            return m_logModule->getLogCount(tableName);
        }
        return 0;
    }

    int getSearchLogCount(const std::string& keyword,
                          const std::string& timeFrom,
                          const std::string& timeTo) {
        if (m_logModule) {
            return m_logModule->getSearchLogCount(keyword, timeFrom, timeTo);
        }
        return 0;
    }

    std::vector<std::string> getLogTableNames() {
        if (m_logModule) {
            return m_logModule->getLogTableNames();
        }
        return std::vector<std::string>();
    }

    bool saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                       const std::string& filePath,
                       const std::string& format) {
        if (m_logModule) {
            return m_logModule->saveLogsToFile(logs, filePath, format);
        }
        return false;
    }
};

LogInterface::LogInterface() : m_impl(new Impl()) {
}

LogInterface::~LogInterface() {
    delete m_impl;
}

bool LogInterface::initialize() {
    return m_impl->initialize();
}

void LogInterface::shutdown() {
    m_impl->shutdown();
}

void LogInterface::logInfo(const std::string& content) {
    m_impl->logInfo(content);
}

void LogInterface::logDebug(const std::string& content) {
    m_impl->logDebug(content);
}

void LogInterface::logError(const std::string& content) {
    m_impl->logError(content);
}

void LogInterface::logWarn(const std::string& content) {
    m_impl->logWarn(content);
}

void LogInterface::log(const std::string& content, int level) {
    m_impl->log(content, level);
}

std::vector<std::map<std::string, std::string>> LogInterface::getAuditTrailLogs(
    const std::string& tableName) {
    return m_impl->getAuditTrailLogs(tableName);
}

std::vector<std::map<std::string, std::string>> LogInterface::getRunLogs(
    const std::string& logPath) {
    return m_impl->getRunLogs(logPath);
}

std::vector<std::map<std::string, std::string>> LogInterface::searchLogs(
    const std::string& keyword,
    const std::string& timeFrom,
    const std::string& timeTo,
    int page,
    int pageSize,
    int* totalCount) {
    return m_impl->searchLogs(keyword, timeFrom, timeTo, page, pageSize, totalCount);
}

int LogInterface::getLogCount(const std::string& tableName) {
    return m_impl->getLogCount(tableName);
}

int LogInterface::getSearchLogCount(const std::string& keyword,
                                   const std::string& timeFrom,
                                   const std::string& timeTo) {
    return m_impl->getSearchLogCount(keyword, timeFrom, timeTo);
}

std::vector<std::string> LogInterface::getLogTableNames() {
    return m_impl->getLogTableNames();
}

bool LogInterface::saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                                   const std::string& filePath,
                                   const std::string& format) {
    return m_impl->saveLogsToFile(logs, filePath, format);
}

}
