#ifndef I_BASE_CONFIG_H
#define I_BASE_CONFIG_H

#include <string>
#include <vector>
#include <map>

class IBaseConfig {
public:
    virtual ~IBaseConfig() = default;

    virtual bool GetConfig(const std::string& key, std::string& value) = 0;
    virtual bool SetConfig(const std::string& key, const std::string& value) = 0;
    
    virtual bool GetIntConfig(const std::string& key, int& value) = 0;
    virtual bool SetIntConfig(const std::string& key, int value) = 0;
    
    virtual bool GetBoolConfig(const std::string& key, bool& value) = 0;
    virtual bool SetBoolConfig(const std::string& key, bool value) = 0;
    
    virtual bool GetDoubleConfig(const std::string& key, double& value) = 0;
    virtual bool SetDoubleConfig(const std::string& key, double value) = 0;
    
    virtual std::vector<std::string> GetAllKeys() const = 0;
    virtual bool SaveConfig() = 0;
    virtual bool LoadConfig() = 0;
    
    virtual std::string LoadTranslation(const std::string& language, const std::string& key) = 0;
    virtual std::string FindTranslationKey(const std::string& language, const std::string& key) = 0;

    
    virtual int SaveTableToTxt(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) = 0;
    virtual int SaveTableToCsv(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) = 0;
    virtual int SaveTableToPdf(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename, const std::string& charset) = 0;
};

#endif // I_BASE_CONFIG_H
