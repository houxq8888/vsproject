#include "rwDb.h"
#include "HGExactTime.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <cstdio>
#include "hgcommonutility.h"
#include "config.h"
#include "hglogservice.h"

namespace HGMACHINE{
    std::vector<std::string> userInfoName={
    "UserNo",
    "UserAccount",
    "Password",
    "PasswdInputLimitCount",
    "PasswdCycle",
    "UserName",
    "UserJob",
    "Department",
    "Authority",
    "AccountManagement",
    "UserDescription",
    "Creator",
    "CreateTime",
    "LastModifier",
    "LastModifyTime",
    "LoginTime",
    "QuitTime"
};
std::vector<std::string> userGroupInfoName={
        "GroupName",
        "GroupPNumber",
        "GroupPerson",
        "Authority",
        "Creator",
        "CreateTime",
        "LastModifier",
        "LastModifyTime"
    };
    std::vector<std::string> scannerInfoName={
        "Sname","Sbatch","Sserial","Samount","Separator"
    };
    std::vector<std::string> methodInfoName={
        "序号","名称","类型","创建时间","DBName","参数"
    };
    std::vector<std::string> flowInfoName={
        "序号","名称","类型","创建时间","DBName"
    };
    std::vector<std::string> taskInfoName={"序号","样品来源","取样方式","样品名称","目标成分","样品总量","单位",\
      "condition1","value1","condition2","value2","通道","流程","方法","工作模式",\
        "间隔时间","取样管路","状态"
    };
    std::vector<std::string> channelInfoName={
        "序号","通道","检测器","检测方法","DBName"
    };
std::vector<std::string> reagentInfoName={
    "序号","试剂类别","试剂编码","试剂名称","滴定度浓度","标定日期","标定周期","试剂余量","当前状态","DBName"
};

const std::vector<std::string> reagentNoEditName={
    "序号","标定日期","试剂余量","当前状态"
};
const std::vector<std::string> reagentLinkDeviceNoEditName={
    "序号","联机","当前通道"
};
const std::vector<std::string> reagentLinkDeviceName={
    "序号","指标设备单元","接口","联机","当前通道","选择"
};
    std::map<int,std::string> taskWorkingMode={
        {0,"周期性测定"},
        {1,"循环测试"},
        {2,"定时测试"}
    };
    std::map<int,std::string> taskInterval={
        {0,"30min"}
    };
    RWDb::RWDb(){

    }
    RWDb::~RWDb(){

    }

    HGSaveDataToDB RWDb::dbOpera;
    HGSaveDataToDB RWDb::dataOpera;
    HGSaveDataToDB RWDb::logOpera;
#define DATABASE_PATH "/database"
    void RWDb::openDB(const std::string &basePath){
        std::string dbPath=basePath+DATABASE_PATH;
        HGMkDir(dbPath);
        dbOpera.openDB(dbPath+DB_PATH);
        dataOpera.openDB(dbPath+DATA_DB_PATH);
        logOpera.openDB(dbPath+LOG_DB_PATH);
    }
    void RWDb::closeDB(){
        dbOpera.closeDB();
        dataOpera.closeDB();
        logOpera.closeDB();
    }
    void RWDb::clearDB(const std::string& tableName){
        dbOpera.clearDB(tableName);
    }
    std::string RWDb::readCurDirPath(){
        std::string path=FileConfig::getDirPath(); 
        return path;
    }
    std::string RWDb::readLoginName()
    {
        std::string curDirPath = RWDb::readCurDirPath();
        std::map<std::string, std::string> noLoginRecordInfo;
        noLoginRecordInfo["loginName"] = "";
        dbOpera.readSingleInfo(SYSTEMINFODBNAME, noLoginRecordInfo);
        return noLoginRecordInfo["loginName"];
    }
    int RWDb::readAuditTrailLogCount(const std::string &tableName){
        std::map<std::string,std::string> info;
        info["lastAuditTrailDB"]="";
        logOpera.readSingleInfo(AUDITTRAILDBRECORD,info);
        std::map<std::string,std::string> infoS = {
            {"Operator",""},
            {"Time",""},
            {"LogContent",""}
        };
        std::string readTableName="";
        if (tableName != ""){
            readTableName = tableName;
        } else {
            readTableName = info["lastAuditTrailDB"];
        }
        return logOpera.countOfTable(readTableName);
    }
    void RWDb::recordSingleInfo(std::string dbName, const std::map<std::string, std::string> &infoS)
    {
        dbOpera.recordSingleInfo(dbName, infoS);
    }



