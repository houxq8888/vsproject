#ifndef USER_DATA_ACCESS_ADAPTER_H
#define USER_DATA_ACCESS_ADAPTER_H

#include "IUserDataAccess.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT UserDataAccessAdapter : public IUserDataAccess {
public:
    UserDataAccessAdapter();
    virtual ~UserDataAccessAdapter() = default;

    void Load() override;
    void Save() override;
    
    std::vector<std::map<std::string, std::string>> GetInfo() const override;
    std::map<std::string, std::string> GetUserInfo(int index) const override;
    std::vector<std::string> GetUsersNo() const override;
    std::string GetAuthority(const std::string& key) override;
    std::string GetField(int index, const std::string& fieldName) const override;
    void SetField(int index, const std::string& fieldName, const std::string& value) override;
    void AddFieldValue(int index, const std::string& fieldName, const std::string& value) override;
    void AddRecord(const std::map<std::string, std::string>& info) override;
    void SetUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) override;
    void DeleteUserInfo(const std::string& userAccount) override;
    int GetUserFieldIndex(const std::string& userAccount) const override;

private:
    class Impl;
    Impl* m_impl;
};

#endif // USER_DATA_ACCESS_ADAPTER_H
