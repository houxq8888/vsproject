/*
    @ 
    @file   method.cpp
    @brief  Implementation file
    @author xiaoqin.hou
*/
#include "method.h"
#include "hgonlinerwDB.h"


namespace HGMACHINE{
    MethodALT::MethodALT(std::string name){
        m_methodName=name;
        std::vector<std::map<std::string,std::string>> methodInfos=HGOnlineRWDB::readMethodInfo();
        for (int i=0;i<int(methodInfos.size());i++){
            std::string tempName="["+methodInfos[i]["序号"] + "_" + methodInfos[i]["名称"]+"]";
            if (tempName==m_methodName){
                m_methodInfo.createTime=methodInfos[i]["创建时间"];
                m_methodInfo.dbName=methodInfos[i]["DBName"];
                m_methodInfo.indexStr=methodInfos[i]["序号"];
                m_methodInfo.name=methodInfos[i]["名称"];
                m_methodInfo.type=methodInfos[i]["类型"];
                m_methodInfo.param=methodInfos[i]["参数"];
                break;
            }
        }
    }
    MethodALT::~MethodALT(){
        m_methodInfo.Clear();
    }
}