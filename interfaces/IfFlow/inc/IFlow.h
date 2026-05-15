#ifndef IFLOW_H
#define IFLOW_H

#include "IfFlow_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFLOW_EXPORT IFlow {
public:
    virtual ~IFlow() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> readFlowInfo() = 0;
    virtual std::string readFlowOfTask(const std::string& dbName) = 0;
    virtual std::vector<std::string> getAllTables(const std::string& dbName) = 0;
    virtual std::vector<std::string> getMethodNames() = 0;
    
    virtual void writeFlowManageRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void writeFlowRecord(const std::string& dbName, bool coverFlag, 
                               const std::vector<std::map<std::string, std::string>>& info) = 0;
    
    virtual void deleteRecord(const std::string& tableName, const std::string& key, 
                           const std::string& value) = 0;
    virtual void deleteDB(const std::string& dbName) = 0;
    virtual void clearFlowManageRecord() = 0;
    
    virtual std::map<std::string, std::string> getFlowMap(int index, 
                                                          const std::string& indexStr,
                                                          const std::string& name,
                                                          const std::string& type,
                                                          const std::string& createTime,
                                                          const std::string& dbName) = 0;
    virtual std::vector<std::map<std::string, std::string>> getFlowStepMap(const std::string& indexStr,
                                                                           const std::string& name,
                                                                           const std::string& type,
                                                                           const std::string& createTime,
                                                                           const std::string& dbName,
                                                                           const std::vector<std::map<std::string, std::string>>& steps) = 0;
};

}

#endif // IFLOW_H
