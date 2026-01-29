#ifndef SYSTEMDATAADAPTER_H
#define SYSTEMDATAADAPTER_H

#include "ISystemData.h"
#include <memory>

namespace HGMACHINE {

class SystemDataAdapter : public ISystemData {
public:
    SystemDataAdapter();
    ~SystemDataAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    bool loadSystemInfo() override;
    bool saveSystemInfo() override;
    std::string getSystemInfo(const std::string& key) override;
    void setSystemInfo(const std::string& key, const std::string& value) override;
    void addSystemInfo(const std::string& key, const std::string& value) override;
    void delSystemInfo(const std::string& key, const std::string& value) override;
    
    std::map<std::string, std::string> getAllSystemInfo() override;
    void setAllSystemInfo(const std::map<std::string, std::string>& info) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif
