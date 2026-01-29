#include "CoreTask.h"
#include "rwDb.h"
#include "hgjson.h"

using namespace HGMACHINE;

class CoreTask::Impl {
public:
    Impl() {
    }
    
    ~Impl() {
    }
};

CoreTask::CoreTask() : m_impl(new Impl()) {
}

CoreTask::~CoreTask() {
    delete m_impl;
}

bool CoreTask::initialize() {
    return true;
}

void CoreTask::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreTask::readTaskInfo(const std::string& taskSeqName) {
    return RWDb::readTaskInfo(taskSeqName);
}

void CoreTask::writeTaskRecord(bool coverFlag, const std::string& tableName, 
                               const std::vector<std::map<std::string, std::string>>& infoSS) {
    RWDb::writeTaskRecord(coverFlag, tableName, infoSS);
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
    return RWDb::getTaskMap(index, task);
}

Task CoreTask::getTaskSFromMap(const std::map<std::string, std::string>& infoS) {
    return RWDb::getTaskSFromMap(infoS);
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

void CoreTask::setTaskRunFlag(const std::string& flag) {
    RWDb::setTaskRunFlag(flag);
}

std::string CoreTask::getTaskRunFlag() {
    return RWDb::getTaskRunFlag();
}

void CoreTask::resetTaskRunFlag() {
    RWDb::resetTaskRunFlag();
}

void CoreTask::setTaskRunRecordDataDB(const std::string& dbName) {
    RWDb::setTaskRunRecordDataDB(dbName);
}

void CoreTask::insertTaskRunInfo(const std::string& startTime, const std::string& tableName) {
    RWDb::insertTaskRunInfo(startTime, tableName);
}
