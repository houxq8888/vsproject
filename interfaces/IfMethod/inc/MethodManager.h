#ifndef METHODMANAGER_H
#define METHODMANAGER_H

#include "IMethod.h"
#include <memory>

namespace HGMACHINE {

class MethodManager {
public:
    static MethodManager& instance();
    
    IMethod& get();
    
private:
    MethodManager();
    ~MethodManager();
    
    MethodManager(const MethodManager&) = delete;
    MethodManager& operator=(const MethodManager&) = delete;
    
    std::unique_ptr<IMethod> m_method;
};

}

#endif // METHODMANAGER_H
