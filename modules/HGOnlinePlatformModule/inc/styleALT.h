/*
**
**	版权:	
**
**	名称:	styleALT.h
**
**	作者:	侯 晓 琴
**
**	日期:	2024/11/11
**
**	描述:
**
**	功能:	式样类
**
**	备注:
**
*/

#ifndef STYLEALT_H
#define STYLEALT_H

#include "style.h"
#include <vector>
#include "hgonlineplatformmodule.h"
#include "taskALT.h"
#include "structUpperPcToLowerPC.h"

namespace HGMACHINE
{
    enum {
        TASK_RUN,
        TASK_INIT,
        TASK_OVER
    };
    class StyleALT : public Style
    {
    public:
        StyleALT(int type, std::string);
        ~StyleALT();
        TaskALT* GetTaskObj(int);

        int getTaskCount() {return m_taskArr.size();};
        void setCurRunningTaskIndex(int index);
        int getCurRunningTaskIndex() {return m_curRunningTaskIndex;};
        std::map<std::string,std::string> getCurRunningTaskInfo();
        UpperPCToLowerPC getControlCode();
        int getRunIndex(std::string &status);
        Task getTaskInfo(int);
        void setCurrentRunningStepIndex(int);
        void reset();
        virtual bool LoadDB();

        std::vector<std::string> getSelfCheckStatus();
        void updateFlowInfo();

    protected:
        std::vector<TaskALT*> m_taskArr;
        std::vector<int> m_taskRunStatus;
        // std::vector<Task> m_taskInfos;
        std::string m_runStatus;
        std::string m_preRunStatus;
        int m_curRunningTaskIndex;
    };
}

#endif // STYLEALT_H
