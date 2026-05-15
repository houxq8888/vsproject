#ifndef RWDB_H
#define RWDB_H

#include "hgsavedatatodb.h"
#include "HGExactTime.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
    typedef struct tagUserInfoS{
    std::string userno;
    std::string username;
    std::string userpwd;
    std::string authority;
    std::string department;
    tagUserInfoS(){
        username="";
        userpwd="";
        authority="";
        department="";
        userno="";
    }
    tagUserInfoS(std::string _name,
        std::string pwd,
        std::string _authority,
        std::string _depart,
        std::string _userno)
    {
        username=_name;
        userpwd=pwd;
        authority=_authority;
        department=_depart;
        userno=_userno;
    }
    bool operator==(const tagUserInfoS& obj) const{
        bool ret=false;
        if (username==obj.username &&
            userpwd==obj.userpwd &&
            authority==obj.authority &&
            department==obj.department &&
            userno==obj.userno)
        {
            ret=true;
        }
        return ret;
    }
    tagUserInfoS(const tagUserInfoS& obj){
        Copy(obj);
    }
    void Copy(const tagUserInfoS& obj){
        username=obj.username;
        userpwd=obj.userpwd;
        authority=obj.authority;
        department=obj.department;
        userno=obj.userno;
    }
    tagUserInfoS& operator=(const tagUserInfoS& obj){
        Copy(obj);
        return *this;
    }
    bool operator!=(const tagUserInfoS& obj)const
    {
        return (!(*this==obj));
    }
    tagUserInfoS* Clone() const
    {
        return (new tagUserInfoS(*this));
    }
} UserInfoS, *PUserInfoS;

    typedef struct tagDeviceInterface{
        std::string rootName;
        std::string name;
        std::string param;
        tagDeviceInterface(){
            Clear();
        }
        void Clear(){
            rootName="";
            name="";
            param="";
        }
        bool operator==(const tagDeviceInterface& obj) const{
            bool ret=false;
            if (rootName==obj.rootName &&
                name==obj.name &&
                param==obj.param)
            {
                ret=true;
            }
            return ret;
        }
        tagDeviceInterface(const tagDeviceInterface& obj){
            Copy(obj);
        }
        void Copy(const tagDeviceInterface& obj){
            rootName=obj.rootName;
            name=obj.name;
            param=obj.param;
        }
        tagDeviceInterface& operator=(const tagDeviceInterface& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagDeviceInterface& obj)const
        {
            return (!(*this==obj));
        }
        tagDeviceInterface* Clone() const
        {
            return (new tagDeviceInterface(*this));
        }
    }DeviceInterface,*PDeviceInterface;
    



    #define SYSTEMINFODBNAME "NoLoginInfo"
    #define AUDITTRAILLOGDBNAME "AuditTrailLog"
    #define USERINFODBNAME "UserInfo"
    #define USERGROUPDBNAME "UserGroupInfo"
    #define SERIALPORTDBNAME "SerialPortInfo"
    #define AUDITTRAILDBRECORD "AUDITTRAILDBRECORD"
    #define DATACHARTMANAGENAME "DataChartManageInfo"
    #define ScannerDBName "ScannerInfo"


    #define DB_PATH "/HG.db"
    #define DATA_DB_PATH "/HGData.db"
    #define LOG_DB_PATH "/HGLog.db"

    class RWDb{
        public:
            RWDb();
            ~RWDb();

            static void openDB(const std::string &basePath);
            static void closeDB();
            static void clearDB(const std::string& tableName);

            //------------------------base -----------------------------//
            static std::string readCurDirPath();
            static void copyTable(const std::string& sourceDBName,
                const std::string &targetDbName,const std::string &tableName);

            //------------------------read & write---------------------------//
            static void recordSingleInfo(std::string dbName, const std::map<std::string, std::string> &infoS);
            static std::vector<std::map<std::string,std::string>> readRecord(std::string dbName, std::map<std::string,std::string> &infoS);
            static void readSingleInfo(std::string dbName, std::map<std::string, std::string> &infoS);
            static void deleteRecord(std::string dbName, std::string deleteKey, std::string deleteValue);
            static void deleteDB(const std::string &dbName);
            static std::vector<std::string> getAllTables(std::string key);

            //------------------------login------------------------------//
            static std::string readLoginName();
            static bool isLoginRightTime();

            //------------------------user------------------------------//
            static std::string getUserInfoLastCount();
            static std::vector<std::map<std::string,std::string>> readUserInfo();
            static std::vector<std::map<std::string,std::string>> readUserGroupInfo();
            static void writeUserInfo(const std::map<std::string,std::string> &infoS);
            static void writeUserGroupInfo(const std::map<std::string,std::string> &infoS);
            static void deleteUserGroupInfo(const std::string &value);
            static std::vector<std::map<std::string,std::string>> searchUserFromKeyword(const std::string& key);
            static std::vector<std::map<std::string,std::string>> searchUserFromKeywordWithHighlight(
                const std::string& key, 
                int pageIndex = 0, 
                int pageSize = 50, 
                int* totalCount = nullptr);
            //-------------------------audit log -----------------------------//
            static std::vector<std::map<std::string,std::string>> readAuditTrailLog(const std::string &tableName="");
            static std::vector<std::string> getAllAuditLogTables();
            static void writeAuditTrailLog(const std::string &logContent);
            static int readAuditTrailLogCount(const std::string &tableName="");
            static void deleteAuditTrail();
            static std::vector<std::map<std::string,std::string>> searchAuditTrailLogAcrossTables(
                const std::string &key="", 
                const HGExactTime &timeFrom=HGExactTime(), 
                const HGExactTime &timeTo=HGExactTime());
            static std::vector<std::map<std::string,std::string>> searchAuditTrailLogAcrossTablesWithPagination(
                const std::string &key="", 
                const HGExactTime &timeFrom=HGExactTime(), 
                const HGExactTime &timeTo=HGExactTime(),
                int pageIndex=0,
                int pageSize=1000,
                int *totalCount=nullptr);
            static int searchAuditTrailLogCount(
                const std::string &key="", 
                const HGExactTime &timeFrom=HGExactTime(), 
                const HGExactTime &timeTo=HGExactTime());
            static std::vector<std::map<std::string,std::string>> searchAuditTrailLogAcrossTablesWithPaginationOptimized(
                const std::string &key="", 
                const HGExactTime &timeFrom=HGExactTime(), 
                const HGExactTime &timeTo=HGExactTime(),
                int pageIndex=0,
                int pageSize=1000,
                int *totalCount=nullptr);


            //-------------------------serial port------------------------------//
            static void writeSerialPortInfo(const std::vector<std::string> &serialPorts);
            static std::vector<std::string> readSerialPortInfo();
            static std::string getLastLoginTime();

           
            //----------------------------scanner---------------------------//
            static void writeScannerInfo(const std::map<std::string,std::string> &info);
            static std::map<std::string,std::string> readScannerInfo();

        protected:
            // static SendInfo response;
            static HGSaveDataToDB dbOpera;
            static HGSaveDataToDB dataOpera;
            static HGSaveDataToDB logOpera;
    };
}
#ifdef __cplusplus
}
#endif
#endif