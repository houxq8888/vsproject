#include "rwTaskDb.h"

namespace HGMACHINE {

    std::map<std::string,std::string> RWTaskDb::getTaskMap(int index,Task taskInfo){
        std::map<std::string, std::string> infoS;
        infoS["序号"] = std::to_string(index);
        infoS["样品名称"] = taskInfo.sampleName;
        infoS["流程"] = taskInfo.testFlow;
        infoS["方法"] = taskInfo.testMethod;
        infoS["状态"]=taskInfo.runStatus;
        infoS["通道"] = std::to_string(taskInfo.testChannel);
        infoS["样品总量"] = std::to_string(taskInfo.content);

        #ifdef ENABLE_CAS6
        infoS["批号"]=taskInfo.batchNo;
        infoS["样品编号"]=taskInfo.serailNo;
        infoS["盘位"]=std::to_string(taskInfo.circleNo);
        infoS["检测状态"]=taskInfo.detectStatus;
        infoS["空白"]=taskInfo.blank;
        infoS["平衡策略"]=std::to_string(taskInfo.sampleDetectStrategy);
        #else
        infoS["样品来源"]=taskInfo.sampleInput;
        infoS["取样方式"]=taskInfo.getSampleMethod;
        infoS["目标成分"] = taskInfo.targetElement;
        infoS["单位"] = taskInfo.unit;
        infoS["condition1"] = taskInfo.standard1Condition;
        infoS["value1"] = std::to_string(taskInfo.standard1);
        infoS["condition2"] = taskInfo.standard2Condition;
        infoS["value2"] = std::to_string(taskInfo.standard2);
        infoS["工作模式"] = taskWorkingMode[taskInfo.workingMode];
        infoS["间隔时间"] = taskInterval[taskInfo.interval];
        infoS["取样管路"] = (taskInfo.getSamplePump);
        #endif
        return infoS;
    }
    Task RWTaskDb::getTaskSFromMap(std::map<std::string, std::string> infoS)
    {
        Task taskInfo;
        taskInfo.sampleName = infoS["样品名称"];
        taskInfo.testFlow = infoS["流程"];
        taskInfo.testMethod = infoS["方法"];
        taskInfo.runStatus = infoS["状态"];
        taskInfo.testChannel = std::stoi(infoS["通道"]);
        taskInfo.content = std::stod(infoS["样品总量"]);
        #ifdef ENABLE_CAS6
        taskInfo.batchNo=infoS["批号"];
        taskInfo.circleNo=std::atoi(infoS["盘位"].c_str());
        taskInfo.serailNo=infoS["样品编号"];
        taskInfo.detectStatus=infoS["检测状态"];
        taskInfo.blank=infoS["空白"];
        taskInfo.sampleDetectStrategy=std::atoi(infoS["平衡策略"].c_str());
        #else
        taskInfo.sampleInput = infoS["样品来源"];
        taskInfo.getSampleMethod = infoS["取样方式"];
        taskInfo.getSamplePump = infoS["取样管路"];
        taskInfo.targetElement = infoS["目标成分"];
        taskInfo.unit = infoS["单位"];
        taskInfo.standard1Condition = infoS["condition1"];
        taskInfo.standard1 = std::stod(infoS["value1"]);
        taskInfo.standard2Condition = infoS["condition2"];
        taskInfo.standard2 = std::stod(infoS["value2"]);
        auto result = findKeyByValue(taskWorkingMode, infoS["工作模式"]);
        if (result.first)
        {
            taskInfo.workingMode = result.second;
        }
        auto result1 = findKeyByValue(taskInterval, infoS["间隔时间"]);
        if (result1.first)
            taskInfo.interval = result1.second;
        #endif
        return taskInfo;
    }
    std::vector<std::map<std::string, std::string>> RWTaskDb::readTaskInfo(const std::string& taskSeqName)
    {
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(taskInfoName.size()); i++)
        {
            infoS[taskInfoName[i]] = "";
        }
        fillContent = readRecord(taskSeqName/*HgOnlinePlatformModule::getTaskSeqName()*/, infoS);
        return fillContent;
    }
    void RWTaskDb::writeTaskRecord(bool coverFlag, const std::string& tableName,const std::vector<std::map<std::string, std::string>> &infoSS)
    {
        std::map<std::string, std::string> infoS;
        if (coverFlag)
        {
            // dbOpera.deleteDB(readCurDirPath() + DATABASE_PATH +DB_PATH, HgOnlinePlatformModule::getTaskSeqName());
        }
        for (const auto &infoS : infoSS)
        {
            dbOpera.writeRecord(tableName, "序号", infoS);
        }
        dbOpera.sortTable(tableName,"序号");
    }

    void RWTaskDb::insertTaskRunInfo(const std::string &startTime,const std::string &tableName)
    {
        std::string loginName=readLoginName();
        std::vector<std::map<std::string, std::string>> taskInfos = readTaskInfo(tableName); //  HgOnlinePlatformModule::getTaskSeqName());
        std::map<std::string, std::string> infoS;
        for (auto taskInfo : taskInfos)
        {
            infoS["序号"] = std::to_string(dataOpera.countOfTable(TASKRUNRECORDINFODBNAME) + 1);
            infoS["时间"] = startTime;
            infoS["通道"] = taskInfo["通道"];
            infoS["样品池"] = taskInfo["样品来源"];
            infoS["目标成分"] = taskInfo["目标成分"];
            infoS["流程"] = taskInfo["流程"];
            infoS["方法"] = taskInfo["方法"];
            infoS["检测器"] = "";
            infoS["滴定剂"] = "";
            infoS["反应液"] = "";
            infoS["溶剂"] = "";
            infoS["分析时长"] = "0";
            infoS["运行状态"] = "0";
            infoS["测试含量"] = "0";
            infoS["操作员"] = loginName == "" ? "admin" : loginName;
            infoS["DBName"] = "Data" + startTime;
            dataOpera.writeRecord(TASKRUNRECORDINFODBNAME,
                                        "序号", infoS);
        }
        dbOpera.sortTable(TASKRUNRECORDINFODBNAME,"序号");
    }
