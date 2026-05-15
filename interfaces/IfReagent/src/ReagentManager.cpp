#include "ReagentManager.h"
#include "ReagentAdapter.h"

using namespace HGMACHINE;

ReagentManager::ReagentManager() : m_reagent(new ReagentAdapter()) {
}

ReagentManager::~ReagentManager() {
}

ReagentManager& ReagentManager::instance() {
    static ReagentManager instance;
    return instance;
}

IReagent& ReagentManager::get() {
    return *m_reagent;
}
