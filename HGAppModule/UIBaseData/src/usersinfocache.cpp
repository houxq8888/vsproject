#include "usersinfocache.h"
#include "rwDb.h"
#include "hgcommonutility.h"

using namespace HGMACHINE;

void UsersInfoCache::load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        m_data = RWDb::readUserInfo();
        m_loaded = true;
    }
}

void UsersInfoCache::save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_dirty) {
        for (int i=0;i<int(m_data.size());i++)
            RWDb::writeUserInfo(m_data[i]);
        m_dirty = false;
    }
}
std::string UsersInfoCache::getAuthority(const std::string &key){
    std::string authority;
    for (int i=0;i<int(m_data.size());i++){
        if (key == m_data[i]["UserAccount"]){
            authority = m_data[i]["Authority"];
            break;
        }
    }

    return authority;
}

std::vector<std::map<std::string, std::string>> UsersInfoCache::getInfo() const {
    return m_data;
}
std::map<std::string, std::string> UsersInfoCache::getUserInfo(int index) const {
    if (index >= 0 && index < int(m_data.size()))
        return m_data[index];
    else {
        return std::map<std::string, std::string>{};
    }
}
void UsersInfoCache::deleteUserInfo(const std::string& userAccount){
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<std::string> userNos = getUsersNo();
    int index=-1;
    for (int i = 0; i < int(userNos.size()); i++) {
        if (userNos[i] == userAccount) {
            index = i;
            break;
        }
    }
    if (index >= 0 && index < int(m_data.size())){
        std::string userNoErase = m_data[index]["UserNo"];
        m_data.erase(m_data.begin() + index);
        RWDb::deleteRecord(USERINFODBNAME,"UserNo",userNoErase);
        m_dirty=true;
    }
}
std::vector<std::string> UsersInfoCache::getUsersNo() const{
    std::vector<std::string> userNos;
    for (int i = 0; i < int(m_data.size()); i++) {
        userNos.push_back(m_data[i].at("UserAccount"));
    }
    return userNos;
}
int UsersInfoCache::getUserFieldIndex(const std::string& userAccount) const{ 
    int index=-1;
    for (int i = 0; i < int(m_data.size()); i++) {
        if (m_data[i].at("UserAccount") == userAccount)
        {
            index=i;
            break;
        }
    }
    return index;
}
std::string UsersInfoCache::getField(int index, const std::string& fieldName) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        auto it = m_data[index].find(fieldName);
        if (it != m_data[index].end())
            return it->second;
    }
    return "";
}

void UsersInfoCache::setField(int index, const std::string& fieldName, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        m_data[index][fieldName] = value;
        m_dirty = true;
    }
}
void UsersInfoCache::addFieldValue(int index, const std::string& fieldName, const std::string& value){
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        if (m_data[index][fieldName].find(value)!=std::string::npos) return;
        m_data[index][fieldName] += value;
        m_dirty = true;
    }
}
void UsersInfoCache::addRecord(const std::map<std::string,std::string> &info){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push_back(info);
    m_dirty = true;
}
void UsersInfoCache::setUserInfoWithIndex(int index, const std::map<std::string,std::string> &info){
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        m_data[index] = info;
        m_dirty = true;
    }
}
