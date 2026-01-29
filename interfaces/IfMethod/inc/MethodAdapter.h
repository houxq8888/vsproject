#ifndef METHODADAPTER_H
#define METHODADAPTER_H

#include "IMethod.h"
#include "IfMethod_global.h"

namespace HGMACHINE {

class IFMETHOD_EXPORT MethodAdapter : public IMethod {
public:
    MethodAdapter();
    virtual ~MethodAdapter();
    
    bool initialize() override;
    void shutdown() override;
    
    std::vector<std::map<std::string, std::string>> readMethodInfo() override;
    std::string readMethodParam(const std::string& dbName) override;
    std::vector<std::string> getAllTables(const std::string& dbName) override;
    
    void writeMethodManageRecord(const std::map<std::string, std::string>& info) override;
    void writeMethodRecord(const std::string& dbName, bool coverFlag, 
                           const std::vector<std::map<std::string, std::string>>& info) override;
    std::string getMethodTableName() override;
    std::string getMethodPrefix() override;
    void deleteRecord(const std::string& key, 
                     const std::string& value) override;
    void deleteDB(const std::string& dbName) override;
    void clearMethodManageRecord() override;
    
    std::map<std::string, std::string> getMethodMap(int index, 
                                                      const std::string& indexStr,
                                                      const std::string& name,
                                                      const std::string& type,
                                                      const std::string& createTime,
                                                      const std::string& dbName,
                                                      const std::string& param) override;
    std::vector<std::map<std::string, std::string>> getMethodParamMap(const std::string& indexStr,
                                                                        const std::string& name,
                                                                        const std::string& type,
                                                                        const std::string& createTime,
                                                                        const std::string& dbName,
                                                                        const std::string& param) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // METHODADAPTER_H