    template <typename K, typename V>
    std::pair<bool, K> findKeyByValue(const std::map<K, V> &m, const V &value)
    {
        for (const auto &[key, val] : m)
        {
            if (val == value)
            {
                return {true, key};
            }
        }
        return {false, K()};
    }

    std::vector<std::string> RWDb::getAllAuditLogTables(){
        std::vector<std::string> tableNames=logOpera.getAllTables();
        for (int i=0;i<int(tableNames.size());i++){
            if (tableNames[i].find("AuditTrailLog") == std::string::npos){
                tableNames.erase(tableNames.begin()+i);
                i--;
            }
        }
        return tableNames;
    }
    std::vector<std::map<std::string,std::string>> RWDb::readAuditTrailLog(const std::string &tableName){
        std::map<std::string,std::string> info;
        info["lastAuditTrailDB"]="";
        logOpera.readSingleInfo(AUDITTRAILDBRECORD,info);

        std::map<std::string,std::string> infoS = {
            {"Operator",""},
            {"Time",""},
            {"LogContent",""}
        };
        std::string readTableName="";
        if (tableName != ""){
            readTableName = tableName;
        } else {
            readTableName = info["lastAuditTrailDB"];
        }
        return logOpera.readRecord(readTableName, infoS);
    }
    std::vector<std::map<std::string, std::string>> RWDb::readRecord(std::string dbName, std::map<std::string, std::string> &infoS)
    {
        return dbOpera.readRecord(dbName, infoS);
    }
    void RWDb::readSingleInfo(std::string dbName, std::map<std::string, std::string> &infoS)
    {
        dbOpera.readSingleInfo(dbName, infoS);
    }
    std::string RWDb::getUserInfoLastCount(){
        std::vector<std::map<std::string, std::string>> fillContent;
        fillContent = readUserInfo();
        std::string lastCount;
        int count = dbOpera.countOfTable(USERINFODBNAME);
        bool flag=false;
        for (int i=0;i<int(fillContent.size());i++){
            int curIndex= std::atoi(fillContent[i]["UserNo"].c_str());
            if (curIndex > count){
                flag=true;
                count = curIndex;
            }
        }
        std::string strcount;
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << count+1;
        return oss.str();
    }


    std::vector<std::map<std::string, std::string>> RWDb::readUserInfo()
    {
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(userInfoName.size()); i++)
        {
            infoS[userInfoName[i]] = "";
        }
        fillContent = readRecord(USERINFODBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> RWDb::readUserGroupInfo(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(userGroupInfoName.size()); i++)
        {
            infoS[userGroupInfoName[i]] = "";
        }
        fillContent = readRecord(USERGROUPDBNAME,infoS);
        return fillContent;
    }
    void RWDb::deleteAuditTrail(){
        std::map<std::string,std::string> info;
        info["lastAuditTrailDB"]="";
        logOpera.recordSingleInfo(AUDITTRAILDBRECORD,info);
        std::vector<std::string> tableNames = getAllAuditLogTables();
        for (int i=0;i<int(tableNames.size());i++){
            logOpera.deleteDB(tableNames[i]);
        }
    }

    void RWDb::writeAuditTrailLog(const std::string &logContent)
    {
        std::map<std::string,std::string> noLoginRecordInfo;
        noLoginRecordInfo["track"] = "";
        noLoginRecordInfo["debug"]="";
        readSingleInfo(SYSTEMINFODBNAME,noLoginRecordInfo);
        // if (noLoginRecordInfo["track"] != "true"&&noLoginRecordInfo["debug"]!="true") return;
        std::map<std::string, std::string> infoS = {
            {"Operator", readLoginName()},
            {"Time", HGExactTime::currentTime().toStringFromYearToSecAndZone()},
            {"LogContent", logContent}};

        std::map<std::string,std::string> info;
        info["lastAuditTrailDB"]="";
        logOpera.readSingleInfo(AUDITTRAILDBRECORD,info);
        std::string dbName="";
        if (info["lastAuditTrailDB"] == "")
        {
            HGExactTime curTime = HGExactTime::currentTime();
            dbName = "AuditTrailLog_" + curTime.toStringFromYearToDay();
            printf("blank: %s\n",dbName.c_str());
        }
        else
        {
            int count = logOpera.countOfTable(info["lastAuditTrailDB"]);
            if (count >= 3000)
            {
                HGExactTime curTime = HGExactTime::currentTime();
                dbName = "AuditTrailLog_" + curTime.toStringFromYearToDay();
                printf(">=3000: %s\n",dbName.c_str());
            }
            else
            {
                dbName = info["lastAuditTrailDB"];
                printf("<3000: %s\n",dbName.c_str());
            }
        }
        logOpera.writeRecord(dbName, "Time", infoS);
        info["lastAuditTrailDB"]=dbName;
        logOpera.recordSingleInfo(AUDITTRAILDBRECORD,info);
    }
    void RWDb::writeSerialPortInfo(const std::vector<std::string> &serialPorts)
    {
        std::map<std::string, std::string> info;
        dbOpera.clearDB(SERIALPORTDBNAME);
        std::string keyName = "序号";
        for (int i = 0; i < int(serialPorts.size()); i++)
        {
            info["序号"] = std::to_string(i + 1);
            info["名称"] = serialPorts[i];
            dbOpera.writeRecord(SERIALPORTDBNAME, keyName, info);
        }
    }
    void RWDb::writeUserInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.writeRecord(USERINFODBNAME, "UserNo",infoS);
        dbOpera.sortTable(USERINFODBNAME,"UserNo");
    }
    void RWDb::writeUserGroupInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.writeRecord(USERGROUPDBNAME, "GroupName",infoS);
        dbOpera.sortTable(USERGROUPDBNAME,"GroupName");
    }
    void RWDb::deleteUserGroupInfo(const std::string &value){
        dbOpera.deleteRecord(USERGROUPDBNAME,"GroupName",value);
    }
    std::string RWDb::getLastLoginTime()
    {
        std::map<std::string, std::string> noLoginRecordInfo;
        noLoginRecordInfo["lastLoginTime"] = "";
        readSingleInfo(SYSTEMINFODBNAME,noLoginRecordInfo);
        return noLoginRecordInfo["lastLoginTime"];
    }
    bool RWDb::isLoginRightTime()
    {
        std::string lastLoginTime = getLastLoginTime();
        if (lastLoginTime == "")
            return true;
        HGExactTime curTimer = HGExactTime::currentTime();
        HGExactTime deadlineTimer = HGExactTime::currentTime();
        TIME_STRUECT tmS;
        decodeStandardTime(lastLoginTime, tmS);
        deadlineTimer.tm_year = tmS.year; 
        deadlineTimer.tm_mon = tmS.month; 
        deadlineTimer.tm_mday = tmS.day; 
        if (curTimer < deadlineTimer)
        {
            return false;
        }
        return true;
    }

    void RWDb::deleteRecord(std::string dbName, std::string deleteKey, std::string deleteValue)
    {
        dbOpera.deleteRecord(dbName, deleteKey, deleteValue);
    }
    std::vector<std::string> RWDb::readSerialPortInfo(){
        std::map<std::string,std::string> info;
        info["序号"]="";
        info["名称"]="";
        std::vector<std::map<std::string,std::string>> contents;
        contents=dbOpera.readRecord(SERIALPORTDBNAME,info);
        std::vector<std::string> ports;
        for (auto port : contents){
            ports.push_back(port["名称"]);
        }
        return ports;
    }
    void RWDb::deleteDB(const std::string &dbName)
    {
        dbOpera.deleteDB(dbName);
    }
    std::vector<std::string> RWDb::getAllTables(std::string key)
    {
        std::vector<std::string> returnNames;
        std::vector<std::string> names = dbOpera.getAllTables();
        for (const auto &name : names)
        {
            if (name.find(key) != std::string::npos)
            {
                returnNames.push_back(name);
            }
        }
        return returnNames;
    }





void RWDb::copyTable(const std::string& sourceDBName,
            const std::string &targetDbName,const std::string &tableName){
    if (targetDbName==(RWDb::readCurDirPath()+DATABASE_PATH+DB_PATH)){
        bool flag = dbOpera.copyTable(sourceDBName, tableName);
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        if (flag)
        {
            HGLogService::getInstance(HGLogService::getLogPath())->logInfo("copy table " + tableName + " success");
        }
        else
        {
            HGLogService::getInstance(HGLogService::getLogPath())->logError("copy table " + tableName + " fail");
        }
#endif
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        if (tableName.find("Flow_") != std::string::npos)
        {
            HGSaveDataToDB dbOperatorTemp;
            dbOperatorTemp.openDB(sourceDBName);
            for (int i = 0; i < int(flowInfoName.size()); i++)
            {
                infoS[flowInfoName[i]] = "";
            }
            fillContent = dbOperatorTemp.readRecord(FLOWMANAGEDBNAME, infoS);

            for (int i = 0; i < int(fillContent.size()); i++)
            {
                if (fillContent[i]["DBName"] == tableName)
                {
                    fillContent[i]["序号"] = std::to_string(readFlowInfo().size() + 1);
                    dbOpera.writeRecord(FLOWMANAGEDBNAME, "序号", fillContent[i]);
                }
            }

            dbOpera.sortTable(FLOWMANAGEDBNAME,"序号");
            dbOperatorTemp.closeDB();
        }else if (tableName.find("Channel_") != std::string::npos)
        {
            HGSaveDataToDB dbOperatorTemp;
            dbOperatorTemp.openDB(sourceDBName);
            for (int i=0;i<int(channelInfoName.size());i++){
                infoS[channelInfoName[i]]="";
            }
            
            fillContent = dbOperatorTemp.readRecord(CHANNELMANAGENAME, infoS);

            for (int i = 0; i < int(fillContent.size()); i++)
            {
                if (fillContent[i]["DBName"] == tableName)
                {
                    fillContent[i]["序号"] = std::to_string(readChannelInfo().size() + 1);
                    dbOpera.writeRecord(CHANNELMANAGENAME, "序号", fillContent[i]);
                }
            }
            dbOpera.sortTable(CHANNELMANAGENAME,"序号");
            dbOperatorTemp.closeDB();
        }else if (tableName.find("Method_") != std::string::npos)
        {
            HGSaveDataToDB dbOperatorTemp;
            dbOperatorTemp.openDB(sourceDBName);
            for (int i = 0; i < int(methodInfoName.size()); i++)
            {
                infoS[methodInfoName[i]] = "";
            }
            fillContent =dbOperatorTemp.readRecord(METHODMANAGENAME, infoS);
            for (int i = 0; i < int(fillContent.size()); i++)
            {
                if (fillContent[i]["DBName"] == tableName)
                {
                    fillContent[i]["序号"] = std::to_string(readMethodInfo().size() + 1);
                    dbOpera.writeRecord(METHODMANAGENAME, "序号", fillContent[i]);
                }
            }
            dbOpera.sortTable(METHODMANAGENAME,"序号");
            dbOperatorTemp.closeDB();
        }else if (tableName.find("Reagent_") != std::string::npos)
        {
            HGSaveDataToDB dbOperatorTemp;
            dbOperatorTemp.openDB(sourceDBName);
            std::vector<REAGENT> reagents;
            for (int i=0;i<int(reagentInfoName.size());i++){
                infoS[reagentInfoName[i]]="";
            }
            fillContent=dbOperatorTemp.readRecord(tableName,infoS);
            std::map<std::string,int> reagentDeviceTableNameCount;
            for (int i=0;i<int(fillContent.size());i++){
                reagentDeviceTableNameCount[fillContent[i]["DBName"]]=1;
            }
            dbOperatorTemp.closeDB();
            for (auto deviceTableName:reagentDeviceTableNameCount){
                bool flag=dbOpera.copyTable(sourceDBName,deviceTableName.first);
            }
        }
        else if (tableName.find("TaskSequence_") != std::string::npos)
        { // nothing
        }
    }
}





