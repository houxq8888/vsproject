#include "rwReagentDb.h"

namespace HGMACHINE{
  
std::vector<REAGENT> RWReagentDb::readReagentInfo(const std::string &reagentTableName){
    std::vector<REAGENT> reagents;
    std::vector<std::map<std::string,std::string>> fillContent;
    std::map<std::string,std::string> infoS;
    for (int i=0;i<int(reagentInfoName.size());i++){
        infoS[reagentInfoName[i]]="";
    }
    fillContent=readRecord(/*HgOnlinePlatformModule::getReagentName()*/reagentTableName,infoS);
    for(int i=0;i<int(fillContent.size());i++){
        REAGENT reagent;
        reagent.index=std::stoi(fillContent[i]["序号"]);
        reagent.type=fillContent[i]["试剂类别"];
        reagent.serialNumber=fillContent[i]["试剂编码"];
        reagent.name=fillContent[i]["试剂名称"];
        reagent.concertraction=fillContent[i]["滴定度浓度"];
        reagent.calibrationDate=fillContent[i]["标定日期"];
        reagent.calibrationCircle=fillContent[i]["标定周期"];
        reagent.reagentRemainAmount=fillContent[i]["试剂余量"];
        reagent.currentState=fillContent[i]["当前状态"];
        reagent.dbName=fillContent[i]["DBName"];
        std::vector<std::map<std::string,std::string>> contents;
        infoS.clear();
        for (int j=0;j<int(reagentLinkDeviceName.size());j++){
            infoS[reagentLinkDeviceName[j]]="";
        }
        contents=readRecord(reagent.dbName,infoS);
        for (int j=0;j<int(contents.size());j++){
            reagent.linkDevices[j].index=std::stoi(contents[j]["序号"]);
            reagent.linkDevices[j].name=contents[j]["指标设备单元"];
            reagent.linkDevices[j].interfaceName=contents[j]["接口"];
            reagent.linkDevices[j].linkState=contents[j]["联机"];
            reagent.linkDevices[j].channel=contents[j]["当前通道"];
            reagent.linkDevices[j].choice=contents[j]["选择"];
        }
        reagents.push_back(reagent);
    }
    return reagents;
}
REAGENT RWReagentDb::getFromReagentMap(std::map<std::string,std::string> regentInfo,
    std::vector<std::map<std::string,std::string>> deviceInfos)
{
    REAGENT reagent;
    reagent.index=std::stoi(regentInfo["序号"]);
    reagent.type=regentInfo["试剂类别"];
    reagent.serialNumber=regentInfo["试剂编码"];
    reagent.name=regentInfo["试剂名称"];
    reagent.concertraction=regentInfo["滴定度浓度"];
    reagent.calibrationDate=regentInfo["标定日期"];
    reagent.calibrationCircle=regentInfo["标定周期"];
    reagent.reagentRemainAmount=regentInfo["试剂余量"];
    reagent.currentState=regentInfo["当前状态"];
    reagent.dbName=regentInfo["DBName"];
    for (int i=0;i<int(deviceInfos.size());i++){
        reagent.linkDevices[i].index=std::stoi(deviceInfos[i]["序号"]);
        reagent.linkDevices[i].name=deviceInfos[i]["指标设备单元"];
        reagent.linkDevices[i].interfaceName=deviceInfos[i]["接口"];
        reagent.linkDevices[i].linkState=deviceInfos[i]["联机"];
        reagent.linkDevices[i].channel=deviceInfos[i]["当前通道"];
        reagent.linkDevices[i].choice=deviceInfos[i]["选择"];
    }
    return reagent;
}
std::map<std::string,std::string> RWReagentDb::getMapFromReagent(const REAGENT &info){
    std::map<std::string,std::string> infoMap;
    infoMap["序号"]=std::to_string(info.index);
    infoMap["试剂类别"]=info.type;
    infoMap["试剂编码"]=info.serialNumber;
    infoMap["试剂名称"]=info.name;
    infoMap["滴定度浓度"]=info.concertraction;
    infoMap["标定日期"]=info.calibrationDate;
    infoMap["标定周期"]=info.calibrationCircle;
    infoMap["试剂余量"]=info.reagentRemainAmount;
    infoMap["当前状态"]=info.currentState;
    infoMap["DBName"]=info.dbName;
    return infoMap;
}
const std::vector<std::string> RWReagentDb::getReagentNoEditName() {
    return reagentNoEditName;
}
const std::vector<std::string> RWReagentDb::getReagentLinkDeviceNoEditName(){
    return reagentLinkDeviceNoEditName;
}
    void RWReagentDb::writeReagentRecord(bool coverFlag, const std::string& tableName,const std::vector<REAGENT> &infoSS)
    {
        std::map<std::string, std::string> infoS;
        if (coverFlag)
        {
            dbOpera.clearDB(tableName/*HgOnlinePlatformModule::getReagentName()*/);
        }
        if (infoSS.size() <= 0)
        {
            for (int i = 0; i < int(reagentInfoName.size()); i++)
                infoS[reagentInfoName[i]] = "";
            dbOpera.writeRecord(tableName/*HgOnlinePlatformModule::getReagentName()*/, "序号", infoS);
            dbOpera.sortTable(tableName/*HgOnlinePlatformModule::getReagentName()*/,"序号");
            infoS.clear();
        }
        for (auto reagentInfo : infoSS)
        {
            infoS.clear();
            std::string deviceDBName = tableName/*HgOnlinePlatformModule::getReagentName()*/ + "_" + ReagentLinkDeviceDBName + std::to_string(reagentInfo.index);
            infoS["序号"] = std::to_string(reagentInfo.index);
            infoS["试剂类别"] = reagentInfo.type;
            infoS["试剂编码"] = reagentInfo.serialNumber;
            infoS["试剂名称"] = reagentInfo.name;
            infoS["滴定度浓度"] = reagentInfo.concertraction;
            infoS["标定日期"] = reagentInfo.calibrationDate;
            infoS["标定周期"] = reagentInfo.calibrationCircle;
            infoS["试剂余量"] = reagentInfo.reagentRemainAmount;
            infoS["当前状态"] = reagentInfo.currentState;
            infoS["DBName"] = deviceDBName;
            dbOpera.writeRecord(tableName/*HgOnlinePlatformModule::getReagentName()*/, "序号", infoS);
            dbOpera.sortTable(tableName/*HgOnlinePlatformModule::getReagentName()*/,"序号");
            infoS.clear();
            dbOpera.clearDB(deviceDBName);
            for (auto deviceS : reagentInfo.linkDevices)
            {
                infoS["序号"] = std::to_string(deviceS.second.index);
                infoS["指标设备单元"] = deviceS.second.name;
                infoS["接口"] = deviceS.second.interfaceName;
                infoS["联机"] = deviceS.second.linkState;
                infoS["当前通道"] = deviceS.second.channel;
                infoS["选择"] = deviceS.second.choice;
                dbOpera.writeRecord(deviceDBName, "序号", infoS);
            }
            dbOpera.sortTable(deviceDBName,"序号");
        }
    }
std::vector<std::map<std::string,std::string>> RWReagentDb::readReagentDeviceInfo(std::string dbName){
    std::vector<std::map<std::string,std::string>> fillContent;
    std::map<std::string,std::string> info;
    for (int i=0;i<int(reagentLinkDeviceName.size());i++)
        info[reagentLinkDeviceName[i]]="";
    fillContent=readRecord(dbName,info);
    return fillContent;
}

std::map<std::string,std::string> RWReagentDb::getMapFromDevices(const ReagentLinkDevice &devices){
    ReagentLinkDevice device=devices;
    std::map<std::string,std::string> info;
    info["序号"]=std::to_string(device.index);
    info["指标设备单元"]=device.name;
    info["接口"]=device.interfaceName;
    info["联机"]=device.linkState;
    info["当前通道"]=device.channel;
    info["选择"]=device.choice;
    return info;
}
}