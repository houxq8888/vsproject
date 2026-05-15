#include "CoreTask.h"
#include "rwTaskDb.h"
#include "DatabaseManagerAdapter.h"
#include "hgjson.h"

using namespace HGMACHINE;

class CoreTask::Impl {
public:
    Impl() {
        m_dbManager = &DatabaseManagerAdapter::instance();
    }
    
    ~Impl() {
    }
    
    DatabaseManagerAdapter* m_dbManager;
    RWTaskDb m_rwTaskDb;
};

CoreTask::CoreTask() : m_impl(new Impl()) {
}

CoreTask::~CoreTask() {
    delete m_impl;
}

bool CoreTask::initialize() {
    if (m_impl->m_dbManager && !m_impl->m_dbManager->IsConnected()) {
        std::string basePath = RWDb::readCurDirPath();
        RWDb::openDB(basePath);
    }
    return true;
}

void CoreTask::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreTask::readTaskInfo(const std::string& taskSeqName) {
    return m_impl->m_rwTaskDb.readTaskInfo(taskSeqName);
}

void CoreTask::writeTaskRecord(bool coverFlag, const std::string& tableName, 
                               const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_rwTaskDb.writeTaskRecord(coverFlag, tableName, info);
}

void CoreTask::insertTaskRunInfo(const std::string& startTime, const std::string& tableName) {
    m_impl->m_rwTaskDb.insertTaskRunInfo(startTime, tableName);
}

std::string CoreTask::getTaskRunFlag() {
    return m_impl->m_rwTaskDb.getTaskRunFlag();
}

std::vector<std::map<std::string, std::string>> CoreTask::getTaskRunInfo() {
    return m_impl->m_rwTaskDb.getTaskRunInfo();
}

void CoreTask::setTaskRunRecordDataDB(const std::string& dbName) {
    m_impl->m_rwTaskDb.setTaskRunRecordDataDB(dbName);
}

std::string CoreTask::getTaskRunRecordDataDB() {
    return m_impl->m_rwTaskDb.getTaskRunRecordDataDB();
}

void CoreTask::setTaskRunFlag(std::string flag) {
    m_impl->m_rwTaskDb.setTaskRunFlag(flag);
}

void CoreTask::resetTaskRunFlag() {
    m_impl->m_rwTaskDb.resetTaskRunFlag();
}

void CoreTask::resetTaskRunStatus(const std::string& tableName) {
    m_impl->m_rwTaskDb.resetTaskRunStatus(tableName);
}

std::map<std::string, std::string> CoreTask::getTaskMap(int index, 
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
    Task task;
    task.sampleName = sampleName;
    task.testFlow = testFlow;
    task.testMethod = testMethod;
    task.runStatus = runStatus;
    task.testChannel = testChannel;
    task.content = content;
    HGJson::deserialize(flowoftask, task.flowoftask);
    HGJson::deserialize(method, task.method);
    #ifdef ENABLE_CAS6
    task.blank = blank;
    task.sampleDetectStrategy = sampleDetectStrategy;
    task.circleNo = circleNo;
    task.batchNo = batchNo;
    task.serailNo = serialNo;
    task.detectStatus = detectStatus;
    #else
    task.sampleInput = sampleInput;
    task.getSampleMethod = getSampleMethod;
    task.targetElement = targetElement;
    task.unit = unit;
    task.standard1Condition = standard1Condition;
    task.standard2Condition = standard2Condition;
    task.standard1 = standard1;
    task.standard2 = standard2;
    task.workingMode = workingMode;
    task.interval = interval;
    task.getSamplePump = getSamplePump;
    #endif
    return m_impl->m_rwTaskDb.getTaskMap(index, task);
}

TaskInfo CoreTask::getTaskFromMap(const std::map<std::string, std::string>& info) {
    Task task = m_impl->m_rwTaskDb.getTaskFromMap(info);
    TaskInfo taskInfo;
    taskInfo.seq = task.seq;
    taskInfo.sampleSource = task.sampleSource;
    taskInfo.sampleMethod = task.sampleMethod;
    taskInfo.sampleName = task.sampleName;
    taskInfo.targetComponent = task.targetComponent;
    taskInfo.sampleTotal = task.sampleTotal;
    taskInfo.unit = task.unit;
    taskInfo.condition1 = task.condition1;
    taskInfo.value1 = task.value1;
    taskInfo.condition2 = task.condition2;
    taskInfo.value2 = task.value2;
    taskInfo.channel = task.channel;
    taskInfo.flow = task.flow;
    taskInfo.method = task.method;
    taskInfo.workMode = task.workMode;
    taskInfo.intervalTime = task.intervalTime;
    taskInfo.samplePipeline = task.samplePipeline;
    taskInfo.status = task.status;
    return taskInfo;
}

std::vector<std::string> CoreTask::getAllTables(const std::string& dbName) {
    return RWDb::getAllTables(dbName);
}

std::vector<std::string> CoreTask::getFlowNames() {
    return RWDb::getFlowNames();
}

std::string CoreTask::getMethodName(const std::string& flowName) {
    return RWDb::getMethodName(flowName);
}
