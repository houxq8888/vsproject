#ifndef COREFLOW_H
#define COREFLOW_H

#include "CoreFlow_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

struct StepOfFlow {
    int index;
    std::string name;
    std::string param;
    std::string nameOfSameStep;
    int indexOfSameStep;
};

struct FlowOfTask {
    std::string indexStr;
    std::string name;
    std::string type;
    std::string createTime;
    std::string dbName;
    std::vector<StepOfFlow> steps;
};

class COREFLOW_EXPORT CoreFlow {
public:
    CoreFlow();
    ~CoreFlow();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> readFlowInfo();
    std::string readFlowOfTask(const std::string& dbName);
    std::vector<std::string> getAllTables(const std::string& dbName);
    std::vector<std::string> getMethodNames();
    
    void writeFlowManageRecord(const std::map<std::string, std::string>& info);
    void writeFlowRecord(const std::string& dbName, bool coverFlag, 
                       const std::vector<std::map<std::string, std::string>>& info);
    
    void deleteRecord(const std::string& tableName, const std::string& key, 
                     const std::string& value);
    void deleteDB(const std::string& dbName);
    void clearFlowManageRecord();
    
    std::map<std::string, std::string> getFlowMap(int index, 
                                                      const std::string& indexStr,
                                                      const std::string& name,
                                                      const std::string& type,
                                                      const std::string& createTime,
                                                      const std::string& dbName);
    std::vector<std::map<std::string, std::string>> getFlowStepMap(const std::string& indexStr,
                                                                   const std::string& name,
                                                                   const std::string& type,
                                                                   const std::string& createTime,
                                                                   const std::string& dbName,
                                                                   const std::vector<std::map<std::string, std::string>>& steps);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // COREFLOW_H
