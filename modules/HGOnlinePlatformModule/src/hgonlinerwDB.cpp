#include "hgonlinerwDB.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include "hglog4cplus.h"
#include "hgserial.h"
#include "hgonlineplatformmodule.h"
#include "HGExactTime.h"
#include "hgcommonutility.h"
#include "HGAuthorityService.h"


namespace HGMACHINE{
    #define DEVICEFROMLOWERPCDBNAME "DeviceInfoFromLowerPC"
    #define DEVICE2_SET_MODULES             std::vector<std::string>{"馈液单元","辅助泵","搅拌器"}
    #define DEVICE5_SET_MODULES             DEVICE2_SET_MODULES
    #define DEVICE1_SET_MODUELS             std::vector<std::string>{"外接设备","扩展接口"}
    #define DEVICE4_SET_MODULES             std::vector<std::string>{"PH检测器"}
    #define DEVICE_UNKNOWN                  std::vector<std::string>{"无法识别"}






std::vector<std::string> deviceInterfaceInfoName={
    "序号","名称","父名","参数"
};
std::vector<std::string> authorityInfoName={""
};
std::vector<std::string> resultInfoName={
    "时间","通道","样品池","目标成分","测试含量","操作员","流程","方法", "检测器", "滴定剂", "反应液", \
    "溶剂", "分析时长", "信号值",  "信号微分值", "体积", "信号二次微分"
};


std::vector<std::string> gNoexpandDeviceNames={
    "外接设备","扩展接口"
};
#ifdef ENABLE_CAS6
std::vector<std::string> taskInfoName={"序号","盘位","批号","样品名称","样品编号","样品总量",\
    "通道","流程","方法","空白","检测状态",\
    "状态","平衡策略"
};
std::vector<std::string> insertDeviceNameOrder={
    "AKF-CAS6","外接设备","无法识别"
};
    std::map<std::string,std::map<std::string,std::vector<DeviceName>>> gDeviceNames={
        {"AKF-CAS6",{
                    {"检测器",{
                        DeviceName("水分仪","shuifenyi")
                    }},
                    {"进样器",{
                        DeviceName("自动进样器","zidongjinyangqi")
                    }},
                    {"加热板",{
                        DeviceName("加热板","jiareban")
                    }}
        }},
        {"外接设备",{
                    {"COM 1#",{
                        DeviceName("COM 1#","COM1#")
                    }}
                    }
        }
    };
#else
std::vector<std::string> taskInfoName={"序号","样品来源","取样方式","样品名称","目标成分","样品总量","单位",\
    "condition1","value1","condition2","value2","通道","流程","方法","工作模式",\
    "间隔时间","取样管路","状态"
};
std::vector<std::string> insertDeviceNameOrder={
    "在线监测","外接设备","扩展接口","无法识别"
};
std::map<std::string,std::map<std::string,std::vector<DeviceName>>> gDeviceNames={
    {"在线监测",{
                {"检测器",{
                    DeviceName("电极接口1#-Upol","dianjijiekou1#-Upol"),
                    DeviceName("SENSOR 1#","SENSOR1#"),
                    DeviceName("SENSOR 2#","SENSOR2#"),
                    DeviceName("电极接口2#-lpol","dianjijiekou2#-lpol"),
                    DeviceName("电极接口3#-Upol","dianjijiekou3#-Upol"),
                    DeviceName("SENSOR 3#","SENSOR3#")
                }},
                {"馈液单元",{
                    DeviceName("CAN-1#滴定组件","CAN-1#didingzujian"),
                    DeviceName("CAN-2#滴定组件","CAN-2#didingzujian"),
                    DeviceName("CAN-3#滴定组件","CAN-3#didingzujian"),
                    DeviceName("CAN-4#滴定组件","CAN-4#didingzujian")
                }},
                {"辅助泵",{
                    DeviceName("PW-1#接口","PW-1#jiekou"),
                    DeviceName("PW-2#接口","PW-2#jiekou"),
                    DeviceName("PW-3#接口","PW-3#jiekou"),
                    DeviceName("PW-4#接口","PW-4#jiekou"),
                    DeviceName("CAN-5#接口","CAN-5#jiekou"),
                    DeviceName("CAN-6#接口","CAN-6#jiekou")
                }},
                {"搅拌器",{
                    DeviceName("搅拌接口1#","jiaobanjiekou1#"),
                    DeviceName("搅拌接口2#","jiaobanjiekou2#"),
                    DeviceName("搅拌接口3#","jiaobanjiekou3#"),
                    DeviceName("搅拌接口4#","jiaobanjiekou4#")
                }}
                }
    },
    {"外接设备",{
                {"COM 1#",{
                    DeviceName("COM 1#","COM1#")
                }}
                }
    },
    {"扩展接口",{
                {"COM 2#",{
                    DeviceName("COM 2#","COM2#")
                }},
                {"COM 3#",{
                    DeviceName("COM 3#","COM3#")
                }},
                {"CAN-11#",{
                    DeviceName("CAN-11#","CAN-11#")
                }},
                {"USB 1#",{
                    DeviceName("USB 1#","USB1#")
                }},
                {"4-20 mA信号",{
                    DeviceName("4-20 mA信号","4-20mAxinhao")
                }}
                }
    },
    {"无法识别",{}
    }
};
#endif

