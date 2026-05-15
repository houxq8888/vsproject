#ifndef SYSTEMDATAMANAGER_H
#define SYSTEMDATAMANAGER_H

#include "ISystemData.h"
#include <memory>

namespace HGMACHINE {

class SystemDataManager {
public:
    static SystemDataManager& instance();
    
    ISystemData& get();
    
private:
    SystemDataManager();
    ~SystemDataManager();
    SystemDataManager(const SystemDataManager&) = delete;
    SystemDataManager& operator=(const SystemDataManager&) = delete;
    
    std::unique_ptr<ISystemData> m_systemData;
};

}

#endif
