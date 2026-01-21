/*
**
**	版权:	
**
**	名称:	taskALT.h
**
**	作者:	xiaoqin.hou
**
**	日期:	2024/11/1
**
**	描述:
**
**	功能: task alt
**
**	备注:
**
*/
#ifndef TASKALT_H
#define TASKALT_H

#include "task.h"
#include "flowALT.h"

namespace HGMACHINE
{
    class TaskALT : public TaskBase
    {
    public:
        TaskALT(int, std::string,std::string);
        TaskALT(int,const std::map<std::string,std::string> &);
        virtual ~TaskALT(void);
        virtual void LoadDB();
        int getTaskControlCode(UpperPCToLowerPC &upperToLower);
        void reset();
        std::string selfCheck();
        void updateFlowInfo();
        std::string getCurRunningStepName(int);
        Task getTaskInfo() {return m_taskInfo;};

    private:
    private:
        std::string m_flowName;
        std::string m_methodName;
        FlowALT *m_flowALT;
        Task m_taskInfo;
        int m_stepCount;

        std::string m_actionName;
    };
}

#endif // TASKALT_H
