#include "CoreUserAudit.h"
#include "logmodule.h"
#include "CoreSystemData.h"

namespace HGMACHINE {

static LogModule g_logModule;

CoreUserAudit::CoreUserAudit() : m_initialized(false) {
    g_logModule.initialize("");
    m_systemData = new CoreSystemData();
}

CoreUserAudit::~CoreUserAudit() {
    shutdown();
    delete m_systemData;
}

bool CoreUserAudit::initialize() {
    if (!m_initialized) {
        m_systemData->initialize();
        m_systemData->loadSystemInfo();
        m_usersCache.load();
        m_authorityCache.load();
        m_initialized = true;
    }
    return true;
}

void CoreUserAudit::shutdown() {
    if (m_initialized) {
        m_systemData->saveSystemInfo();
        m_usersCache.save();
        m_authorityCache.save();
        m_initialized = false;
    }
}

// 系统信息管理
bool CoreUserAudit::loadSystemInfo() {
    return m_systemData->loadSystemInfo();
}

bool CoreUserAudit::saveSystemInfo() {
    return m_systemData->saveSystemInfo();
}

std::string CoreUserAudit::getSystemInfo(const std::string& key) {
    return m_systemData->getSystemInfo(key);
}

void CoreUserAudit::setSystemInfo(const std::string& key, const std::string& value) {
    m_systemData->setSystemInfo(key, value);
}

void CoreUserAudit::addSystemInfo(const std::string& key, const std::string& value) {
    m_systemData->addSystemInfo(key, value);
}

void CoreUserAudit::delSystemInfo(const std::string& key, const std::string& value) {
    m_systemData->delSystemInfo(key, value);
}

// 用户管理
bool CoreUserAudit::loadUsersInfo() {
    return m_usersCache.load();
}

bool CoreUserAudit::saveUsersInfo() {
    return m_usersCache.save();
}

std::vector<std::map<std::string, std::string>> CoreUserAudit::getUsersInfo() {
    return m_usersCache.getInfo();
}

std::map<std::string, std::string> CoreUserAudit::getUserInfo(int index) {
    return m_usersCache.getUserInfo(index);
}

std::map<std::string, std::string> CoreUserAudit::getUserInfo(const std::string& userAccount) {
    int index = m_usersCache.getUserFieldIndex(userAccount);
    if (index >= 0) {
        return m_usersCache.getUserInfo(index);
    }
    return std::map<std::string, std::string>();
}

int CoreUserAudit::getUserFieldIndex(const std::string& userAccount) {
    return m_usersCache.getUserFieldIndex(userAccount);
}

std::string CoreUserAudit::getUserField(int index, const std::string& fieldName) const {
    return m_usersCache.getField(index, fieldName);
}

void CoreUserAudit::setUserField(int index, const std::string& fieldName, const std::string& value) {
    m_usersCache.setField(index, fieldName, value);
}

void CoreUserAudit::addUserField(int index, const std::string& fieldName, const std::string& value) {
    m_usersCache.addFieldValue(index, fieldName, value);
}

void CoreUserAudit::addUserRecord(const std::map<std::string, std::string>& info) {
    m_usersCache.addRecord(info);
}

void CoreUserAudit::setUserInfoWithIndex(int index, const std::map<std::string, std::string>& info) {
    m_usersCache.setUserInfoWithIndex(index, info);
}

void CoreUserAudit::deleteUserInfo(const std::string& userAccount) {
    m_usersCache.deleteUserInfo(userAccount);
    g_logModule.writeAuditTrailLog("删除用户: " + userAccount);
}

std::vector<std::string> CoreUserAudit::getUsersNo() {
    return m_usersCache.getUsersNo();
}

std::string CoreUserAudit::getUserAuthority(const std::string& userAccount) {
    return m_usersCache.getAuthority(userAccount);
}

std::string CoreUserAudit::getUserInfoLastCount() {
    return m_usersCache.getUserInfoLastCount();
}

// 权限管理
bool CoreUserAudit::loadUserGroupInfo() {
    return m_authorityCache.load();
}

bool CoreUserAudit::saveUserGroupInfo() {
    return m_authorityCache.save();
}

std::vector<std::string> CoreUserAudit::getWholeAuthority() {
    return m_authorityCache.getWholeAuthority();
}

void CoreUserAudit::setWholeAuthority(const std::vector<std::string>& wholeAuthority) {
    m_authorityCache.setWholeAuthority(wholeAuthority);
}

std::vector<std::string> CoreUserAudit::getAuthorityDetail() {
    return m_authorityCache.getAuthorityDetail("", false);
}

std::vector<std::string> CoreUserAudit::getAuthorityDetail(const std::string& authority) {
    return m_authorityCache.getAuthorityDetail(authority, false);
}

std::vector<std::map<std::string, std::string>> CoreUserAudit::getAuthorityInfo() {
    return m_authorityCache.getInfo();
}

std::string CoreUserAudit::getAuthorityField(int index, const std::string& fieldName) const {
    return m_authorityCache.getField(index, fieldName);
}

void CoreUserAudit::setAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_authorityCache.setField(index, fieldName, value);
}

void CoreUserAudit::addAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_authorityCache.addFieldValue(index, fieldName, value);
}

void CoreUserAudit::addAuthorityRecord(const std::map<std::string, std::string>& info) {
    m_authorityCache.addRecord(info);
}

void CoreUserAudit::delAuthorityRecord(const std::string& value) {
    m_authorityCache.delRecord(value);
}

// 权限编辑
bool CoreUserAudit::beginAuthorityEdit() {
    return m_authorityCache.beginEdit();
}

bool CoreUserAudit::commitAuthorityEdit() {
    bool result = m_authorityCache.commitEdit();
    if (result) {
        g_logModule.writeAuditTrailLog("提交权限编辑");
    }
    return result;
}

bool CoreUserAudit::rollbackAuthorityEdit() {
    bool result = m_authorityCache.rollbackEdit();
    if (result) {
        g_logModule.writeAuditTrailLog("回滚权限编辑");
    }
    return result;
}

bool CoreUserAudit::isAuthorityEditing() {
    return m_authorityCache.isEditing();
}

void CoreUserAudit::addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue) {
    int index = -1;
    auto authorityInfo = m_authorityCache.getInfo();
    for (int i = 0; i < int(authorityInfo.size()); i++) {
        if (authorityInfo[i]["GroupName"] == authorityName) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        m_authorityCache.addFieldValueToEdit(index, "GroupPerson", fieldValue + ":");
    }
}

void CoreUserAudit::removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) {
    int index = -1;
    auto authorityInfo = m_authorityCache.getInfo();
    for (int i = 0; i < int(authorityInfo.size()); i++) {
        if (authorityInfo[i]["GroupName"] == authorityName) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        std::string groupPerson = m_authorityCache.getField(index, "GroupPerson");
        std::string searchStr = fieldValue + ":";
        size_t pos = groupPerson.find(searchStr);
        if (pos != std::string::npos) {
            groupPerson.erase(pos, searchStr.length());
            m_authorityCache.setFieldToEdit(index, "GroupPerson", groupPerson);
        }
    }
}

}
