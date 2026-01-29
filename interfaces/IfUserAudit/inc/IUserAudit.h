#ifndef IUSERAUDIT_H
#define IUSERAUDIT_H

#include <string>
#include <vector>
#include <map>
#include "IfUserAudit_global.h"

namespace HGMACHINE {

class IFUSERAUDITINTERFACE_EXPORT IUserAudit {
public:
    virtual ~IUserAudit() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    

    // 用户管理
    virtual bool loadUsersInfo() = 0;
    virtual bool saveUsersInfo() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> getUsersInfo() = 0;
    virtual std::map<std::string, std::string> getUserInfo(int index) = 0;
    virtual std::map<std::string, std::string> getUserInfo(const std::string& userAccount) = 0;
    virtual int getUserFieldIndex(const std::string& userAccount) = 0;
    virtual std::string getUserField(int index, const std::string& fieldName) const = 0;
    virtual void setUserField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void addUserField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void addUserRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void setUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) = 0;
    virtual void deleteUserInfo(const std::string& userAccount) = 0;
    virtual std::vector<std::string> getUsersNo() = 0;
    virtual std::string getUserAuthority(const std::string& userAccount) = 0;
    virtual std::string getUserInfoLastCount() = 0;
    
    // 权限管理
    virtual bool loadUserGroupInfo() = 0;
    virtual bool saveUserGroupInfo() = 0;
    
    virtual std::vector<std::string> getWholeAuthority() = 0;
    virtual void setWholeAuthority(const std::vector<std::string>& wholeAuthority) = 0;
    virtual std::vector<std::string> getAuthorityDetail() = 0;
    virtual std::vector<std::string> getAuthorityDetail(const std::string& authority) = 0;
    virtual std::vector<std::map<std::string, std::string>> getAuthorityInfo() = 0;
    virtual std::string getAuthorityField(int index, const std::string& fieldName) const = 0;
    virtual void setAuthorityField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void addAuthorityField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void addAuthorityRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void delAuthorityRecord(const std::string& value) = 0;
    
    // 权限编辑
    virtual bool beginAuthorityEdit() = 0;
    virtual bool commitAuthorityEdit() = 0;
    virtual bool rollbackAuthorityEdit() = 0;
    virtual bool isAuthorityEditing() = 0;
    virtual void addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue) = 0;
    virtual void removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) = 0;
};

}

#endif // IUSERAUDIT_H
