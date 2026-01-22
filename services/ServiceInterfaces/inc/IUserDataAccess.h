#ifndef I_USER_DATA_ACCESS_H
#define I_USER_DATA_ACCESS_H

#include <string>
#include <vector>
#include <map>
#include "ServiceInterfaces_global.h"

class SERVICEINTERFACES_EXPORT IUserDataAccess {
public:
    virtual ~IUserDataAccess() = default;

    virtual void Load() = 0;
    virtual void Save() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> GetInfo() const = 0;
    virtual std::map<std::string, std::string> GetUserInfo(int index) const = 0;
    virtual std::vector<std::string> GetUsersNo() const = 0;
    virtual std::string GetAuthority(const std::string& key) = 0;
    virtual std::string GetField(int index, const std::string& fieldName) const = 0;
    virtual void SetField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void AddFieldValue(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void AddRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void SetUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) = 0;
    virtual void DeleteUserInfo(const std::string& userAccount) = 0;
    virtual int GetUserFieldIndex(const std::string& userAccount) const = 0;
};

#endif // I_USER_DATA_ACCESS_H
