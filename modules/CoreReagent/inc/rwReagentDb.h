#ifndef RWREAGENTDB_H
#define RWREAGENTDB_H

#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
     typedef struct tagReagentLinkDevice{
        int index;
        std::string name;
        std::string interfaceName;
        std::string linkState;
        std::string channel;
        std::string choice;
        tagReagentLinkDevice(){
            Clear();
        }
        void Clear(){
            index=-1;
            interfaceName="";
            name="";
            linkState="";
            channel="";
            choice="";
        }
        bool operator==(const tagReagentLinkDevice& obj) const{
            bool ret=false;
            if (index==obj.index &&
                name==obj.name &&
                interfaceName==obj.interfaceName &&
                linkState==obj.linkState &&
                channel==obj.channel &&
                choice==obj.choice)
            {
                ret=true;
            }
            return ret;
        }
        tagReagentLinkDevice(const tagReagentLinkDevice& obj){
            Copy(obj);
        }
        void Copy(const tagReagentLinkDevice& obj){
            index=obj.index;
            name=obj.name;
            interfaceName=obj.interfaceName;
            linkState=obj.linkState;
            channel=obj.channel;
            choice=obj.choice;
        }
        tagReagentLinkDevice& operator=(const tagReagentLinkDevice& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagReagentLinkDevice& obj)const
        {
            return (!(*this==obj));
        }
        tagReagentLinkDevice* Clone() const
        {
            return (new tagReagentLinkDevice(*this));
        }
    } ReagentLinkDevice,*PReagentLinkDevice;
    typedef struct tagReagent{
        int index;
        std::string type;
        std::string serialNumber;
        std::string name;
        std::string concertraction;
        std::string calibrationDate;
        std::string calibrationCircle;
        std::string reagentRemainAmount;
        std::string currentState;
        std::string dbName;
        std::map<int,ReagentLinkDevice> linkDevices;
        tagReagent(){
            Clear();
        }
        void Clear(){
            index=-1;
            name="";
            type="";
            serialNumber="";
            concertraction="";
            calibrationDate="";
            calibrationCircle="";
            reagentRemainAmount="";
            currentState="";
            dbName="";
            for(auto device : linkDevices) device.second.Clear();
        }
        bool operator==(const tagReagent& obj) const{
            bool ret=false;
            if (index==obj.index &&
                name==obj.name &&
                type==obj.type &&
                serialNumber==obj.serialNumber &&
                concertraction==obj.concertraction &&
                calibrationDate==obj.calibrationDate &&
                calibrationCircle==obj.calibrationCircle &&
                reagentRemainAmount==obj.reagentRemainAmount &&
                currentState==obj.currentState &&
                dbName==obj.dbName &&
                linkDevices.size()==obj.linkDevices.size() &&
                linkDevices==obj.linkDevices)
            {
                // std::map<int,ReagentLinkDevice>::iterator it=obj.linkDevices.begin();
                // std::map<int,ReagentLinkDevice>::iterator it1=linkDevices.begin();
                // for (;it!=obj.linkDevices.end(),it1!=linkDevices.end();it++,it1++) {
                //     if (it->second!=it1->second)
                //     {
                //         return false;
                //     }
                // }
                ret=true;
            }
            return ret;
        }
        tagReagent(const tagReagent& obj){
            Copy(obj);
        }
        void Copy(const tagReagent& obj){
            index=obj.index;
            name=obj.name;
            type=obj.type;
            serialNumber=obj.serialNumber;
            concertraction=obj.concertraction;
            calibrationDate=obj.calibrationDate;
            calibrationCircle=obj.calibrationCircle;
            reagentRemainAmount=obj.reagentRemainAmount;
            currentState=obj.currentState;
            dbName=obj.dbName;
            for (const auto &device : obj.linkDevices)
                linkDevices[device.first]=device.second;
        }
        tagReagent& operator=(const tagReagent& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagReagent& obj)const
        {
            return (!(*this==obj));
        }
        tagReagent* Clone() const
        {
            return (new tagReagent(*this));
        }
    } REAGENT,*PREAGENT;

    #define ReagentLinkDeviceDBName "ReagentDevice"
    #define ReagentDBName "Reagent"

    class RWReagentDb {
        public:
            RWReagentDb();
            ~RWReagentDb();

            //-------------------------reagent --------------------------------//
            static std::vector<REAGENT> readReagentInfo(const std::string &reagentTableName);
            static std::map<std::string,std::string> getMapFromReagent(const REAGENT &info);
            static REAGENT getFromReagentMap(std::map<std::string,std::string> regentInfo,std::vector<std::map<std::string,std::string>> deviceInfos);
            static const std::vector<std::string> getReagentNoEditName();
            static const std::vector<std::string> getReagentLinkDeviceNoEditName();   
            static void writeReagentRecord(bool coverFlag,const std::string& tableName,const std::vector<REAGENT> &infoSS);
            static std::vector<std::map<std::string,std::string>> readReagentDeviceInfo(std::string dbName);
            static std::map<std::string,std::string> getMapFromDevices(const ReagentLinkDevice &devices);
           

        protected:
    };
}
#ifdef __cplusplus
}
#endif
#endif // RWREAGENTDB_H
