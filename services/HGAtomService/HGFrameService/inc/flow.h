/*
**
**	版权:	
**
**	名称:	flow.h
**
**	作者:	xiaoqin.hou
**
**	日期:	2024/11/11
**
**	描述:
**
**	功能:	flow base class
**
**	备注:
**
*/
#ifndef FLOW_H
#define FLOW_H

#include <string>

namespace HGMACHINE
{
    using namespace std;

    class FlowBase
    {
    public:
        FlowBase();
        virtual ~FlowBase(void);
    };
}
#endif // Flow_H
