/*
    @file   styleALT.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "styleALT.h"
#include "HGMacroData.h"
#include "hgcommonutility.h"
#include "hgxml.h"
#include <sstream>
#include "hgonlinerwDB.h"
#include <vector>
#include <map>
#include "hglog4cplus.h"

namespace HGMACHINE {
        StyleALT::StyleALT(int type, std::string dbName)
            :Style(type,dbName),
            m_curRunningTaskIndex(-1)
        {
            std::vector<std::map<std::string,std::string>> taskInfo;
            if (type == LOAD) {
                taskInfo=HGOnlineRWDB::readTaskInfo(HgOnlinePlatformModule::getTaskSeqName());
                for (auto task : taskInfo){
                    // m_taskArr.push_back(new TaskALT(LOAD,HGOnlineRWDB::decodeFlowDBName(task["流程"]),
                    //     HGOnlineRWDB::decodeMethodDBName(task["方法"])));
                    for (int i=0;i<int(taskInfoName.size());i++){
                        printf("%s: %s\n",taskInfoName[i].c_str(),task.at(taskInfoName[i]).c_str());
                    }
                    m_taskArr.push_back(new TaskALT(LOAD,task));
                    m_taskRunStatus.push_back(TASK_INIT); // no run
                }
            } 
            m_runStatus="";
        }
        StyleALT::~StyleALT()
        {
            for (int i = 0; i < int(m_taskArr.size()); i++)
                SAFE_DELETE(m_taskArr[i]);
            m_taskArr.clear();
            m_taskRunStatus.clear();
            m_runStatus="";
        }
        void StyleALT::updateFlowInfo(){
            for (int i=0;i<int(m_taskArr.size());i++){
                m_taskArr[i]->updateFlowInfo();
            }
        }
        Task StyleALT::getTaskInfo(int index){
            return m_taskArr[index]->getTaskInfo();
        }
        std::map<std::string,std::string> StyleALT::getCurRunningTaskInfo(){
            if (m_curRunningTaskIndex==-1) return std::map<std::string,std::string>();
            Task task=getTaskInfo(m_curRunningTaskIndex);
            std::map<std::string,std::string> taskInfo;
            taskInfo["quantity"]=std::to_string(task.content);
            taskInfo["method"]=task.testMethod;
            taskInfo["param"]=task.method.param;
            #ifdef ENABLE_CAS6
            taskInfo["circleNo"]=std::to_string(task.circleNo);
            #endif
            return taskInfo;
        }
        void StyleALT::setCurRunningTaskIndex(int index){
            m_curRunningTaskIndex=index;
        }
        void StyleALT::reset(){
            for (int index=0;index<int(m_taskRunStatus.size());index++){
                m_taskRunStatus[index]=TASK_INIT;
                m_taskArr[index]->reset();
            }
        }
        UpperPCToLowerPC StyleALT::getControlCode(){
            m_preRunStatus=m_runStatus;
            UpperPCToLowerPC upperToLower=initUpperPCToLowerPC();
            upperToLower.CurPhase=RUN_FLOW;
            int cnt=0;
            for (auto status : m_taskRunStatus){
                if (status==TASK_INIT){
                    m_taskArr[cnt]->getTaskControlCode(upperToLower);
                    m_taskRunStatus[cnt]=TASK_RUN;
                    break;
                } else if (status==TASK_OVER){
                    cnt++;
                    continue;
                }
            }
            return upperToLower;
        }
        void StyleALT::setCurrentRunningStepIndex(int index){
            for(int i=0;i<int(m_taskRunStatus.size());i++){
                if (m_taskRunStatus[i]!=TASK_OVER){
                    m_runStatus=m_taskArr[i]->getCurRunningStepName(index);
                    if (m_runStatus=="已完成") m_taskRunStatus[i]=TASK_OVER;
                    break;
                } 
            }
        }
        int StyleALT::getRunIndex(std::string &status){
            status="";
            for(int i=0;i<int(m_taskRunStatus.size());i++){
                if (m_taskRunStatus[i]==TASK_INIT){
                    status="初始化";
                    return i;
                } else if (m_taskRunStatus[i]==TASK_RUN){
                    status=m_runStatus;
                    return i;
                } else if (m_taskRunStatus[i]==TASK_OVER) {
                    continue;
                }
            }
            return -1;
        }
        std::vector<std::string> StyleALT::getSelfCheckStatus(){
            std::vector<std::string> result;
            std::ostringstream logtext;
            for (int i = 0; i < int(m_taskArr.size()); i++){
                result.push_back(m_taskArr[i]->selfCheck());

                logtext<<"device status:"<<result[i];
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
            }
            return result;
        }
        bool StyleALT::LoadDB()
        {
            
            return true;
        }

      
}

