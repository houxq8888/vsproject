/*
    @ 工位
    @file   taskALT.cpp
    @brief  Implementation file
    @author xiaoqin.hou
*/
#include "taskALT.h"
#include "HGMacroData.h"
#include "hgonlinerwDB.h"

namespace HGMACHINE
{
    TaskALT::TaskALT(int type, std::string flow,std::string method)
        : TaskBase(type)
    {
        m_flowName=flow;
        m_methodName=method;
        m_flowALT=new FlowALT(m_flowName,m_methodName);
        m_stepCount=m_flowALT->getStepsCount();
    }
    TaskALT::TaskALT(int type,const std::map<std::string,std::string>& _task)
        : TaskBase(type)
    {
        m_taskInfo=HGOnlineRWDB::getTaskSFromMap(_task);
        m_flowName=HGOnlineRWDB::decodeFlowDBName(_task.at("流程"));
        m_methodName=_task.at("方法");
        m_flowALT=new FlowALT(m_flowName,m_methodName);
        m_taskInfo.flowoftask=m_flowALT->getFlowOfTask();
        m_taskInfo.method=m_flowALT->getMethodInfo();
        m_stepCount=m_flowALT->getStepsCount();
    }
    TaskALT::~TaskALT(void)
    {
        SAFE_DELETE(m_flowALT);
        m_stepCount=0;
    }
    void TaskALT::LoadDB()
    {
    }
    void TaskALT::reset(){
    }
    void TaskALT::updateFlowInfo(){
        m_flowALT->updateFlow();
    }
    std::string TaskALT::getCurRunningStepName(int index){
        return m_flowALT->getCurRunningStepName(index);
    }
    std::string TaskALT::selfCheck(){
        return m_flowALT->selfCheckDevice();
    }
    int TaskALT::getTaskControlCode(UpperPCToLowerPC &upperToLower){ 
        upperToLower=m_flowALT->getStepControlCode();
        return 0;
    }
}
