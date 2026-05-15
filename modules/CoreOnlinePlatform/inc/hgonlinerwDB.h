#ifndef HGONLINERWDB_H
#define HGONLINERWDB_H

#include <map>
#include <vector>
#include <list>
#include "hgonlineplatformmodule.h"
#include "structUpperPcToLowerPC.h"
#include <stdint.h>
#include <stdlib.h>
#include "hgsavedatatodb.h"
#include "rwDb.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE{
    // #define ENABLE_CAS6

    #define CAS6_BININFODBNAME "CAS6BinInfo"
    #define DeviceFromLPCManageInfoDBNAME "DeviceFromLowerPCManageInfo"
    
    #define MATERIALINFODBNAME "MaterialInfo"

    typedef struct tagDeviceName{
        std::string pinyin;
        std::string zhongwen;
        tagDeviceName(){
            pinyin="";
            zhongwen="";
        }
        tagDeviceName(std::string _zhongwen,std::string _pinyin)
        {
            pinyin=_pinyin;
            zhongwen=_zhongwen;
        }
    } DeviceName;

    extern std::vector<std::string> taskInfoName;
    extern std::vector<std::string> flowInfoName;
    extern std::vector<std::string> insertDeviceNameOrder;
    extern std::vector<std::string> gNoexpandDeviceNames;
    extern std::map<std::string,std::map<std::string,std::vector<DeviceName>>> gDeviceNames;

typedef struct tagSendInfo{
    std::vector<std::string> type;
    std::string sendData;
    std::string deviceType;
    int interfaceCount;
    std::string curTableName;
    int index;
    tagSendInfo(){
        index=0;
        interfaceCount=0;
        curTableName="";
    }
    tagSendInfo(std::vector<std::string> _type,
        std::string _sendData)
    {
        type=_type;
        sendData=_sendData;
    }
} SendInfo;

    class HGOnlineRWDB : public RWDb
    {
    public:
        HGOnlineRWDB();
        ~HGOnlineRWDB();

        
        static int readLoginAuthority();
        static std::string readEnterComInfo();
        static std::vector<std::map<std::string,std::string>> readDeviceDataInfo(std::string dbName);
        
        
        
        
        static std::vector<DeviceInterface> readDeviceInfo(std::string dbName);
        
        static std::vector<std::map<std::string,std::string>> readMaterialInfo();
        static std::vector<std::string> getExistCAS6DeviceNames();
        
        static void writeCAS6DeviceInfo(const std::vector<std::map<std::string,std::string>> &info);
        static void writeMaterialInfo(const std::map<std::string,std::string> &infoS);
        static void writeRecord(std::string dbName,std::string keyName,const std::map<std::string,std::string> &infoS);
        
        
        static void writeResultRecord(bool coverFlag,std::string dbName,const std::vector<std::map<std::string,std::string>> &infoSS);
        static void writeEnterComInfo(const std::string &comInfo);
        static void writeLowerPCDeviceInfo(const UpperPCToLowerPC &upperToLowerStruct);
        static std::map<std::string, std::string> decodeDeviceDataInfoAndWrite(const UpperPCToLowerPC &upperToLowerStruct, const Task &curTaskInfo);
        static void writeDeviceDataInfo(std::string dbName,const std::map<std::string,std::string> &contents);
        static void writeDeviceFromLPCInfo(std::string dbName,const std::map<std::string,std::string> &contents);
        static void writeDeviceFromLPCManageInfo(std::string dbName,const std::map<std::string,std::string> &contents);

        
        

        
        
        
        static std::map<std::string,std::string> getDeviceInfoMap(int index,DeviceInterface info);
        static DeviceInterface getDeviceInterfaceFromMap(std::map<std::string,std::string> info);
        
        
        static std::vector<std::string> getDevicePortInfos();
        static uint16_t getDeviceTypeFromCanId(const uint8_t &canId);
        static std::vector<std::string> getExistDeviceNames();


        static void setDeviceFromLPCManageOffline();


        static bool isCanIdOnline(const uint8_t &canId);
        static bool isFaceIndexOnline(const uint8_t &canId,const uint8_t &faceIndex);

        
        static void clearDeviceFromLPCManageInfo(std::string dbName);
        static void clearCAS6DeviceStatus();

        
        static void deleteDeviceFromLPCManageInfo(std::string dbName);

        
        

    private:
        static SendInfo response;
    };
}
#ifdef __cplusplus
}
#endif
#endif // HGONLINERWDB_H