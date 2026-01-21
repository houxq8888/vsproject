#ifndef LOGMODULE_H
#define LOGMODULE_H

#include "LogModule_global.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace HGMACHINE {

class LOGMODULE_EXPORT LogModule
{
public:
    LogModule();
    ~LogModule();
    
    bool initialize(const std::string& dbPath);
    void shutdown();
    
    void logInfo(const std::string& content);
    void logDebug(const std::string& content);
    void logError(const std::string& content);
    void logWarn(const std::string& content);
    void log(const std::string& content, int level);
    
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName);
    
    std::vector<std::map<std::string, std::string>> getRunLogs(
        const std::string& logPath);
    
    std::vector<std::map<std::string, std::string>> searchLogs(
        const std::string& keyword,
        const std::string& timeFrom,
        const std::string& timeTo,
        int page,
        int pageSize,
        int* totalCount);
    
    int getLogCount(const std::string& tableName);
    
    int getSearchLogCount(const std::string& keyword,
                          const std::string& timeFrom,
                          const std::string& timeTo);
    
    std::vector<std::string> getLogTableNames();
    
    bool saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                       const std::string& filePath,
                       const std::string& format);

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // LOGMODULE_H
