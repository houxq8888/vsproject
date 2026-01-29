#ifndef USERAUDITMANAGER_H
#define USERAUDITMANAGER_H

#include "IfUserAudit_global.h"
#include "UserAuditAdapter.h"
#include <memory>

namespace HGMACHINE {

class IFUSERAUDITINTERFACE_EXPORT UserAuditManager {
public:
    static UserAuditManager& instance();
    
    bool initialize();
    void shutdown();
    
    IUserAudit& get() { return *m_userAudit; }
    
private:
    UserAuditManager();
    ~UserAuditManager();
    UserAuditManager(const UserAuditManager&) = delete;
    UserAuditManager& operator=(const UserAuditManager&) = delete;
    
private:
    std::unique_ptr<UserAuditAdapter> m_userAudit;
    bool m_initialized;
};

}

#endif
