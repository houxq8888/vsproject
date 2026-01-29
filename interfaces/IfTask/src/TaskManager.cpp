#include "TaskManager.h"
#include "TaskAdapter.h"

using namespace HGMACHINE;

TaskManager::TaskManager() : m_task(new TaskAdapter()) {
}

TaskManager::~TaskManager() {
}

TaskManager& TaskManager::instance() {
    static TaskManager instance;
    return instance;
}

ITask& TaskManager::get() {
    return *m_task;
}
