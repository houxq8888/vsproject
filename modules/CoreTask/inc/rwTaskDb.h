#ifndef RWTASKDB_H
#define RWTASKDB_H

#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
    
    typedef struct tagTask{
        std::string sampleName;
        std::string testFlow;
        std::string testMethod;
        FlowOfTask flowoftask;
        Method method;
        std::string runStatus;
        int testChannel;
        double content; // hanliang
        tagTask(){
            Clear();
        }
        tagTask(const tagTask& obj){
            Copy(obj);
        }

        #ifdef ENABLE_CAS6
        int circleNo;
        std::string batchNo;
        std::string serailNo;
        std::string detectStatus;
        std::string blank;
        int sampleDetectStrategy;
        void Clear(){
            blank="";
            circleNo=1;
            batchNo="";
            sampleName="";
            serailNo="";
            detectStatus="";
            content=0.0;
            testFlow="";
            testMethod="";
            flowoftask.Clear();
            method.Clear();
            runStatus="";
            testChannel=-1;
            sampleDetectStrategy=-1;
        }
        
        void Copy(const tagTask& obj){
            blank=obj.blank;
            circleNo=obj.circleNo;
            batchNo=obj.batchNo;
            sampleName=obj.sampleName;
            serailNo=obj.serailNo;
            detectStatus=obj.detectStatus;
            content=obj.content;
            testFlow=obj.testFlow;
            testMethod=obj.testMethod;
            flowoftask=obj.flowoftask;
            method=obj.method;
            runStatus=obj.runStatus;
            testChannel=obj.testChannel;
            sampleDetectStrategy=obj.sampleDetectStrategy;
        }
        
        bool operator==(const tagTask& obj) const{
            bool ret=false;
            if (batchNo==obj.batchNo &&
                blank==obj.blank &&
                circleNo==obj.circleNo &&
                sampleName==obj.sampleName &&
                serailNo==obj.serailNo &&
                detectStatus==obj.detectStatus &&
                content==obj.content &&
                testFlow==obj.testFlow && 
                testMethod==obj.testMethod && 
                flowoftask==obj.flowoftask && 
                method==obj.method &&
                testChannel==obj.testChannel &&
                sampleDetectStrategy==obj.sampleDetectStrategy &&
                runStatus==obj.runStatus)
            {
                ret=true;
            }
            return ret;
        }
        #else
        std::string sampleInput;
        std::string getSampleMethod;
        std::string targetElement;
        std::string unit; 
        std::string standard1Condition;
        std::string standard2Condition;
        double standard1;
        double standard2;
        std::string getSamplePump;
        int workingMode;
        int interval;
        
        void Clear(){
            sampleInput="";
            sampleName="";
            getSampleMethod="";
            targetElement="";
            content=0.0;
            unit="";
            standard1Condition="";
            standard2Condition="";
            standard1=0.0;
            standard2=0.0;
            testChannel=-1;
            getSamplePump="";
            testFlow="";
            testMethod="";
            workingMode=-1;
            interval=-1;
            flowoftask.Clear();
            method.Clear();
            runStatus="";
        }
        
        void Copy(const tagTask& obj){
            sampleInput=obj.sampleInput;
            sampleName=obj.sampleName;
            getSampleMethod=obj.getSampleMethod;
            targetElement=obj.targetElement;
            content=obj.content;
            unit=obj.unit;
            standard1Condition=obj.standard1Condition;
            standard2Condition=obj.standard2Condition;
            standard1=obj.standard1;
            standard2=obj.standard2;
            testChannel=obj.testChannel;
            getSamplePump=obj.getSamplePump;
            testFlow=obj.testFlow;
            testMethod=obj.testMethod;
            workingMode=obj.workingMode;
            interval=obj.interval;
            flowoftask=obj.flowoftask;
            method=obj.method;
            runStatus=obj.runStatus;
        }
        bool operator==(const tagTask& obj) const{
            bool ret=false;
            if (sampleInput==obj.sampleInput &&
                sampleName==obj.sampleName &&
                getSampleMethod==obj.getSampleMethod &&
                targetElement==obj.targetElement &&
                content==obj.content &&
                getSamplePump==obj.getSamplePump &&
                unit==obj.unit && 
                standard1Condition==obj.standard1Condition && 
                standard2Condition==obj.standard2Condition && 
                standard1==obj.standard1 && 
                standard2==obj.standard2 && 
                testChannel==obj.testChannel && 
                testFlow==obj.testFlow && 
                testMethod==obj.testMethod && 
                workingMode==obj.workingMode && 
                interval==obj.interval && 
                flowoftask==obj.flowoftask && 
                method==obj.method &&
                runStatus==obj.runStatus)
            {
                ret=true;
            }
            return ret;
        }
        #endif
        tagTask& operator=(const tagTask& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagTask& obj)const
        {
            return (!(*this==obj));
        }
        tagTask* Clone() const
        {
            return (new tagTask(*this));
        }
    } Task,*PTask;

    #define TaskSequenceDBName "TaskSequence"
    #define TASKRUNRECORDINFODBNAME "TaskRunRecordInfo"
    #define TASKRUNSTATUSNAME "TaskRunStatusInfo"

    class RWTaskDb {
        public:
            RWTaskDb();
            ~RWTaskDb();

           
            //-------------------------task-------------------------------------//
            static std::map<std::string,std::string> getTaskMap(int index,Task taskInfo);
            static Task getTaskSFromMap(std::map<std::string, std::string> infoS);
            static std::vector<std::map<std::string, std::string>> readTaskInfo(const std::string& taskSeqName);
            static void writeTaskRecord(bool coverFlag,const std::string& tableName,const std::vector<std::map<std::string,std::string>> &infoSS); 
            static void insertTaskRunInfo(const std::string &startTime,const std::string &tableName);
            static std::string getTaskRunFlag();
            static std::vector<std::map<std::string,std::string>> getTaskRunInfo();
            static void setTaskRunRecordDataDB(const std::string &dbName);
            static std::string getTaskRunRecordDataDB();
            static void setTaskRunFlag(std::string);
            static void resetTaskRunFlag();
            static void resetTaskRunStatus(const std::string &tableName);


        protected:
    };
}
#ifdef __cplusplus
}
#endif
#endif // RWTASKDB_H
