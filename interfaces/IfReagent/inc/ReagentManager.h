#ifndef REAGENTMANAGER_H
#define REAGENTMANAGER_H

#include "IReagent.h"
#include <memory>

namespace HGMACHINE {

class ReagentManager {
public:
    static ReagentManager& instance();
    
    IReagent& get();
    
private:
    ReagentManager();
    ~ReagentManager();
    
    ReagentManager(const ReagentManager&) = delete;
    ReagentManager& operator=(const ReagentManager&) = delete;
    
    std::unique_ptr<IReagent> m_reagent;
};

}

#endif // REAGENTMANAGER_H
