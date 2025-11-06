/*
**
**	版权:	
**
**	名称:	task.h
**
**	作者:	xiaoqin.hou
**
**	日期:	2024/11/1
**
**	描述:
**
**	功能:	task base class 
**
**	备注:
**
*/
#ifndef TASK_H
#define TASK_H

#include <string>
#include "hgxml.h"

namespace HGMACHINE
{
    class TaskBase
    {
    public:
        TaskBase(int);
        virtual ~TaskBase(void);
        virtual void LoadDB();

    protected:
    private:
    };
}

#endif // TASK_H
