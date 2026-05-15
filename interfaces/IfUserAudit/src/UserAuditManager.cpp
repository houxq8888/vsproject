#include "UserAuditManager.h"

namespace HGMACHINE {

UserAuditManager::UserAuditManager() : m_initialized(false) {
    m_userAudit = std::make_unique<UserAuditAdapter>();
}

UserAuditManager::~UserAuditManager() {
    shutdown();
}

UserAuditManager& UserAuditManager::instance() {
    static UserAuditManager instance;
    return instance;
}

bool UserAuditManager::initialize() {
    if (!m_initialized) {
        m_userAudit->initialize();
        // m_userAudit->loadSystemInfo();
        m_userAudit->loadUsersInfo();
        m_userAudit->loadUserGroupInfo();
        m_initialized = true;
    }
    return true;
}

void UserAuditManager::shutdown() {
    if (m_initialized) {
        // m_userAudit->saveSystemInfo();
        m_userAudit->saveUsersInfo();
        m_userAudit->saveUserGroupInfo();
        m_userAudit->shutdown();
        m_initialized = false;
    }
}

}
