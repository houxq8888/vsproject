#ifndef USERAUDITADAPTER_H
#define USERAUDITADAPTER_H

#include "IUserAudit.h"
#include "IfUserAudit_global.h"
#include <memory>

namespace HGMACHINE {

class IFUSERAUDITINTERFACE_EXPORT UserAuditAdapter : public IUserAudit {
public:
    UserAuditAdapter();
    ~UserAuditAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    // 用户管理
    bool loadUsersInfo() override;
    bool saveUsersInfo() override;
    
    std::vector<std::map<std::string, std::string>> getUsersInfo() override;
    std::map<std::string, std::string> getUserInfo(int index) override;
    std::map<std::string, std::string> getUserInfo(const std::string& userAccount) override;
    int getUserFieldIndex(const std::string& userAccount) override;
    std::string getUserField(int index, const std::string& fieldName) const override;
    void setUserField(int index, const std::string& fieldName, const std::string& value) override;
    void addUserField(int index, const std::string& fieldName, const std::string& value) override;
    void addUserRecord(const std::map<std::string, std::string>& info) override;
    void setUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) override;
    void deleteUserInfo(const std::string& userAccount) override;
    std::vector<std::string> getUsersNo() override;
    std::string getUserAuthority(const std::string& userAccount) override;
    std::string getUserInfoLastCount() override;
    
    // 权限管理
    bool loadUserGroupInfo() override;
    bool saveUserGroupInfo() override;
    
    std::vector<std::string> getWholeAuthority() override;
    void setWholeAuthority(const std::vector<std::string>& wholeAuthority) override;
    std::vector<std::string> getAuthorityDetail() override;
    std::vector<std::string> getAuthorityDetail(const std::string& authority) override;
    std::vector<std::map<std::string, std::string>> getAuthorityInfo() override;
    std::string getAuthorityField(int index, const std::string& fieldName) const override;
    void setAuthorityField(int index, const std::string& fieldName, const std::string& value) override;
    void addAuthorityField(int index, const std::string& fieldName, const std::string& value) override;
    void addAuthorityRecord(const std::map<std::string, std::string>& info) override;
    void delAuthorityRecord(const std::string& value) override;
    
    // 权限编辑
    bool beginAuthorityEdit() override;
    bool commitAuthorityEdit() override;
    bool rollbackAuthorityEdit() override;
    bool isAuthorityEditing() override;
    void addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue) override;
    void removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif
