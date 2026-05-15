#include "dashboardRWDB.h"

namespace HGMACHINE{
    std::vector<std::string> deviceMapInfoName={
        "inputTime",
        "longtitude",
        "latitude",
        "city",
        "deviceType", // 0--tuzhuang,1--diding,2--shuifen
        "inputOperator",
        "reserved1",
        "reserved2",
        "reserved3",
        "reserved4"
    };

    HGSaveDataToDB DashboardRWDB::dbOpera;
    DashboardRWDB::DashboardRWDB(){

    }
    DashboardRWDB::~DashboardRWDB(){

    }
    void DashboardRWDB::openDB(const std::string &basePath){
        dbOpera.openDB(DASHBOARDDB_PATH);
    }
    void DashboardRWDB::closeDB(){
        dbOpera.closeDB();
    }
    void DashboardRWDB::clearDB(const std::string& tableName){
        dbOpera.clearDB(/*DASHBOARDDB_PATH,*/tableName);
    }
    void DashboardRWDB::writeDashboardDataInfo(std::string tableName,
        const std::map<std::string,std::string> &contents)
    {
        std::vector<std::string> keys;
        keys.push_back("ts");
        keys.push_back("key");
        keys.push_back("recTs");
        dbOpera.writeDataWithMultiKey(/*DASHBOARDDB_PATH,*/tableName,keys,contents);
        // dbOpera.writeRecord(DASHBOARDDB_PATH,tableName,"ts",contents);
    }

    std::vector<std::map<std::string,std::string>> DashboardRWDB::readDashboardDataInfo(std::string tableName){
        std::map<std::string,std::string> info;
        info["ts"] = "";
        info["key"] = "";
        info["value"] = "";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(/*DASHBOARDDB_PATH,*/tableName,info);
        return contents;
    }
    void DashboardRWDB::writeMaintenanceInfo(const std::map<std::string,std::string> &contents,const int& type){
        dbOpera.recordSingleInfo(DASHBOARD_MAINTENANCE_TABLE+std::to_string(type),contents);
    }
    std::map<std::string,std::string> DashboardRWDB::readMaintenanceInfo(const int& type){
        std::map<std::string,std::string> info;
        switch (type){
            case 0:
                info["加药桶换水清理"]="";
                info["药桶换水清理"]="";
                info["加液桶换水清理"]="";
                info["液桶换水清理"]="";
                info["取样桶1换水清理"]="";
                info["取样桶2换水清理"]="";
                info["PH桶换水清理"]="";
                info["电导桶换水清理"]="";
                info["蠕动泵头"]="";
                info["蠕动泵管"]="";
                info["检测电极"]="";
                info["电解电极"]="";
                info["计量管活塞1"]="";
                info["计量管活塞2"]="";
                info["计量管活塞3"]="";
                break;
            case 1:
                info["蠕动泵头"]="";
                info["蠕动泵管"]="";
                info["检测电极"]="";
                info["电解电极"]="";
                info["计量管活塞1"]="";
                info["计量管活塞2"]="";
                break;
            case 2:
                info["蠕动泵头"]="";       
                info["蠕动泵管"]="";
                info["手工全柜换气"]="";
                info["干燥剂"]="";
                break;
            default:break;
        }
        
        dbOpera.readSingleInfo(DASHBOARD_MAINTENANCE_TABLE+std::to_string(type),info);
        return info;
    }
    void DashboardRWDB::writeDeviceMapInfo(const std::map<std::string,std::string> &info){
        dbOpera.writeRecord(DASHBOARD_DEVICE_MAP_TABLE,"inputTime",info);
    }
    std::vector<std::map<std::string,std::string>> DashboardRWDB::readDeviceMapInfo(){
        std::map<std::string,std::string> info;
        for (int i=0;i<int(deviceMapInfoName.size());i++){
            info[deviceMapInfoName[i]]="";
        }
        return dbOpera.readRecord(DASHBOARD_DEVICE_MAP_TABLE,info);
    }
}