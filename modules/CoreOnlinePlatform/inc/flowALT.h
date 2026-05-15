/*
**
**	版权:	
**
**	名称:	flowATL.h
**
**	作者:	xiaoqin.hou
**
**	日期:	2024/11/11
**
**	描述:
**
**	功能:	flow alt
**
**	备注:
**
*/
#ifndef FLOWALT_H
#define FLOWALT_H

#include "flow.h"
#include "method.h"
#include "hgonlineplatformmodule.h"
#include "structUpperPcToLowerPC.h"

namespace HGMACHINE
{
    class FlowALT : public FlowBase
    {
    public:
        FlowALT(std::string,std::string);
        virtual ~FlowALT(void);

        UpperPCToLowerPC getStepControlCode();
        int getStepsCount();
        std::string selfCheckDevice();
        void updateFlow();
        std::string getCurRunningStepName(int index);
        FlowOfTask getFlowOfTask() {return m_flowOfTask;};
        Method getMethodInfo() {return m_methodALT->getMethodInfo();};

    private:
        bool decodeCanIdAndIndex(const std::string& param,uint8_t &canId,uint8_t &index);

    private:
        FlowOfTask m_flowOfTask;
        MethodALT* m_methodALT;
        std::string m_name;
    };
}

#endif // FLOWALT_H
