#ifndef COREMETHOD_H
#define COREMETHOD_H

#include "CoreMethod_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class COREMETHOD_EXPORT CoreMethod {
public:
    CoreMethod();
    ~CoreMethod();
    
    bool initialize();
    void shutdown();
    
    std::string getMethodPrefix();
    std::string getMethodTableName();
    std::vector<std::map<std::string, std::string>> readMethodInfo();
    std::string readMethodParam(const std::string& dbName);
    std::vector<std::string> getAllTables(const std::string& dbName);
    
    void writeMethodManageRecord(const std::map<std::string, std::string>& info);
    void writeMethodRecord(const std::string& dbName, bool coverFlag, 
                           const std::vector<std::map<std::string, std::string>>& info);
    
    void deleteRecord(const std::string& key, 
                     const std::string& value);
    void deleteDB(const std::string& dbName);
    void clearMethodManageRecord();
    
    std::map<std::string, std::string> getMethodMap(int index, 
                                                     const std::string& indexStr,
                                                     const std::string& name,
                                                     const std::string& type,
                                                     const std::string& createTime,
                                                     const std::string& dbName,
                                                     const std::string& param);
    std::vector<std::map<std::string, std::string>> getMethodParamMap(const std::string& indexStr,
                                                                        const std::string& name,
                                                                        const std::string& type,
                                                                        const std::string& createTime,
                                                                        const std::string& dbName,
                                                                        const std::string& param);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // COREMETHOD_H
