#include "TaskAdapter.h"
#include "CoreTask/CoreTask.h"

using namespace HGMACHINE;

class TaskAdapter::Impl {
public:
    CoreTask m_coreTask;
};

TaskAdapter::TaskAdapter() : m_impl(new Impl()) {
}

TaskAdapter::~TaskAdapter() {
    delete m_impl;
}

bool TaskAdapter::initialize() {
    return m_impl->m_coreTask.initialize();
}

void TaskAdapter::shutdown() {
    m_impl->m_coreTask.shutdown();
}

std::vector<std::map<std::string, std::string>> TaskAdapter::readTaskInfo(const std::string& taskSeqName) {
    return m_impl->m_coreTask.readTaskInfo(taskSeqName);
}

void TaskAdapter::writeTaskRecord(bool coverFlag, const std::string& tableName, 
                                  const std::vector<std::map<std::string, std::string>>& infoSS) {
    m_impl->m_coreTask.writeTaskRecord(coverFlag, tableName, infoSS);
}

std::map<std::string, std::string> TaskAdapter::getTaskMap(int index, 
                                                             const std::string& sampleName,
                                                             const std::string& testFlow,
                                                             const std::string& testMethod,
                                                             const std::string& runStatus,
                                                             int testChannel,
                                                             double content,
                                                             const std::string& sampleInput,
                                                             const std::string& getSampleMethod,
                                                             const std::string& targetElement,
                                                             const std::string& unit,
                                                             const std::string& standard1Condition,
                                                             const std::string& standard2Condition,
                                                             double standard1,
                                                             double standard2,
                                                             const std::string& getSamplePump,
                                                             int workingMode,
                                                             int interval,
                                                             const std::string& flowoftask,
                                                             const std::string& method,
                                                             const std::string& blank,
                                                             int sampleDetectStrategy,
                                                             int circleNo,
                                                             const std::string& batchNo,
                                                             const std::string& serialNo,
                                                             const std::string& detectStatus) {
    return m_impl->m_coreTask.getTaskMap(index, sampleName, testFlow, testMethod, runStatus, 
                                         testChannel, content, sampleInput, getSampleMethod, 
                                         targetElement, unit, standard1Condition, standard2Condition,
                                         standard1, standard2, getSamplePump, workingMode, interval,
                                         flowoftask, method, blank, sampleDetectStrategy,
                                         circleNo, batchNo, serialNo, detectStatus);
}

Task TaskAdapter::getTaskSFromMap(const std::map<std::string, std::string>& infoS) {
    return m_impl->m_coreTask.getTaskSFromMap(infoS);
}

std::vector<std::string> TaskAdapter::getAllTables(const std::string& dbName) {
    return m_impl->m_coreTask.getAllTables(dbName);
}

std::vector<std::string> TaskAdapter::getFlowNames() {
    return m_impl->m_coreTask.getFlowNames();
}

std::string TaskAdapter::getMethodName(const std::string& flowName) {
    return m_impl->m_coreTask.getMethodName(flowName);
}

void TaskAdapter::setTaskRunFlag(const std::string& flag) {
    m_impl->m_coreTask.setTaskRunFlag(flag);
}

std::string TaskAdapter::getTaskRunFlag() {
    return m_impl->m_coreTask.getTaskRunFlag();
}

void TaskAdapter::resetTaskRunFlag() {
    m_impl->m_coreTask.resetTaskRunFlag();
}

void TaskAdapter::setTaskRunRecordDataDB(const std::string& dbName) {
    m_impl->m_coreTask.setTaskRunRecordDataDB(dbName);
}

void TaskAdapter::insertTaskRunInfo(const std::string& startTime, const std::string& tableName) {
    m_impl->m_coreTask.insertTaskRunInfo(startTime, tableName);
}
