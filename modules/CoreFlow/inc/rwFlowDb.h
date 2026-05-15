#ifndef RWFLOWDB_H
#define RWFLOWDB_H

#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE {
    
    typedef struct tagStepOfFlow {
        int index;
        std::string name;
        std::string param;
        std::string nameOfSameStep;
        int indexOfSameStep;
        
        tagStepOfFlow() {
            Clear();
        }
        
        void Clear() {
            index = 0;
            name = "";
            param = "";
            nameOfSameStep = "";
            indexOfSameStep = 0;
        }
        
        bool operator==(const tagStepOfFlow& obj) const {
            return index == obj.index &&
                   name == obj.name &&
                   param == obj.param &&
                   nameOfSameStep == obj.nameOfSameStep &&
                   indexOfSameStep == obj.indexOfSameStep;
        }
        
        tagStepOfFlow(const tagStepOfFlow& obj) {
            Copy(obj);
        }
        
        void Copy(const tagStepOfFlow& obj) {
            index = obj.index;
            name = obj.name;
            param = obj.param;
            nameOfSameStep = obj.nameOfSameStep;
            indexOfSameStep = obj.indexOfSameStep;
        }
        
        tagStepOfFlow& operator=(const tagStepOfFlow& obj) {
            Copy(obj);
            return *this;
        }
        
        bool operator!=(const tagStepOfFlow& obj) const {
            return !(*this == obj);
        }
        
        tagStepOfFlow* Clone() const {
            return new tagStepOfFlow(*this);
        }
    } StepOfFlow, *PStepOfFlow;
    
    typedef struct tagFlowOfTask {
        std::string indexStr;
        std::string name;
        std::string type;
        std::string createTime;
        std::string dbName;
        std::vector<StepOfFlow> steps;
        
        tagFlowOfTask() {
            Clear();
        }
        
        void Clear() {
            indexStr = "";
            name = "";
            type = "";
            createTime = "";
            dbName = "";
            steps.clear();
        }
        
        bool operator==(const tagFlowOfTask& obj) const {
            return indexStr == obj.indexStr &&
                   name == obj.name &&
                   type == obj.type &&
                   createTime == obj.createTime &&
                   dbName == obj.dbName &&
                   steps == obj.steps;
        }
        
        tagFlowOfTask(const tagFlowOfTask& obj) {
            Copy(obj);
        }
        
        void Copy(const tagFlowOfTask& obj) {
            indexStr = obj.indexStr;
            name = obj.name;
            type = obj.type;
            createTime = obj.createTime;
            dbName = obj.dbName;
            steps = obj.steps;
        }
        
        tagFlowOfTask& operator=(const tagFlowOfTask& obj) {
            Copy(obj);
            return *this;
        }
        
        bool operator!=(const tagFlowOfTask& obj) const {
            return !(*this == obj);
        }
        
        tagFlowOfTask* Clone() const {
            return new tagFlowOfTask(*this);
        }
    } FlowOfTask, *PFlowOfTask;

    #define FLOWMANAGENAME "FlowInfo"
    #define FlowDBName "Flow"

    class RWFlowDb {
    public:
        RWFlowDb();
        ~RWFlowDb();

        std::vector<std::map<std::string, std::string>> getFlowStepMap(FlowOfTask flow);
        std::map<std::string, std::string> getFlowMap(int index, FlowOfTask flow);
        std::vector<std::map<std::string, std::string>> readFlowInfo();
        FlowOfTask readFlowOfTask(std::string dbName);
        std::vector<std::string> getFlowNames();
        void clearFlowManageRecord();
        std::string decodeFlowDBName(const std::string& flowName);
        void writeFlowManageRecord(const std::map<std::string, std::string>& infoS);
        void writeFlowRecord(std::string dbName, bool coverFlag, const std::vector<std::map<std::string, std::string>>& infoSS);
        
    protected:
    };
}

#ifdef __cplusplus
}
#endif

#endif // RWFLOWDB_H
