#include "securitymodule.h"
#include "hgsecurity.h"
#include <fstream>
#include <sstream>

namespace HGMACHINE {

class SecurityModule::Impl {
public:
    bool m_dbInitialized;
    
    Impl() 
        : m_dbInitialized(false)
    {
    }

    bool initialize(const std::string& dbPath) {
        if (!dbPath.empty()) {
            m_dbInitialized = true;
        }
        return true;
    }

    void shutdown() {
        if (m_dbInitialized) {
            m_dbInitialized = false;
        }
    }

    bool isRightAuthority(const std::string &in) {
        return HGMACHINE::isRightAuthority(in);
    }
};

SecurityModule::SecurityModule() : m_impl(new Impl()) {
}

SecurityModule::~SecurityModule() {
    delete m_impl;
}

bool SecurityModule::initialize(const std::string& dbPath) {
    return m_impl->initialize(dbPath);
}

void SecurityModule::shutdown() {
    m_impl->shutdown();
}

bool SecurityModule::isRightAuthority(const std::string &in) {
    return m_impl->isRightAuthority(in);
}


}