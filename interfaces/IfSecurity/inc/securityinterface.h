#ifndef SECURITYINTERFACE_H
#define SECURITYINTERFACE_H

#include "SecurityInterface_global.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

namespace HGMACHINE {

class SECURITYINTERFACE_EXPORT SecurityInterface
{
public:
    SecurityInterface();
    ~SecurityInterface();
    
    bool initialize();
    void shutdown();
    
    bool isRightAuthority(const std::string &in);
    
    

    static SecurityInterface& instance();

private:
    class Impl;
    Impl* m_impl;
};

}

#define SECURITY_IF HGMACHINE::SecurityInterface::instance()

#endif // SECURITYINTERFACE_H
