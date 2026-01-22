#include "UserDataAccessAdapter.h"
#include "usersinfocache.h"

class UserDataAccessAdapter::Impl {
public:
    UsersInfoCache cache;
};

UserDataAccessAdapter::UserDataAccessAdapter() : m_impl(new Impl()) {
}

UserDataAccessAdapter::~UserDataAccessAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void UserDataAccessAdapter::Load() {
    m_impl->cache.load();
}

void UserDataAccessAdapter::Save() {
    m_impl->cache.save();
}

std::vector<std::map<std::string, std::string>> UserDataAccessAdapter::GetInfo() const {
    return m_impl->cache.getInfo();
}

std::map<std::string, std::string> UserDataAccessAdapter::GetUserInfo(int index) const {
    return m_impl->cache.getUserInfo(index);
}

std::vector<std::string> UserDataAccessAdapter::GetUsersNo() const {
    return m_impl->cache.getUsersNo();
}

std::string UserDataAccessAdapter::GetAuthority(const std::string& key) {
    return m_impl->cache.getAuthority(key);
}

std::string UserDataAccessAdapter::GetField(int index, const std::string& fieldName) const {
    return m_impl->cache.getField(index, fieldName);
}

void UserDataAccessAdapter::SetField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->cache.setField(index, fieldName, value);
}

void UserDataAccessAdapter::AddFieldValue(int index, const std::string& fieldName, const std::string& value) {
    m_impl->cache.addFieldValue(index, fieldName, value);
}

void UserDataAccessAdapter::AddRecord(const std::map<std::string, std::string>& info) {
    m_impl->cache.addRecord(info);
}

void UserDataAccessAdapter::SetUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) {
    m_impl->cache.setUserInfoWithIndex(index, info);
}

void UserDataAccessAdapter::DeleteUserInfo(const std::string& userAccount) {
    m_impl->cache.deleteUserInfo(userAccount);
}

int UserDataAccessAdapter::GetUserFieldIndex(const std::string& userAccount) const {
    return m_impl->cache.getUserFieldIndex(userAccount);
}
