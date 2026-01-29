#ifndef SECURITYMODULE_H
#define SECURITYMODULE_H

#include "SecurityModule_global.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace HGMACHINE {

class SECURITYMODULE_EXPORT SecurityModule
{
public:
    SecurityModule();
    ~SecurityModule();
    
    bool initialize(const std::string& dbPath);
    void shutdown();
    
    bool isRightAuthority(const std::string &in);

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // SECURITYMODULE_H
