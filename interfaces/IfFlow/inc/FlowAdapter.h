#ifndef FLOWADAPTER_H
#define FLOWADAPTER_H

#include "IFlow.h"
#include "IfFlow_global.h"

namespace HGMACHINE {

class IFLOW_EXPORT FlowAdapter : public IFlow {
public:
    FlowAdapter();
    virtual ~FlowAdapter();
    
    bool initialize() override;
    void shutdown() override;
    
    std::vector<std::map<std::string, std::string>> readFlowInfo() override;
    std::string readFlowOfTask(const std::string& dbName) override;
    std::vector<std::string> getAllTables(const std::string& dbName) override;
    std::vector<std::string> getMethodNames() override;
    
    void writeFlowManageRecord(const std::map<std::string, std::string>& info) override;
    void writeFlowRecord(const std::string& dbName, bool coverFlag, 
                         const std::vector<std::map<std::string, std::string>>& info) override;
    
    void deleteRecord(const std::string& tableName, const std::string& key, 
                     const std::string& value) override;
    void deleteDB(const std::string& dbName) override;
    void clearFlowManageRecord() override;
    
    std::map<std::string, std::string> getFlowMap(int index, 
                                                    const std::string& indexStr,
                                                    const std::string& name,
                                                    const std::string& type,
                                                    const std::string& createTime,
                                                    const std::string& dbName) override;
    std::vector<std::map<std::string, std::string>> getFlowStepMap(const std::string& indexStr,
                                                                  const std::string& name,
                                                                  const std::string& type,
                                                                  const std::string& createTime,
                                                                  const std::string& dbName,
                                                                  const std::vector<std::map<std::string, std::string>>& steps) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // FLOWADAPTER_H
