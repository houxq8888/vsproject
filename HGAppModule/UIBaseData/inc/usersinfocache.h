#ifndef USERSINFOCACHE_H
#define USERSINFOCACHE_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

class UsersInfoCache
{
public:
    void load();
    void save();
    std::vector<std::map<std::string,std::string>> getInfo() const ;
    std::map<std::string,std::string> getUserInfo(int index) const;
    std::vector<std::string> getUsersNo() const;
    std::string getAuthority(const std::string &key);
    std::string getField(int index, const std::string& fieldName) const ;
    void setField(int index, const std::string& fieldName, const std::string& value);
    void addFieldValue(int index, const std::string& fieldName, const std::string& value);
    void addRecord(const std::map<std::string,std::string> &info);
    void setUserInfoWithIndex(int index, const std::map<std::string,std::string> &info);
    void deleteUserInfo(const std::string &userAccount);
    int getUserFieldIndex(const std::string& userAccount) const;

private:
    std::vector<std::map<std::string,std::string>> m_data;
    bool m_dirty = false;
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

#endif