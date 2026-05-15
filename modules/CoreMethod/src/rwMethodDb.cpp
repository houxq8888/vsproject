#include "rwMethodDb.h"
#include "rwDb.h"

namespace HGMACHINE {
    
    Method RWMethodDb::readMethodParam(std::string dbName){
        Method method;
        std::vector<std::map<std::string,std::string>> fillContent;
        std::map<std::string,std::string> info;
        info["序号"]="";
        info["参数"]="";
        fillContent=RWDb::readRecord(dbName,info);
        if (fillContent.size()>0){
            method.param=fillContent[0]["参数"];
        }
        return method;
    }
    std::vector<std::string> RWMethodDb::getMethodNames(){
        std::vector<std::map<std::string,std::string>> fillContent=readMethodInfo();
        std::vector<std::string> names;
        for (int i=0;i<int(fillContent.size());i++){    
            names.push_back(fillContent[i]["序号"]+"_"+fillContent[i]["名称"]);
        }
        return names;
    }

    std::string RWMethodDb::getMethodName(const std::string &flowName){
        std::string methodName="";
        // first find flow
        std::vector<std::map<std::string,std::string>> fillContent=readFlowInfo();
        std::vector<std::string> names;
        std::string dbName="";
        for (int i=0;i<int(fillContent.size());i++){
            std::string tempName=fillContent[i]["序号"]+"_"+fillContent[i]["名称"];
            if (tempName==flowName){
                dbName=fillContent[i]["DBName"];
                break;
            }
        }
        if (dbName=="") return methodName;
        FlowOfTask flowOfTask=RWMethodDb::readFlowOfTask(dbName);
        for (int i=0;i<int(flowOfTask.steps.size());i++){
            if (flowOfTask.steps[i].name=="测定"){
                std::map<std::string, std::string> wparam = getParamMap(flowOfTask.steps[i].param);
                methodName+="["+wparam["测试方法"]+"]";
            }
        }
        return methodName; 
    }

    std::vector<std::map<std::string,std::string>> RWMethodDb::getMethodParamMap(Method method){
        std::vector<std::map<std::string,std::string>> infoS;
        std::map<std::string,std::string> info;
        info["参数"]=method.param;
        info["序号"]=std::to_string(1);
        infoS.push_back(info);
        return infoS;
    }
    std::map<std::string,std::string> RWMethodDb::getMethodMap(int index,Method method){
        std::map<std::string,std::string> infoS;
        infoS["序号"]=method.indexStr;
        infoS["名称"]=method.name;
        infoS["类型"]=method.type;
        infoS["创建时间"]=method.createTime;
        infoS["DBName"]=method.dbName;
        infoS["参数"]=method.param;
        return infoS;
    }
    void RWMethodDb::writeMethodManageRecord(const std::map<std::string, std::string> &infoS)
    {
        dbOpera.writeRecord(METHODMANAGENAME, "序号", infoS);
        dbOpera.sortTable(METHODMANAGENAME,"序号");
    }

    std::string RWMethodDb::decodeMethodDBName(const std::string &methodName){
        std::vector<std::map<std::string, std::string>> fillContent = readMethodInfo();
        std::string dbName = "";
        for (int i = 0; i < int(fillContent.size()); i++)
        {
            std::string tempName = fillContent[i]["序号"] + "_" + fillContent[i]["名称"];
            if (tempName == methodName)
            {
                dbName = fillContent[i]["DBName"];
                break;
            }
        }
        return dbName;
    }
    std::vector<std::map<std::string, std::string>> RWMethodDb::readMethodInfo()
    {
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(methodInfoName.size()); i++)
        {
            infoS[methodInfoName[i]] = "";
        }
        fillContent =readRecord(METHODMANAGENAME, infoS);
        return fillContent;
    }
    void RWMethodDb::clearMethodManageRecord(){
        clearDB(METHODMANAGENAME);
    }

}
