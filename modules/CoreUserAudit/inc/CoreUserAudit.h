#ifndef COREUSERAUDIT_H
#define COREUSERAUDIT_H

#include <map>
#include <vector>
#include <string>
#include "UsersInfoCache.h"
#include "AuthorityInfoCache.h"

namespace HGMACHINE {

class CoreSystemData;

class CoreUserAudit {
public:
    CoreUserAudit();
    ~CoreUserAudit();
    
    bool initialize();
    void shutdown();
    
    // 系统信息管理
    bool loadSystemInfo();
    bool saveSystemInfo();
    std::string getSystemInfo(const std::string& key);
    void setSystemInfo(const std::string& key, const std::string& value);
    void addSystemInfo(const std::string& key, const std::string& value);
    void delSystemInfo(const std::string& key, const std::string& value);
    
    // 用户管理
    bool loadUsersInfo();
    bool saveUsersInfo();
    
    std::vector<std::map<std::string, std::string>> getUsersInfo();
    std::map<std::string, std::string> getUserInfo(int index);
    std::map<std::string, std::string> getUserInfo(const std::string& userAccount);
    int getUserFieldIndex(const std::string& userAccount);
    std::string getUserField(int index, const std::string& fieldName) const;
    void setUserField(int index, const std::string& fieldName, const std::string& value);
    void addUserField(int index, const std::string& fieldName, const std::string& value);
    void addUserRecord(const std::map<std::string, std::string>& info);
    void setUserInfoWithIndex(int index, const std::map<std::string, std::string>& info);
    void deleteUserInfo(const std::string& userAccount);
    std::vector<std::string> getUsersNo();
    std::string getUserAuthority(const std::string& userAccount);
    std::string getUserInfoLastCount();
    
    // 权限管理
    bool loadUserGroupInfo();
    bool saveUserGroupInfo();
    
    std::vector<std::string> getWholeAuthority();
    void setWholeAuthority(const std::vector<std::string>& wholeAuthority);
    std::vector<std::string> getAuthorityDetail();
    std::vector<std::string> getAuthorityDetail(const std::string& authority);
    std::vector<std::map<std::string, std::string>> getAuthorityInfo();
    std::string getAuthorityField(int index, const std::string& fieldName) const;
    void setAuthorityField(int index, const std::string& fieldName, const std::string& value);
    void addAuthorityField(int index, const std::string& fieldName, const std::string& value);
    void addAuthorityRecord(const std::map<std::string, std::string>& info);
    void delAuthorityRecord(const std::string& value);
    
    // 权限编辑
    bool beginAuthorityEdit();
    bool commitAuthorityEdit();
    bool rollbackAuthorityEdit();
    bool isAuthorityEditing();
    void addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue);
    void removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue);
    
private:
    UsersInfoCache m_usersCache;
    AuthorityInfoCache m_authorityCache;
    bool m_initialized;
    CoreSystemData* m_systemData;
};

}

#endif
