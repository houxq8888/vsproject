#include "FlowManager.h"
#include "FlowAdapter.h"

using namespace HGMACHINE;

FlowManager::FlowManager() : m_flow(new FlowAdapter()) {
}

FlowManager::~FlowManager() {
}

FlowManager& FlowManager::instance() {
    static FlowManager instance;
    return instance;
}

IFlow& FlowManager::get() {
    return *m_flow;
}
