#ifndef RWCHANNELDB_H
#define RWCHANNELDB_H

#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
        typedef struct tagModuleOfChannel{
        int index;
        std::string name;
        std::string param;
        std::string nameOfSameModule;
        int indexOfSameModule;
        std::string typeName;
        tagModuleOfChannel(){
            Clear();
        }
        void Clear(){
            index=-1;
            name="";
            param="";
            typeName="";
            nameOfSameModule="";
            indexOfSameModule=-1;
        }
        bool operator==(const tagModuleOfChannel& obj) const{
            bool ret=false;
            if (index==obj.index &&
                name==obj.name &&
                typeName==obj.typeName &&
                param==obj.param &&
                nameOfSameModule==obj.nameOfSameModule &&
                indexOfSameModule==obj.indexOfSameModule)
            {
                ret=true;
            }
            return ret;
        }
        tagModuleOfChannel(const tagModuleOfChannel& obj){
            Copy(obj);
        }
        void Copy(const tagModuleOfChannel& obj){
            index=obj.index;
            name=obj.name;
            param=obj.param;
            typeName=obj.typeName;
            nameOfSameModule=obj.nameOfSameModule;
            indexOfSameModule=obj.indexOfSameModule;
        }
        tagModuleOfChannel& operator=(const tagModuleOfChannel& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagModuleOfChannel& obj)const
        {
            return (!(*this==obj));
        }
        tagModuleOfChannel* Clone() const
        {
            return (new tagModuleOfChannel(*this));
        }
    } ModuleOfChannel, *PModuleOfChannel;
    typedef struct tagChannel{
        int channel;
        std::string dbName;
        std::vector<ModuleOfChannel> modules;
        tagChannel(){
            Clear();
        }
        void Clear(){
            channel=-1;
            dbName="";
            modules.clear();
        }
        bool operator==(const tagChannel& obj) const{
            bool ret=false;
            if (channel==obj.channel &&
                dbName==obj.dbName &&
                modules==obj.modules)
            {
                ret=true;
            }
            return ret;
        }
        tagChannel(const tagChannel& obj){
            Copy(obj);
        }
        void Copy(const tagChannel& obj){
            channel=obj.channel;
            modules.clear();
            for (int i=0;i<int(obj.modules.size());i++){
                modules.push_back(obj.modules[i]);
            }
            dbName=obj.dbName;
        }
        tagChannel& operator=(const tagChannel& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagChannel& obj)const
        {
            return (!(*this==obj));
        }
        tagChannel* Clone() const
        {
            return (new tagChannel(*this));
        }
    }Channel,*PChannel;

    #define CHANNELMANAGENAME "ChannelInfo"
    #define ChannelDBName "Channel"

    class RWChannelDb {
        public:
            RWChannelDb();
            ~RWChannelDb();

           
            //-------------------------channel --------------------------------//
            static std::vector<std::map<std::string,std::string>> readChannelInfo();
            static void writeChannelManageRecord(const std::map<std::string,std::string> &infoS);
            static std::map<std::string,std::string> getChannelMap(int index,Channel channel);
            static void clearChannelManageRecord();
            static void deleteAllChannelModuleDB();    
 
            //----------------------------module ---------------------------//
            static std::vector<ModuleOfChannel> readModulesParam(std::string dbName);
            static std::vector<std::map<std::string,std::string>> getModulesMap(Channel info);
            static void writeModulesRecord(std::string dbName,bool coverFlag,const std::vector<std::map<std::string,std::string>> &infoSS);
        

        protected:
    };
}
#ifdef __cplusplus
}
#endif
#endif // RWCHANNELDB_H
