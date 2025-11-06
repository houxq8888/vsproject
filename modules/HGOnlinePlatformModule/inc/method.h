/*
**
**	版权:	
**
**	名称:	method.h
**
**	作者:	xiaoqin.hou
**
**	日期:	2024/11/11
**
**	描述:
**
**	功能:	method alt
**
**	备注:
**
*/
#ifndef __METHOD_H__
#define __METHOD_H__

#include "hgonlineplatformmodule.h"

namespace HGMACHINE{
    class MethodALT{
        public:
            MethodALT(std::string);
            virtual ~MethodALT();
            Method getMethodInfo() {return m_methodInfo;};
            
        private:
            Method m_methodInfo;
            std::string m_methodName;
    };
}

#endif