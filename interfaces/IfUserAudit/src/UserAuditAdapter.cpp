#include "UserAuditAdapter.h"
#include "CoreUserAudit.h"

namespace HGMACHINE {

class UserAuditAdapter::Impl {
public:
    CoreUserAudit coreUserAudit;
};

UserAuditAdapter::UserAuditAdapter() : m_impl(new Impl()) {
}

UserAuditAdapter::~UserAuditAdapter() {
    delete m_impl;
}

bool UserAuditAdapter::initialize() {
    return m_impl->coreUserAudit.initialize();
}

void UserAuditAdapter::shutdown() {
    m_impl->coreUserAudit.shutdown();
}

// 用户管理
bool UserAuditAdapter::loadUsersInfo() {
    return m_impl->coreUserAudit.loadUsersInfo();
}

bool UserAuditAdapter::saveUsersInfo() {
    return m_impl->coreUserAudit.saveUsersInfo();
}

std::vector<std::map<std::string, std::string>> UserAuditAdapter::getUsersInfo() {
    return m_impl->coreUserAudit.getUsersInfo();
}

std::map<std::string, std::string> UserAuditAdapter::getUserInfo(int index) {
    return m_impl->coreUserAudit.getUserInfo(index);
}

std::map<std::string, std::string> UserAuditAdapter::getUserInfo(const std::string& userAccount) {
    return m_impl->coreUserAudit.getUserInfo(userAccount);
}

int UserAuditAdapter::getUserFieldIndex(const std::string& userAccount) {
    return m_impl->coreUserAudit.getUserFieldIndex(userAccount);
}

std::string UserAuditAdapter::getUserField(int index, const std::string& fieldName) const {
    return m_impl->coreUserAudit.getUserField(index, fieldName);
}

void UserAuditAdapter::setUserField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->coreUserAudit.setUserField(index, fieldName, value);
}

void UserAuditAdapter::addUserField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->coreUserAudit.addUserField(index, fieldName, value);
}

void UserAuditAdapter::addUserRecord(const std::map<std::string, std::string>& info) {
    m_impl->coreUserAudit.addUserRecord(info);
}

void UserAuditAdapter::setUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) {
    m_impl->coreUserAudit.setUserInfoWithIndex(index, info);
}

void UserAuditAdapter::deleteUserInfo(const std::string& userAccount) {
    m_impl->coreUserAudit.deleteUserInfo(userAccount);
}

std::vector<std::string> UserAuditAdapter::getUsersNo() {
    return m_impl->coreUserAudit.getUsersNo();
}

std::string UserAuditAdapter::getUserAuthority(const std::string& userAccount) {
    return m_impl->coreUserAudit.getUserAuthority(userAccount);
}

std::string UserAuditAdapter::getUserInfoLastCount() {
    return m_impl->coreUserAudit.getUserInfoLastCount();
}

// 权限管理
bool UserAuditAdapter::loadUserGroupInfo() {
    return m_impl->coreUserAudit.loadUserGroupInfo();
}

bool UserAuditAdapter::saveUserGroupInfo() {
    return m_impl->coreUserAudit.saveUserGroupInfo();
}

std::vector<std::string> UserAuditAdapter::getWholeAuthority() {
    return m_impl->coreUserAudit.getWholeAuthority();
}

void UserAuditAdapter::setWholeAuthority(const std::vector<std::string>& wholeAuthority) {
    m_impl->coreUserAudit.setWholeAuthority(wholeAuthority);
}

std::vector<std::string> UserAuditAdapter::getAuthorityDetail() {
    return m_impl->coreUserAudit.getAuthorityDetail();
}

std::vector<std::string> UserAuditAdapter::getAuthorityDetail(const std::string& authority) {
    return m_impl->coreUserAudit.getAuthorityDetail(authority);
}

std::vector<std::map<std::string, std::string>> UserAuditAdapter::getAuthorityInfo() {
    return m_impl->coreUserAudit.getAuthorityInfo();
}

std::string UserAuditAdapter::getAuthorityField(int index, const std::string& fieldName) const {
    return m_impl->coreUserAudit.getAuthorityField(index, fieldName);
}

void UserAuditAdapter::setAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->coreUserAudit.setAuthorityField(index, fieldName, value);
}

void UserAuditAdapter::addAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->coreUserAudit.addAuthorityField(index, fieldName, value);
}

void UserAuditAdapter::addAuthorityRecord(const std::map<std::string, std::string>& info) {
    m_impl->coreUserAudit.addAuthorityRecord(info);
}

void UserAuditAdapter::delAuthorityRecord(const std::string& value) {
    m_impl->coreUserAudit.delAuthorityRecord(value);
}

// 权限编辑
bool UserAuditAdapter::beginAuthorityEdit() {
    return m_impl->coreUserAudit.beginAuthorityEdit();
}

bool UserAuditAdapter::commitAuthorityEdit() {
    return m_impl->coreUserAudit.commitAuthorityEdit();
}

bool UserAuditAdapter::rollbackAuthorityEdit() {
    return m_impl->coreUserAudit.rollbackAuthorityEdit();
}

bool UserAuditAdapter::isAuthorityEditing() {
    return m_impl->coreUserAudit.isAuthorityEditing();
}

void UserAuditAdapter::addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue) {
    m_impl->coreUserAudit.addAuthorityFieldToEdit(authorityName, fieldValue);
}

void UserAuditAdapter::removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) {
    m_impl->coreUserAudit.removeAuthorityFieldFromEdit(authorityName, fieldValue);
}

}
