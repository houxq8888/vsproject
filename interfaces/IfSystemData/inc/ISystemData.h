#ifndef ISYSTEMDATA_H
#define ISYSTEMDATA_H

#include "IfSystemData_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFSYSTEMDATA_EXPORT ISystemData {
public:
    virtual ~ISystemData() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual bool loadSystemInfo() = 0;
    virtual bool saveSystemInfo() = 0;
    virtual std::string getSystemInfo(const std::string& key) = 0;
    virtual void setSystemInfo(const std::string& key, const std::string& value) = 0;
    virtual void addSystemInfo(const std::string& key, const std::string& value) = 0;
    virtual void delSystemInfo(const std::string& key, const std::string& value) = 0;
    
    virtual std::map<std::string, std::string> getAllSystemInfo() = 0;
    virtual void setAllSystemInfo(const std::map<std::string, std::string>& info) = 0;
};

}

#endif // ISYSTEMDATA_H
