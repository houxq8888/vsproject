#ifndef SVC_SAVE_ADAPTER_H
#define SVC_SAVE_ADAPTER_H

#include "ISvcSave.h"
#include "ServiceInterfaces_global.h"
#include <string>
#include <vector>
#include <memory>

class SERVICEINTERFACES_EXPORT SvcSaveAdapter : public ISvcSave {
private:
    class Impl;
    Impl* m_impl;
    
public:
    SvcSaveAdapter();
    virtual ~SvcSaveAdapter();
    
    bool SaveString(const std::string& key, const std::string& value) override;
    bool LoadString(const std::string& key, std::string& value) override;
    
    bool SaveInt(const std::string& key, int value) override;
    bool LoadInt(const std::string& key, int& value) override;
    
    bool SaveDouble(const std::string& key, double value) override;
    bool LoadDouble(const std::string& key, double& value) override;
    
    bool SaveBinary(const std::string& key, const std::vector<uint8_t>& data) override;
    bool LoadBinary(const std::string& key, std::vector<uint8_t>& data) override;
    
    bool DeleteKey(const std::string& key) override;
    bool ClearAll() override;
    
    bool Exists(const std::string& key) const override;
    std::vector<std::string> GetAllKeys() const override;

    void SaveImage(const HGMACHINE::HGImg2D& mat, const std::string &path) override;
};

#endif // SVC_SAVE_ADAPTER_H
