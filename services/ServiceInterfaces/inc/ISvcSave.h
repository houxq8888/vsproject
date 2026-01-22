#ifndef I_SVC_SAVE_H
#define I_SVC_SAVE_H

#include <string>
#include <vector>
#include <memory>

class ISvcSave {
public:
    virtual ~ISvcSave() = default;

    virtual bool SaveString(const std::string& key, const std::string& value) = 0;
    virtual bool LoadString(const std::string& key, std::string& value) = 0;
    
    virtual bool SaveInt(const std::string& key, int value) = 0;
    virtual bool LoadInt(const std::string& key, int& value) = 0;
    
    virtual bool SaveDouble(const std::string& key, double value) = 0;
    virtual bool LoadDouble(const std::string& key, double& value) = 0;
    
    virtual bool SaveBinary(const std::string& key, const std::vector<uint8_t>& data) = 0;
    virtual bool LoadBinary(const std::string& key, std::vector<uint8_t>& data) = 0;
    
    virtual bool DeleteKey(const std::string& key) = 0;
    virtual bool ClearAll() = 0;
    
    virtual bool Exists(const std::string& key) const = 0;
    virtual std::vector<std::string> GetAllKeys() const = 0;
};

#endif // I_SVC_SAVE_H