void RWDb::writeScannerInfo(const std::map<std::string,std::string> &info){
    dbOpera.recordSingleInfo(ScannerDBName, info);
}
    std::map<std::string,std::string> RWDb::readScannerInfo(){
        std::map<std::string,std::string> info;
        for (auto name:scannerInfoName){
            info[name]="";
        }
        dbOpera.readSingleInfo(ScannerDBName,info);
        return info;
    }

std::vector<std::map<std::string,std::string>> RWDb::searchAuditTrailLogAcrossTables(
    const std::string &key, 
    const HGExactTime &timeFrom, 
    const HGExactTime &timeTo)
{
    std::vector<std::map<std::string,std::string>> allResults;
    std::vector<std::string> tableNames = getAllAuditLogTables();
    
    std::map<std::string,std::string> infoS = {
        {"Operator",""},
        {"Time",""},
        {"LogContent",""}
    };

    for (const auto& tableName : tableNames) {
        std::vector<std::map<std::string,std::string>> tableLogs = logOpera.readRecord(tableName, infoS);
        
        for (auto& log : tableLogs) {
            bool match = true;
            
            if (!key.empty()) {
                if (log["Time"].find(key) == std::string::npos &&
                    log["Operator"].find(key) == std::string::npos &&
                    log["LogContent"].find(key) == std::string::npos) {
                    match = false;
                }
            }
            
            if (match) {
                std::string timeStr = log["Time"];
                TIME_STRUECT timeS;
                decodeStandardTime(timeStr, timeS);
                HGExactTime testTimer = HGExactTime::currentTime();
                testTimer.tm_year = timeS.year;
                testTimer.tm_mon = timeS.month;
                testTimer.tm_mday = timeS.day;
                
                if (testTimer < timeFrom || testTimer > timeTo) {
                    match = false;
                }
            }
            
            if (match) {
                log["_tableName"] = tableName;
                allResults.push_back(log);
            }
        }
    }
    
    return allResults;
}

std::vector<std::map<std::string,std::string>> RWDb::searchAuditTrailLogAcrossTablesWithPagination(
    const std::string &key, 
    const HGExactTime &timeFrom, 
    const HGExactTime &timeTo,
    int pageIndex,
    int pageSize,
    int *totalCount)
{
    std::vector<std::map<std::string,std::string>> allResults;
    std::vector<std::string> tableNames = getAllAuditLogTables();
    
    std::map<std::string,std::string> infoS = {
        {"Operator",""},
        {"Time",""},
        {"LogContent",""}
    };

    for (const auto& tableName : tableNames) {
        std::vector<std::map<std::string,std::string>> tableLogs = logOpera.readRecord(tableName, infoS);
        
        for (auto& log : tableLogs) {
            bool match = true;
            
            if (!key.empty()) {
                std::string timeValue = log["Time"];
                std::string operatorValue = log["Operator"];
                std::string logContentValue = log["LogContent"];
                if (timeValue.find(key) == std::string::npos &&
                    operatorValue.find(key) == std::string::npos &&
                    logContentValue.find(key) == std::string::npos) {
                    match = false;
                }
            }
            
            if (match) {
                std::string timeStr = log["Time"];
                TIME_STRUECT timeS;
                decodeStandardTime(timeStr, timeS);
                HGExactTime testTimer = HGExactTime::currentTime();
                testTimer.tm_year = timeS.year;
                testTimer.tm_mon = timeS.month;
                testTimer.tm_mday = timeS.day;
                
                if (testTimer < timeFrom || testTimer > timeTo) {
                    match = false;
                }
            }
            
            if (match) {
                log["_tableName"] = tableName;
                allResults.push_back(log);
            }
        }
    }
    
    if (totalCount) {
        *totalCount = allResults.size();
    }
    
    std::vector<std::map<std::string,std::string>> pagedResults;
    int startIndex = pageIndex * pageSize;
    
    if (startIndex >= int(allResults.size())) {
        return pagedResults;
    }
    
    int endIndex = std::min(startIndex + pageSize, int(allResults.size()));
    for (int i = startIndex; i < endIndex; i++) {
        pagedResults.push_back(allResults[i]);
    }
    
    return pagedResults;
}