    SendInfo HGOnlineRWDB::response;

    HGOnlineRWDB::HGOnlineRWDB(){

    }
    HGOnlineRWDB::~HGOnlineRWDB(){

    }
    void HGOnlineRWDB::writeRecord(std::string dbName, std::string keyName, const std::map<std::string, std::string> &infoS)
    {
        dbOpera.writeRecord(dbName, keyName, infoS);
    }

    void HGOnlineRWDB::writeMaterialInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.writeRecord(MATERIALINFODBNAME, "MaterialNo", infoS);
        dbOpera.sortTable(MATERIALINFODBNAME,"MaterialNo");
    }
    void HGOnlineRWDB::writeResultRecord(bool coverFlag, std::string dbName, const std::vector<std::map<std::string, std::string>> &infoSS)
    {
        std::map<std::string, std::string> infoS;
        if (coverFlag)
        {
            dbOpera.clearDB(dbName);
        }
        for (const auto &infoS : infoSS)
        {
            dbOpera.writeRecord(dbName, "", infoS);
        }
    }
    std::vector<std::map<std::string,std::string>> HGOnlineRWDB::readMaterialInfo(){
        std::vector<std::map<std::string, std::string>> infoS;
        std::map<std::string, std::string> materialInfo;
        materialInfo["MaterialNo"] = "";
        materialInfo["ReserveQuantity"] = "";
        materialInfo["MaterialName"] = "";
        materialInfo["MaterialManaqer"] = "";
        materialInfo["Recipient"] = "";
        materialInfo["MaterialDescription"] = "";
        materialInfo["AvailableQuantity"] = "";
        materialInfo["Specification"] = "";
        materialInfo["MaterialDescription"] = "";
        infoS=dbOpera.readRecord(MATERIALINFODBNAME, materialInfo);
        return infoS;
    }
    void HGOnlineRWDB::writeEnterComInfo(const std::string &comInfo)
    {
        std::string curDirPath = HGOnlineRWDB::readCurDirPath();
        std::map<std::string, std::string> noLoginRecordInfo;
        noLoginRecordInfo["enterCom"] = comInfo;
        dbOpera.recordSingleInfo(SYSTEMINFODBNAME, noLoginRecordInfo);
    }
    std::string HGOnlineRWDB::readEnterComInfo()
    {
        std::string curDirPath = HGOnlineRWDB::readCurDirPath();
        std::map<std::string, std::string> noLoginRecordInfo;
        noLoginRecordInfo["enterCom"] = "";
        dbOpera.readSingleInfo(SYSTEMINFODBNAME, noLoginRecordInfo);
        return noLoginRecordInfo["enterCom"];
    }
    void HGOnlineRWDB::clearDeviceFromLPCManageInfo(std::string dbName)
    {
        dbOpera.clearDB(dbName);
    }
    void HGOnlineRWDB::deleteDeviceFromLPCManageInfo(std::string dbName)
    {
        dbOpera.deleteDB(dbName);
    }
    void HGOnlineRWDB::setDeviceFromLPCManageOffline()
    {
        std::map<std::string, std::string> infoS;
        infoS["DeviceType"] = "";
        infoS["InstrumentId"] = "";
        infoS["CanId"] = "";
        infoS["FaceNumber"] = "";
        infoS["DBName"] = "";
        infoS["Status"] = "";
        std::vector<std::map<std::string, std::string>> contents;
        contents = dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME, infoS);
        for (int i = 0; i < int(contents.size()); i++)
        {
            contents[i]["Status"] = "Offline";
            dbOpera.writeRecord(DeviceFromLPCManageInfoDBNAME, "InstrumentID", contents[i]);
        }
        dbOpera.sortTable(DeviceFromLPCManageInfoDBNAME,"InstrumentID");
    }
    void HGOnlineRWDB::writeDeviceFromLPCManageInfo(std::string dbName, const std::map<std::string, std::string> &contents)
    {
        dbOpera.writeRecord(dbName, "InstrumentId", contents);
        dbOpera.sortTable(dbName,"InstrumentID");
    }
    void HGOnlineRWDB::writeLowerPCDeviceInfo(const UpperPCToLowerPC &upperToLowerStruct)
    {
        int faceNumber = std::atoi(std::to_string(upperToLowerStruct.CheckDevice.FaceNumber).c_str());
        std::map<std::string, std::map<std::string, std::string>> returnContents;
        returnContents[DeviceFromLPCManageInfoDBNAME]["DeviceType"] = std::to_string(upperToLowerStruct.CheckDevice.Type);
        returnContents[DeviceFromLPCManageInfoDBNAME]["InstrumentId"] = upperToLowerStruct.CheckDevice.Instrument_Id;
        returnContents[DeviceFromLPCManageInfoDBNAME]["CanId"] = std::to_string(upperToLowerStruct.CheckDevice.CanId);
        returnContents[DeviceFromLPCManageInfoDBNAME]["FaceNumber"] = std::to_string(upperToLowerStruct.CheckDevice.FaceNumber);
        returnContents[DeviceFromLPCManageInfoDBNAME]["DBName"] = DeviceFromLPCManageInfoDBNAME + std::string(upperToLowerStruct.CheckDevice.Instrument_Id);
        returnContents[DeviceFromLPCManageInfoDBNAME]["Status"] = "Online";
        writeDeviceFromLPCManageInfo(DeviceFromLPCManageInfoDBNAME, returnContents[DeviceFromLPCManageInfoDBNAME]);

        std::string dbName = returnContents[DeviceFromLPCManageInfoDBNAME]["DBName"];
        // clearDeviceFromLPCManageInfo(dbName);
        for (int i = 0; i < faceNumber; i++)
        {
            returnContents[dbName]["PortIndex"] = std::to_string(i + 1);
            returnContents[dbName]["PortType"] = std::to_string(upperToLowerStruct.CheckDevice.FaceType[i]);
            returnContents[dbName]["PortName"] = std::string(upperToLowerStruct.CheckDevice.Face_Name[i]);
            returnContents[dbName]["FaceRun"] = std::to_string(upperToLowerStruct.CheckDevice.FaceRun[i]);
            returnContents[dbName]["FaceState"] = std::to_string(upperToLowerStruct.CheckDevice.FaceState[i]);

            // ss.str("");
            // ss<<"insert dbname:"<<dbName;
            // ss << "  faceIndex:" << i <<"PortIndex:"<<returnContents[dbName]["PortIndex"]<<" PortType:" << returnContents[dbName]["PortType"] << " PortName:" \
            //     << returnContents[dbName]["PortName"] << " FaceRun:" << returnContents[dbName]["FaceRun"] << "FaceState:"<<returnContents[dbName]["FaceState"]<<" \n";

            // printf(" %s\n", ss.str().c_str());

            writeDeviceFromLPCInfo(dbName, returnContents[dbName]);
        }
    }
    void HGOnlineRWDB::writeCAS6DeviceInfo(const std::vector<std::map<std::string,std::string>> &info){
        for (int i=0;i<int(info.size());i++)
            dbOpera.writeRecord(CAS6_BININFODBNAME, "BinIndex", info[i]);
        
        dbOpera.sortTable(CAS6_BININFODBNAME,"BinIndex");
    }
    void HGOnlineRWDB::clearCAS6DeviceStatus(){
        std::map<std::string,std::string> version;
        version["BinName"]="";
        version["BinVersion"]="";
        version["BinIndex"]="";
        version["Online"]="";

        std::vector<std::map<std::string,std::string>> info = dbOpera.readRecord(CAS6_BININFODBNAME, version);
        for (int i=0;i<int(info.size());i++) {
            info[i]["Online"]="offline";
        }
        writeCAS6DeviceInfo(info);
    }
    std::vector<std::string> HGOnlineRWDB::getExistCAS6DeviceNames(){
        std::vector<std::string> ret;
        std::map<std::string,std::string> version;
        version["BinName"]="";
        version["BinVersion"]="";
        version["BinIndex"]="";
        version["Online"]="";

        std::vector<std::map<std::string,std::string>> info = dbOpera.readRecord(CAS6_BININFODBNAME, version);
        for (int i=0;i<int(info.size());i++) {
            if (info[i]["Online"]!="online") continue;
            ret.push_back(info[i]["BinName"]);
        }
        return ret;
    }
    std::vector<std::string> HGOnlineRWDB::getExistDeviceNames(){
        std::vector<std::string> returnInfos;
        std::map<std::string,std::string> infoS;
        infoS["DeviceType"]="";
        infoS["InstrumentId"]="";
        infoS["CanId"]="";
        infoS["FaceNumber"]="";
        infoS["DBName"]="";
        infoS["Status"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME,infoS);
        std::map<std::string,std::string> infoDevice;
        infoDevice["PortIndex"]="";
        infoDevice["PortType"]="";
        infoDevice["PortName"]="";
        infoDevice["FaceRun"] = "";
        infoDevice["FaceState"] = "";

        for (int i=0;i<int(contents.size());i++){
            if (contents[i]["Status"]=="Online"){
                std::vector<std::map<std::string,std::string>> contentPorts;
                contentPorts=dbOpera.readRecord(contents[i]["DBName"],infoDevice);
                for (int j=0;j<int(contentPorts.size());j++){
                    std::string portName=contentPorts[i]["PortName"];
                    // printf("portName:%s\n",contentPorts[i]["PortName"].c_str());
                    returnInfos.push_back(portName);
                }
            }
        }
        return returnInfos;
    }

    uint16_t HGOnlineRWDB::getDeviceTypeFromCanId(const uint8_t &canId)
    {
        uint16_t type = 0;
        std::map<std::string, std::string> infoS;
        infoS["DeviceType"] = "";
        infoS["InstrumentId"] = "";
        infoS["CanId"] = "";
        infoS["FaceNumber"] = "";
        infoS["DBName"] = "";
        infoS["Status"] = "";
        std::vector<std::map<std::string, std::string>> contents;
        contents = dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME, infoS);
        for (int i = 0; i < int(contents.size()); i++)
        {
            if (contents[i]["CanId"] == std::to_string(canId))
            {
                return (uint16_t)(std::stoi(contents[i]["DeviceType"].c_str()));
            }
        }
        return type;
    }
    bool HGOnlineRWDB::isCanIdOnline(const uint8_t &canId){
        std::map<std::string,std::string> infoS;
        infoS["DeviceType"]="";
        infoS["InstrumentId"]="";
        infoS["CanId"]="";
        infoS["FaceNumber"]="";
        infoS["DBName"]="";
        infoS["Status"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME,infoS);
        std::string status="";
        for (int i=0;i<int(contents.size());i++){
            if (contents[i]["CanId"]==std::to_string(canId)){
                status=contents[i]["Status"];
            }
        }
        if (status=="Online") return true;
        else return false;
    }
    bool HGOnlineRWDB::isFaceIndexOnline(const uint8_t &canId,const uint8_t &faceIndex){
        std::map<std::string,std::string> infoS;
        infoS["DeviceType"]="";
        infoS["InstrumentId"]="";
        infoS["CanId"]="";
        infoS["FaceNumber"]="";
        infoS["DBName"]="";
        infoS["Status"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME,infoS);
        std::map<std::string,std::string> infoDevice;
        infoDevice["PortIndex"]="";
        infoDevice["PortType"]="";
        infoDevice["PortName"]="";
        infoDevice["FaceRun"] = "";
        infoDevice["FaceState"] = "";

        for (int i=0;i<int(contents.size());i++){
            if (contents[i]["CanId"]==std::to_string(canId)){
                std::vector<std::map<std::string,std::string>> contentPorts;
                contentPorts=dbOpera.readRecord(contents[i]["DBName"],infoDevice);
                for (int j=0;j<int(contentPorts.size());j++){
                    if (contentPorts[j]["PortIndex"]==std::to_string(faceIndex)){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    std::vector<std::string> HGOnlineRWDB::getDevicePortInfos(){
        std::vector<std::string> returnInfos;
        std::map<std::string,std::string> infoS;
        infoS["DeviceType"]="";
        infoS["InstrumentId"]="";
        infoS["CanId"]="";
        infoS["FaceNumber"]="";
        infoS["DBName"]="";
        infoS["Status"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(DeviceFromLPCManageInfoDBNAME,infoS);
        std::map<std::string,std::string> infoDevice;
        infoDevice["PortIndex"]="";
        infoDevice["PortType"]="";
        infoDevice["PortName"]="";
        infoDevice["FaceRun"] = "";
        infoDevice["FaceState"] = "";

        for (int i=0;i<int(contents.size());i++){
            if (contents[i]["Status"]=="Online"){
                std::vector<std::map<std::string,std::string>> contentPorts;
                contentPorts=dbOpera.readRecord(contents[i]["DBName"],infoDevice);
                for (int j=0;j<int(contentPorts.size());j++){
                    std::string portName=contents[i]["CanId"]+"_"+contentPorts[j]["PortIndex"];
                    returnInfos.push_back(portName);
                }
            }
        }
        return returnInfos;
    }
    void HGOnlineRWDB::writeDeviceFromLPCInfo(std::string dbName,const std::map<std::string,std::string> &contents){
        dbOpera.writeRecord(dbName,"PortIndex",contents);
        dbOpera.sortTable(dbName,"PortIndex");
    }
    std::vector<DeviceInterface> HGOnlineRWDB::readDeviceInfo(std::string dbName){
        std::vector<std::map<std::string,std::string>> fillContent;
        std::map<std::string,std::string> info;
        for (int index=0;index<int(deviceInterfaceInfoName.size());index++){
            info[deviceInterfaceInfoName[index]]="";
        }
        std::vector<DeviceInterface> devices;
        fillContent=readRecord(dbName,info);
        for (auto content: fillContent){
            devices.push_back(getDeviceInterfaceFromMap(content));
        }
        return devices;
    }
    int HGOnlineRWDB::readLoginAuthority()
    {
        std::string curDirPath = HGOnlineRWDB::readCurDirPath();
        std::map<std::string, std::string> noLoginRecordInfo;
        noLoginRecordInfo["authority"] = "";
        dbOpera.readSingleInfo(SYSTEMINFODBNAME, noLoginRecordInfo);
        if (noLoginRecordInfo["authority"]=="SystemManager"){
            return SYSTEM_MANAGER;
        } else if (noLoginRecordInfo["authority"]=="Manager"){
            return SOFTWARE_MANAGER;
        } else if (noLoginRecordInfo["authority"]=="MaintenanceStaff"){
            return MAINTENANCE_MANAGER;
        } else if (noLoginRecordInfo["authority"]=="Operator"){
            return MANIPULATOR;
        } else {
            return SYSTEM_MANAGER;
        }
    }
    std::map<std::string, std::string> HGOnlineRWDB::decodeDeviceDataInfoAndWrite(const UpperPCToLowerPC &upperToLowerStruct, const Task &curTaskInfo)
    {
        std::string loginName=readLoginName();
        std::map<std::string, std::map<std::string, std::string>> returnContents;
        std::string dbName="";
        std::map<std::string,std::string> mapNames;
        mapNames["curDB"]="";
        dbOpera.readSingleInfo(TASKRUNSTATUSNAME,mapNames);
        dbName=mapNames["curDB"];

        returnContents[dbName]["时间"] = getStandardCurTime();
        returnContents[dbName]["通道"] = std::to_string(curTaskInfo.testChannel);
        #ifdef ENABLE_CAS6
        #else
        returnContents[dbName]["样品池"] = curTaskInfo.sampleInput;
        returnContents[dbName]["目标成分"] = curTaskInfo.targetElement;

        #endif
        returnContents[dbName]["测试含量"] = std::to_string(curTaskInfo.content);
        returnContents[dbName]["操作员"] = loginName==""?"admin":loginName;
        returnContents[dbName]["电解速率"] = std::to_string(upperToLowerStruct.AnalysisData.Balance_Speed);
        returnContents[dbName]["漂移值"] = std::to_string(upperToLowerStruct.AnalysisData.Drift);
        returnContents[dbName]["分析时长"] = std::to_string(upperToLowerStruct.AnalysisData.TimeSpan);
        returnContents[dbName]["测量电压"] = std::to_string(upperToLowerStruct.AnalysisData.Voltage);
        returnContents[dbName]["水质量"] = std::to_string(upperToLowerStruct.AnalysisData.WaterWeigth);
        std::ostringstream resSS;
        resSS << "时间:" << returnContents[dbName]["时间"] << "电解速率:" << upperToLowerStruct.AnalysisData.Balance_Speed << ",漂移值:" << upperToLowerStruct.AnalysisData.Drift << ",分析时长:"
              << upperToLowerStruct.AnalysisData.TimeSpan << ",测量电压:" << upperToLowerStruct.AnalysisData.Voltage << ",水质量:" << upperToLowerStruct.AnalysisData.WaterWeigth;
        printf("%s\n", resSS.str().c_str());


        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(resSS.str(),LOGINFO);

        writeDeviceDataInfo(dbName, returnContents[dbName]);
        return returnContents[dbName];
    }
    void HGOnlineRWDB::writeDeviceDataInfo(std::string dbName,const std::map<std::string,std::string> &contents){
        dataOpera.writeRecord(dbName,"时间",contents);
        dbOpera.sortTable(dbName,"时间");
    }
    std::vector<std::map<std::string,std::string>> HGOnlineRWDB::readDeviceDataInfo(std::string dbName){
        std::map<std::string,std::string> info;
        info["时间"] = "";
        info["通道"] = "";
        info["样品池"] = "";
        info["目标成分"] = "";
        info["测试含量"] = "";
        info["操作员"]="";
        info["电解速率"]="";
        info["漂移值"]="";
        info["分析时长"]="";
        info["测量电压"]="";
        info["水质量"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dataOpera.readRecord(dbName,info);
        return contents;
    }

    DeviceInterface HGOnlineRWDB::getDeviceInterfaceFromMap(std::map<std::string,std::string> info){
        DeviceInterface device;
        device.name=info["名称"];
        device.rootName=info["父名"];
        device.param=info["参数"];
        return device;
    }


std::map<std::string,std::string> HGOnlineRWDB::getDeviceInfoMap(int index,DeviceInterface info){
    std::map<std::string,std::string> infoS;
    infoS["序号"]=std::to_string(index);
    infoS["名称"]=info.name;
    infoS["父名"]=info.rootName;
    infoS["参数"]=info.param;
    return infoS;
}

}