#include "SystemDataManager.h"
#include "SystemDataAdapter.h"

namespace HGMACHINE {

SystemDataManager::SystemDataManager() {
    m_systemData = std::make_unique<SystemDataAdapter>();
    m_systemData->initialize();
}

SystemDataManager::~SystemDataManager() {
    if (m_systemData) {
        m_systemData->shutdown();
    }
}

SystemDataManager& SystemDataManager::instance() {
    static SystemDataManager instance;
    return instance;
}

ISystemData& SystemDataManager::get() {
    return *m_systemData;
}

}