std::string RWTaskDb::getTaskRunFlag(){
    std::map<std::string,std::string> mapNames;
    mapNames["start"]="";
    readSingleInfo(TASKRUNSTATUSNAME,mapNames);
    return mapNames["start"];
}

std::string RWTaskDb::getTaskRunRecordDataDB(){
    std::map<std::string,std::string> mapNames;
    mapNames["curDB"]="";
    readSingleInfo(TASKRUNSTATUSNAME,mapNames);
    return mapNames["curDB"];
}
    std::vector<std::map<std::string, std::string>> RWTaskDb::getTaskRunInfo()
    {
        std::vector<std::map<std::string, std::string>> infoS;
        std::map<std::string, std::string> info;
        #ifdef ENABLE_CAS6
        info["序号"] = "";
        info["时间"] = "";
        info["通道"] = "";
        info["样品池"] = "";
        info["目标成分"] = "";
        info["流程"] = "";
        info["方法"] = "";
        info["检测器"] = "";
        info["滴定剂"] = "";
        info["反应液"] = "";
        info["溶剂"] = "";
        info["分析时长"] = "";
        info["运行状态"] = "";
        info["测试含量"] = "";
        info["操作员"] = "";
        info["DBName"] = "";
        infoS = dataOpera.readRecord(TASKRUNRECORDINFODBNAME, info);
        #else
        info["序号"] = "";
        info["时间"] = "";
        info["通道"] = "";
        info["样品池"] = "";
        info["目标成分"] = "";
        info["流程"] = "";
        info["方法"] = "";
        info["检测器"] = "";
        info["滴定剂"] = "";
        info["反应液"] = "";
        info["溶剂"] = "";
        info["分析时长"] = "";
        info["运行状态"] = "";
        info["测试含量"] = "";
        info["操作员"] = "";
        info["DBName"] = "";
        infoS = dataOpera.readRecord(TASKRUNRECORDINFODBNAME, info);
        #endif
        return infoS;
    }

void RWTaskDb::setTaskRunRecordDataDB(const std::string &dbName){
    std::map<std::string,std::string> mapNames;
    mapNames["curDB"]="Data"+dbName;
    recordSingleInfo(TASKRUNSTATUSNAME,mapNames);
}

void RWTaskDb::setTaskRunFlag(std::string flag){
    std::map<std::string,std::string> mapNames;
    mapNames["start"]=flag;
    recordSingleInfo(TASKRUNSTATUSNAME,mapNames);
}
void RWTaskDb::resetTaskRunFlag(){
    std::map<std::string,std::string> mapNames;
    mapNames["start"]="false";
    recordSingleInfo(TASKRUNSTATUSNAME,mapNames);
}
void RWTaskDb::resetTaskRunStatus(const std::string &tableName)
{
    std::vector<std::map<std::string, std::string>> fillTaskContent = readTaskInfo(tableName); //HgOnlinePlatformModule::getTaskSeqName());
    for (int index = 0; index < int(fillTaskContent.size()); index++)
    {
        fillTaskContent[index]["状态"] = "";
    }
    writeTaskRecord(true, tableName,/*HgOnlinePlatformModule::getTaskSeqName(),*/fillTaskContent);
}

}