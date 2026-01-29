#include "rwFlowDb.h"


namespace HGMACHINE {

    std::vector<std::map<std::string,std::string>> RWFlowDb::getFlowStepMap(FlowOfTask flow){
        std::vector<std::map<std::string,std::string>> infoS;
        for (auto step : flow.steps){
            std::map<std::string,std::string> info;
            info["序号"]=std::to_string(step.index);
            info["名称"]=step.name;
            info["参数"]=step.param;
            infoS.push_back(info);
        }
        return infoS;
    }

    FlowOfTask RWFlowDb::readFlowOfTask(std::string dbName)
    {
        FlowOfTask flow;
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> info;
        info["序号"] = "";
        info["名称"] = "";
        info["参数"] = "";
        fillContent = readRecord(dbName, info);
        for (auto content : fillContent)
        {
            StepOfFlow step;
            step.index = std::stoi(content["序号"]);
            step.name = content["名称"];
            step.param = content["参数"];
            flow.steps.push_back(step);
        }
        return flow;
    }
    void RWFlowDb::writeFlowManageRecord(const std::map<std::string, std::string> &infoS)
    {
        dbOpera.writeRecord(FLOWMANAGEDBNAME, "序号", infoS);
        dbOpera.sortTable(FLOWMANAGEDBNAME,"序号");
    }
    std::map<std::string,std::string> RWFlowDb::getFlowMap(int index,FlowOfTask flow){
        std::map<std::string,std::string> infoS;
        infoS["序号"]=flow.indexStr;
        infoS["名称"]=flow.name;
        infoS["类型"]=flow.type;
        infoS["创建时间"]=flow.createTime;
        infoS["DBName"]=flow.dbName;
        return infoS;
    }
    std::vector<std::map<std::string, std::string>> RWFlowDb::readFlowInfo()
    {
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(flowInfoName.size()); i++)
        {
            infoS[flowInfoName[i]] = "";
        }
        fillContent = readRecord(FLOWMANAGEDBNAME, infoS);

        return fillContent;
    }
    std::vector<std::string> RWFlowDb::getFlowNames(){
        std::vector<std::map<std::string,std::string>> fillContent=readFlowInfo();
        std::vector<std::string> names;
        for (int i=0;i<int(fillContent.size());i++){    
            names.push_back(fillContent[i]["序号"]+"_"+fillContent[i]["名称"]);
        }
        return names;
    }
    void RWFlowDb::clearFlowManageRecord(){
        clearDB(FLOWMANAGEDBNAME);
    }

    std::string RWFlowDb::decodeFlowDBName(const std::string &flowName)
    {
        // first find flow
        std::vector<std::map<std::string, std::string>> fillContent = readFlowInfo();
        std::string dbName = "";
        for (int i = 0; i < int(fillContent.size()); i++)
        {
            std::string tempName = fillContent[i]["序号"] + "_" + fillContent[i]["名称"];
            if (tempName == flowName)
            {
                dbName = fillContent[i]["DBName"];
                break;
            }
        }
        return dbName;
    }
    void RWFlowDb::writeFlowRecord(std::string dbName, bool coverFlag, const std::vector<std::map<std::string, std::string>> &infoSS)
    {
        std::map<std::string, std::string> infoS;
        // int count=HGSaveDataToDB::countOfTable(HgOnlinePlatformModule::getTaskSeqName());
        if (coverFlag)
        {
            dbOpera.clearDB(dbName);
        }
        if (infoSS.size() <= 0)
        {
            infoS["序号"] = "";
            infoS["名称"] = "";
            infoS["参数"] = "";
            dbOpera.createDB(dbName, "序号", infoS);
        }
        for (const auto &infoS : infoSS)
        {
            dbOpera.writeRecord(dbName, "序号", infoS);
        }
        dbOpera.sortTable(dbName,"序号");
    }
}