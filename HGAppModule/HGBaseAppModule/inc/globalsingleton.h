#ifndef GLOBALSINGLETON_H
#define GLOBALSINGLETON_H 

#include <map>
#include "systeminfocache.h"
#include "datachartinfocache.h"
#include "authorityinfocache.h"
#include "usersinfocache.h"

class GlobalSingleton
{
public:
    static GlobalSingleton& instance();
    void saveAll();
    // systeminfo
    void loadSystemInfo();
    void saveSystemInfo();
    std::string getSystemInfo(const std::string &key);
    void setSystemInfo(const std::string &key, const std::string &value);
    void addSystemInfo(const std::string &key, const std::string &value);
    // datachartinfo
    void loadDataChartInfo();
    void saveDataChartInfo();
    std::string getDataChartInfo(const std::string &key);
    void setDataChartInfo(const std::string &key, const std::string &value);
    void addDataChartInfo(const std::string &key, const std::string &value);
    // authorityInfo
    void loadUserGroupInfo();
    void saveUserGroupInfo();
    std::vector<std::string> getWholeAuthority();
    void setWholeAuthority(const std::vector<std::string> &wholeAuthority);
    std::vector<std::string> getAuthorityDetail();
    std::vector<std::string> getAuthorityDetail(const std::string& authority);
    std::vector<std::map<std::string, std::string>> getAuthorityInfo();
    std::string getAuthorityField(int index, const std::string& fieldName) const ;
    void setAuthorityField(int index, const std::string& fieldName, const std::string& value);
    void addAuthorityField(int index, const std::string &fieldName, const std::string &value);
    void addAuthorityRecord(const std::map<std::string,std::string> &info);
    // usersinfo
    void loadUsersInfo();
    void saveUsersInfo();
    int getUserFieldIndex(const std::string& userAccount);
    std::string getUserAuthority(const std::string& key);
    std::vector<std::map<std::string, std::string>> getUsersInfo();
    std::map<std::string,std::string> getUserInfo(int index);
    void deleteUserInfo(const std::string& userAccount);
    std::vector<std::string> getUsersNo();
    void setUserField(int index, const std::string& fieldName, const std::string& value);
    std::string getUserField(int index, const std::string& fieldName) const ;
    void addUserField(int index, const std::string &fieldName, const std::string &value);
    void addUserRecord(const std::map<std::string,std::string> &info);
    void setUserInfoWithIndex(int index, const std::map<std::string,std::string> &info);
    // serviceinfo
    // void loadServiceInfo();
    // void saveServiceInfo();
    // std::string getServiceInfo(const std::string &key);
    // void setServiceInfo(const std::string &key, const std::string &value);
    // void addServiceInfo(const std::string &key, const std::string &value);


private:
    GlobalSingleton();
    GlobalSingleton(const GlobalSingleton&)=delete;
    GlobalSingleton& operator=(const GlobalSingleton&)=delete;

private:
    SystemInfoCache m_systemInfo;
    DataChartInfoCache m_dataChartInfo;
    AuthorityInfoCache m_authorityInfo;
    UsersInfoCache m_usersInfo;
};

#endif