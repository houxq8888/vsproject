#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "ITask.h"
#include <memory>

namespace HGMACHINE {

class TaskManager {
public:
    static TaskManager& instance();
    
    ITask& get();
    
private:
    TaskManager();
    ~TaskManager();
    
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    
    std::unique_ptr<ITask> m_task;
};

}

#endif // TASKMANAGER_H
