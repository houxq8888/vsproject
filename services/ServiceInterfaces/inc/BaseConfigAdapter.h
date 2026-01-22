#ifndef BASE_CONFIG_ADAPTER_H
#define BASE_CONFIG_ADAPTER_H

#include "IBaseConfig.h"
#include "ServiceInterfaces_global.h"
#include <string>
#include <memory>

class SERVICEINTERFACES_EXPORT BaseConfigAdapter : public IBaseConfig {
private:
    class Impl;
    Impl* m_impl;
    
public:
    BaseConfigAdapter();
    virtual ~BaseConfigAdapter();
    
    bool GetConfig(const std::string& key, std::string& value) override;
    bool SetConfig(const std::string& key, const std::string& value) override;
    
    bool GetIntConfig(const std::string& key, int& value) override;
    bool SetIntConfig(const std::string& key, int value) override;
    
    bool GetBoolConfig(const std::string& key, bool& value) override;
    bool SetBoolConfig(const std::string& key, bool value) override;
    
    bool GetDoubleConfig(const std::string& key, double& value) override;
    bool SetDoubleConfig(const std::string& key, double value) override;
    
    std::vector<std::string> GetAllKeys() const override;
    bool SaveConfig() override;
    bool LoadConfig() override;
    
    std::string LoadTranslation(const std::string& language, const std::string& key) override;
    
    std::map<std::string, std::string> GetParamMap(const std::string& param) override;
    
    std::string GetDirPath() const override;
    int SaveTableToTxt(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) override;
    int SaveTableToCsv(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) override;
    int SaveTableToPdf(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename, const std::string& charset) override;
};

#endif // BASE_CONFIG_ADAPTER_H
