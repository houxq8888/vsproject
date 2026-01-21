#include "logmodule.h"
#include "hglogservice.h"
#include "rwDb.h"
#include "hgcsv.h"
#include "hgtxt.h"
#include "hgpdf.h"
#include <fstream>
#include <sstream>

namespace HGMACHINE {

class LogModule::Impl {
public:
    std::unique_ptr<HGMACHINE::HGLogService> m_logService;
    Impl() 
        : m_logService(std::make_unique<HGMACHINE::HGLogService>())
    {
    }

    bool initialize(const std::string& dbPath) {
        m_logService->logInit();
        if (!dbPath.empty()) {
            RWDb::openDB(dbPath);
        }
        return true;
    }

    void shutdown() {
        m_logService->logDeInit();
        RWDb::closeDB();
    }

    void logInfo(const std::string& content) {
        m_logService->logInfo(content);
    }

    void logDebug(const std::string& content) {
        m_logService->logDebug(content);
    }

    void logError(const std::string& content) {
        m_logService->logError(content);
    }

    void logWarn(const std::string& content) {
        m_logService->logWarn(content);
    }

    void log(const std::string& content, int level) {
        m_logService->log(content, static_cast<HGMACHINE::LogLevel>(level));
    }

    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName) {
        return RWDb::readAuditTrailLog(tableName);
    }

    std::vector<std::map<std::string, std::string>> getRunLogs(
        const std::string& logPath) {
        std::vector<std::map<std::string, std::string>> logs;
        std::ifstream file(logPath);
        if (!file.is_open()) {
            return logs;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::map<std::string, std::string> logEntry;
            logEntry["content"] = line;
            logs.push_back(logEntry);
        }

        file.close();
        return logs;
    }

    // 辅助函数：尝试从不同格式的时间字符串解析时间
    HGExactTime parseTimeString(const std::string& timeStr) {
        HGExactTime timeObj;
        
        // 尝试使用标准格式解析
        if (timeObj.fromString(timeStr)) {
            return timeObj;
        }
        
        // 尝试其他可能的格式
        // 格式1: "YYYY-MM-DD HH:MM:SS" (标准字符串格式)
        if (timeStr.length() == 19) {
            std::tm tm = {};
            if (sscanf(timeStr.c_str(), "%d-%d-%d %d:%d:%d", 
                      &tm.tm_year, &tm.tm_mon, &tm.tm_mday, 
                      &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
                tm.tm_year -= 1900;
                tm.tm_mon -= 1;
                return HGExactTime(tm, 0, 0);
            }
        }
        
        // 格式2: "YYYYMMDDHHMMSS" (14位数字)
        if (timeStr.length() == 14) {
            std::tm tm = {};
            if (sscanf(timeStr.c_str(), "%4d%2d%2d%2d%2d%2d", 
                      &tm.tm_year, &tm.tm_mon, &tm.tm_mday, 
                      &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
                tm.tm_year -= 1900;
                tm.tm_mon -= 1;
                return HGExactTime(tm, 0, 0);
            }
        }
        
        // 如果所有格式都解析失败，返回当前时间
        return HGExactTime::currentTime();
    }

    std::vector<std::map<std::string, std::string>> searchLogs(
        const std::string& keyword,
        const std::string& timeFrom,
        const std::string& timeTo,
        int page,
        int pageSize,
        int* totalCount) {
        HGExactTime timeFromObj, timeToObj;
        if (!timeFrom.empty()) {
            timeFromObj = parseTimeString(timeFrom);
        }
        if (!timeTo.empty()) {
            timeToObj = parseTimeString(timeTo);
        }
        return RWDb::searchAuditTrailLogAcrossTablesWithPagination(
            keyword, timeFromObj, timeToObj, page, pageSize, totalCount);
    }

    int getLogCount(const std::string& tableName) {
        return RWDb::readAuditTrailLogCount(tableName);
    }

    int getSearchLogCount(const std::string& keyword,
                          const std::string& timeFrom,
                          const std::string& timeTo) {
        HGExactTime timeFromObj, timeToObj;
        if (!timeFrom.empty()) {
            timeFromObj = parseTimeString(timeFrom);
        }
        if (!timeTo.empty()) {
            timeToObj = parseTimeString(timeTo);
        }
        return RWDb::searchAuditTrailLogCount(keyword, timeFromObj, timeToObj);
    }

    std::vector<std::string> getLogTableNames() {
        return RWDb::getAllAuditLogTables();
    }

    bool saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                       const std::string& filePath,
                       const std::string& format) {
        if (format == "csv") {
            saveTableToCsv(logs, filePath);
        } else if (format == "txt") {
            saveTableToTxt(logs, filePath);
        } else if (format == "pdf") {
            saveTableToPdf(logs, filePath, RWDb::readCurDirPath()+("/resources/simhei.ttf"));
        }
        return false;
    }
};

LogModule::LogModule() : m_impl(new Impl()) {
}

LogModule::~LogModule() {
    delete m_impl;
}

bool LogModule::initialize(const std::string& dbPath) {
    return m_impl->initialize(dbPath);
}

void LogModule::shutdown() {
    m_impl->shutdown();
}

void LogModule::logInfo(const std::string& content) {
    m_impl->logInfo(content);
}

void LogModule::logDebug(const std::string& content) {
    m_impl->logDebug(content);
}

void LogModule::logError(const std::string& content) {
    m_impl->logError(content);
}

void LogModule::logWarn(const std::string& content) {
    m_impl->logWarn(content);
}

void LogModule::log(const std::string& content, int level) {
    m_impl->log(content, level);
}

std::vector<std::map<std::string, std::string>> LogModule::getAuditTrailLogs(
    const std::string& tableName) {
    return m_impl->getAuditTrailLogs(tableName);
}

std::vector<std::map<std::string, std::string>> LogModule::getRunLogs(
    const std::string& logPath) {
    return m_impl->getRunLogs(logPath);
}

std::vector<std::map<std::string, std::string>> LogModule::searchLogs(
    const std::string& keyword,
    const std::string& timeFrom,
    const std::string& timeTo,
    int page,
    int pageSize,
    int* totalCount) {
    return m_impl->searchLogs(keyword, timeFrom, timeTo, page, pageSize, totalCount);
}

int LogModule::getLogCount(const std::string& tableName) {
    return m_impl->getLogCount(tableName);
}

int LogModule::getSearchLogCount(const std::string& keyword,
                                  const std::string& timeFrom,
                                  const std::string& timeTo) {
    return m_impl->getSearchLogCount(keyword, timeFrom, timeTo);
}

std::vector<std::string> LogModule::getLogTableNames() {
    return m_impl->getLogTableNames();
}

bool LogModule::saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                               const std::string& filePath,
                               const std::string& format) {
    return m_impl->saveLogsToFile(logs, filePath, format);
}

}