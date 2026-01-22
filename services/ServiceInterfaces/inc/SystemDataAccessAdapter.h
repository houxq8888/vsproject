#ifndef SYSTEM_DATA_ACCESS_ADAPTER_H
#define SYSTEM_DATA_ACCESS_ADAPTER_H

#include "ISystemDataAccess.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT SystemDataAccessAdapter : public ISystemDataAccess {
public:
    SystemDataAccessAdapter();
    virtual ~SystemDataAccessAdapter() = default;

    void Load() override;
    void Save() override;
    
    std::string GetInfo(const std::string& key) override;
    void SetInfo(const std::string& key, const std::string& value) override;
    void AddInfo(const std::string& key, const std::string& value) override;
    void DelInfo(const std::string& key, const std::string& value) override;

private:
    class Impl;
    Impl* m_impl;
};

#endif // SYSTEM_DATA_ACCESS_ADAPTER_H
