#include "securityinterface.h"
#include "securitymodule.h"

namespace HGMACHINE {
class SecurityInterface::Impl {
public:
    std::unique_ptr<SecurityModule> m_securityModule;

    bool initialize() {
        m_securityModule = std::make_unique<SecurityModule>();
        return m_securityModule->initialize("");
    }

    void shutdown() {
        if (m_securityModule) {
            m_securityModule->shutdown();
        }
    }

    bool isRightAuthority(const std::string &in){
        if (m_securityModule) {
            return m_securityModule->isRightAuthority(in);
        }
        return false;
    }
};

SecurityInterface::SecurityInterface() : m_impl(new Impl()) {
}

SecurityInterface::~SecurityInterface() {
    delete m_impl;
}

SecurityInterface& SecurityInterface::instance() {
    static SecurityInterface instance;
    return instance;
}

bool SecurityInterface::initialize() {
    return m_impl->initialize();
}

void SecurityInterface::shutdown() {
    m_impl->shutdown();
}


bool SecurityInterface::isRightAuthority(const std::string &in){
    return m_impl->isRightAuthority(in);

}
}