#ifndef CORETASK_H
#define CORETASK_H

#include "CoreTask_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class CORETASK_EXPORT CoreTask {
public:
    CoreTask();
    ~CoreTask();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> readTaskInfo(const std::string& taskSeqName);
    void writeTaskRecord(bool coverFlag, const std::string& tableName, 
                       const std::vector<std::map<std::string, std::string>>& infoSS);
    std::map<std::string, std::string> getTaskMap(int index, 
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
                                                   const std::string& detectStatus);
    Task getTaskSFromMap(const std::map<std::string, std::string>& infoS);
    
    std::vector<std::string> getAllTables(const std::string& dbName);
    std::vector<std::string> getFlowNames();
    std::string getMethodName(const std::string& flowName);
    
    void setTaskRunFlag(const std::string& flag);
    std::string getTaskRunFlag();
    void resetTaskRunFlag();
    
    void setTaskRunRecordDataDB(const std::string& dbName);
    void insertTaskRunInfo(const std::string& startTime, const std::string& tableName);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CORETASK_H