int RWDb::searchAuditTrailLogCount(
    const std::string &key, 
    const HGExactTime &timeFrom, 
    const HGExactTime &timeTo)
{
    int count = 0;
    std::vector<std::string> tableNames = getAllAuditLogTables();
    bool hasTimeFilter = !(timeFrom.tm_year == 0 && timeTo.tm_year == 0);

    for (const auto& tableName : tableNames) {
        std::ostringstream sql;
        sql << "SELECT COUNT(*) as count FROM " << tableName;
        
        std::vector<std::string> conditions;
        
        if (!key.empty()) {
            conditions.push_back("(Time LIKE '%" + key + "%' OR Operator LIKE '%" + key + "%' OR LogContent LIKE '%" + key + "%')");
        }
        
        if (hasTimeFilter) {
            char timeFromStr[32];
            char timeToStr[32];
            sprintf(timeFromStr, "%04d-%02d-%02d 00:00:00", timeFrom.tm_year, timeFrom.tm_mon, timeFrom.tm_mday);
            sprintf(timeToStr, "%04d-%02d-%02d 23:59:59", timeTo.tm_year, timeTo.tm_mon, timeTo.tm_mday);
            conditions.push_back("Time >= '" + std::string(timeFromStr) + "' AND Time <= '" + std::string(timeToStr) + "'");
        }
        
        if (!conditions.empty()) {
            sql << " WHERE ";
            for (size_t i = 0; i < conditions.size(); i++) {
                sql << conditions[i];
                if (i < conditions.size() - 1) {
                    sql << " AND ";
                }
            }
        }
        
        sql << ";";
        
        std::vector<std::map<std::string,std::string>> result;
        if (logOpera.readData(sql.str(), result) && !result.empty()) {
            count += std::atoi(result[0]["count"].c_str());
        }
    }
    
    return count;
}

std::vector<std::map<std::string,std::string>> RWDb::searchAuditTrailLogAcrossTablesWithPaginationOptimized(
    const std::string &key, 
    const HGExactTime &timeFrom, 
    const HGExactTime &timeTo,
    int pageIndex,
    int pageSize,
    int *totalCount)
{
    std::vector<std::map<std::string,std::string>> allResults;
    std::vector<std::string> tableNames = getAllAuditLogTables();
    
    int skipCount = pageIndex * pageSize;
    int collectCount = 0;
    int matchCount = 0;
    bool hasTimeFilter = !(timeFrom.tm_year == 0 && timeTo.tm_year == 0);

    for (const auto& tableName : tableNames) {
        if (collectCount >= pageSize) {
            break;
        }

        std::ostringstream sql;
        sql << "SELECT Operator, Time, LogContent FROM " << tableName;
        
        std::vector<std::string> conditions;
        
        if (!key.empty()) {
            conditions.push_back("(Time LIKE '%" + key + "%' OR Operator LIKE '%" + key + "%' OR LogContent LIKE '%" + key + "%')");
        }
        
        if (hasTimeFilter) {
            char timeFromStr[32];
            char timeToStr[32];
            sprintf(timeFromStr, "%04d-%02d-%02d 00:00:00", timeFrom.tm_year, timeFrom.tm_mon, timeFrom.tm_mday);
            sprintf(timeToStr, "%04d-%02d-%02d 23:59:59", timeTo.tm_year, timeTo.tm_mon, timeTo.tm_mday);
            conditions.push_back("Time >= '" + std::string(timeFromStr) + "' AND Time <= '" + std::string(timeToStr) + "'");
        }
        
        if (!conditions.empty()) {
            sql << " WHERE ";
            for (size_t i = 0; i < conditions.size(); i++) {
                sql << conditions[i];
                if (i < conditions.size() - 1) {
                    sql << " AND ";
                }
            }
        }
        
        sql << ";";
        
        std::vector<std::map<std::string,std::string>> tableLogs;
        if (!logOpera.readData(sql.str(), tableLogs)) {
            continue;
        }
        
        for (auto& log : tableLogs) {
            matchCount++;
            
            if (matchCount > skipCount && collectCount < pageSize) {
                log["_tableName"] = tableName;
                allResults.push_back(log);
                collectCount++;
            }
            
            if (collectCount >= pageSize) {
                break;
            }
        }
    }
    
    if (totalCount) {
        *totalCount = matchCount;
    }
    
    return allResults;
}

}