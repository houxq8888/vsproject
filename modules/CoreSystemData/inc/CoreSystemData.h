#ifndef CORESYSTEMDATA_H
#define CORESYSTEMDATA_H

#include "CoreSystemData_global.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace HGMACHINE {

class CORESYSTEMDATA_EXPORT CoreSystemData {
public:
    CoreSystemData();
    ~CoreSystemData();
    
    bool initialize();
    void shutdown();
    
    bool loadSystemInfo();
    bool saveSystemInfo();
    std::string getSystemInfo(const std::string& key);
    void setSystemInfo(const std::string& key, const std::string& value);
    void addSystemInfo(const std::string& key, const std::string& value);
    void delSystemInfo(const std::string& key, const std::string& value);
    
    std::map<std::string, std::string> getAllSystemInfo();
    void setAllSystemInfo(const std::map<std::string, std::string>& info);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CORESYSTEMDATA_H
