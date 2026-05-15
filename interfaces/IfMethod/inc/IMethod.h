#ifndef IMETHOD_H
#define IMETHOD_H

#include "IfMethod_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFMETHOD_EXPORT IMethod {
public:
    virtual ~IMethod() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> readMethodInfo() = 0;
    virtual std::string readMethodParam(const std::string& dbName) = 0;
    virtual std::vector<std::string> getAllTables(const std::string& dbName) = 0;
    
    virtual void writeMethodManageRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void writeMethodRecord(const std::string& dbName, bool coverFlag, 
                                   const std::vector<std::map<std::string, std::string>>& info) = 0;
    virtual std::string getMethodTableName() = 0;
    virtual std::string getMethodPrefix() = 0;
    virtual void deleteRecord(const std::string& key, 
                             const std::string& value) = 0;
    virtual void deleteDB(const std::string& dbName) = 0;
    virtual void clearMethodManageRecord() = 0;
    
    virtual std::map<std::string, std::string> getMethodMap(int index, 
                                                           const std::string& indexStr,
                                                           const std::string& name,
                                                           const std::string& type,
                                                           const std::string& createTime,
                                                           const std::string& dbName,
                                                           const std::string& param) = 0;
    virtual std::vector<std::map<std::string, std::string>> getMethodParamMap(const std::string& indexStr,
                                                                               const std::string& name,
                                                                               const std::string& type,
                                                                               const std::string& createTime,
                                                                               const std::string& dbName,
                                                                               const std::string& param) = 0;
};

}

#endif // IMETHOD_H
