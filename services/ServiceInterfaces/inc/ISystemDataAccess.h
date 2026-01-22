#ifndef I_SYSTEM_DATA_ACCESS_H
#define I_SYSTEM_DATA_ACCESS_H

#include <string>
#include <vector>
#include "ServiceInterfaces_global.h"

class SERVICEINTERFACES_EXPORT ISystemDataAccess {
public:
    virtual ~ISystemDataAccess() = default;

    virtual void Load() = 0;
    virtual void Save() = 0;
    
    virtual std::string GetInfo(const std::string& key) = 0;
    virtual void SetInfo(const std::string& key, const std::string& value) = 0;
    virtual void AddInfo(const std::string& key, const std::string& value) = 0;
    virtual void DelInfo(const std::string& key, const std::string& value) = 0;
};

#endif // I_SYSTEM_DATA_ACCESS_H
