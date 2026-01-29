#ifndef ITASK_H
#define ITASK_H

#include "IfTask_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFTASK_EXPORT ITask {
public:
    virtual ~ITask() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> readTaskInfo(const std::string& taskSeqName) = 0;
    virtual void writeTaskRecord(bool coverFlag, const std::string& tableName, 
                               const std::vector<std::map<std::string, std::string>>& infoSS) = 0;
    virtual std::map<std::string, std::string> getTaskMap(int index, 
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
                                                         const std::string& detectStatus) = 0;
    virtual Task getTaskSFromMap(const std::map<std::string, std::string>& infoS) = 0;
    
    virtual std::vector<std::string> getAllTables(const std::string& dbName) = 0;
    virtual std::vector<std::string> getFlowNames() = 0;
    virtual std::string getMethodName(const std::string& flowName) = 0;
    
    virtual void setTaskRunFlag(const std::string& flag) = 0;
    virtual std::string getTaskRunFlag() = 0;
    virtual void resetTaskRunFlag() = 0;
    
    virtual void setTaskRunRecordDataDB(const std::string& dbName) = 0;
    virtual void insertTaskRunInfo(const std::string& startTime, const std::string& tableName) = 0;
};

}

#endif // ITASK_H
