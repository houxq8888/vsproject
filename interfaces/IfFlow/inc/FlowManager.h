#ifndef FLOWMANAGER_H
#define FLOWMANAGER_H

#include "IFlow.h"
#include <memory>

namespace HGMACHINE {

class FlowManager {
public:
    static FlowManager& instance();
    
    IFlow& get();
    
private:
    FlowManager();
    ~FlowManager();
    
    FlowManager(const FlowManager&) = delete;
    FlowManager& operator=(const FlowManager&) = delete;
    
    std::unique_ptr<IFlow> m_flow;
};

}

#endif // FLOWMANAGER_H
