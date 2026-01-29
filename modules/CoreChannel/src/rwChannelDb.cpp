#include "rwChannelDb.h"


namespace HGMACHINE{
  
    std::vector<std::map<std::string,std::string>> RWChannelDb::readChannelInfo(){
        std::vector<std::map<std::string,std::string>> fillContent;
        std::map<std::string,std::string> infoS;
        for (int i=0;i<int(channelInfoName.size());i++){
            infoS[channelInfoName[i]]="";
        }
        fillContent=readRecord(CHANNELMANAGENAME,infoS);
        return fillContent;
    }

    void RWChannelDb::writeChannelManageRecord(const std::map<std::string, std::string> &infoS)
    {
        dbOpera.writeRecord(CHANNELMANAGENAME, "序号", infoS);
        dbOpera.sortTable(CHANNELMANAGENAME,"序号");
    }

    std::map<std::string,std::string> RWChannelDb::getChannelMap(int index,Channel channel){
        std::map<std::string,std::string> infoS;
        infoS["序号"]=std::to_string(index);
        infoS["通道"]=std::to_string(channel.channel);
        infoS["检测器"]="";
        infoS["检测方法"]="";
        infoS["DBName"]=ChannelDBName+std::string("_")+infoS["序号"];
        for (auto content : channel.modules){
            if (content.typeName=="检测器"){
                infoS["检测器"]=content.name;
                infoS["检测方法"]=content.param;
            }
        }
        return infoS;
    }

    void RWChannelDb::clearChannelManageRecord(){
        clearDB(CHANNELMANAGENAME);
    }

    void RWChannelDb::deleteAllChannelModuleDB(){
        std::vector<std::string> names=getAllTables(ChannelDBName);
        for (auto name:names){
            if (name==CHANNELMANAGENAME) continue;
            deleteDB(name);
        }
    }
    std::vector<std::map<std::string,std::string>> RWChannelDb::getModulesMap(Channel info){
        std::vector<std::map<std::string,std::string>> infoS;
        for (auto module : info.modules){
            std::map<std::string,std::string> info;
            info["序号"]=std::to_string(module.index);
            info["typeName"]=module.typeName;
            info["名称"]=module.name;
            info["参数"]=module.param;
            infoS.push_back(info);
        }
        return infoS;
    }
    std::vector<ModuleOfChannel> RWChannelDb::readModulesParam(std::string dbName){
        std::vector<ModuleOfChannel> modules;
        std::vector<std::map<std::string,std::string>> fillContent;
        std::map<std::string,std::string> info;
        info["序号"]="";
        info["typeName"]="";
        info["名称"]="";
        info["参数"]="";
        fillContent=readRecord(dbName,info);
        if (fillContent.size()>0){
            for (auto infoS : fillContent){
                ModuleOfChannel moduleOfChannel;
                if (infoS["序号"]!="") moduleOfChannel.index=std::stoi(infoS["序号"]);
                moduleOfChannel.name=infoS["名称"];
                moduleOfChannel.typeName=infoS["typeName"];
                moduleOfChannel.param=infoS["参数"];
                modules.push_back(moduleOfChannel);
            }
        }
        return modules;
    }
    void RWChannelDb::writeModulesRecord(std::string dbName, bool coverFlag, const std::vector<std::map<std::string, std::string>> &infoSS)
    {
        std::map<std::string, std::string> infoS;
        if (coverFlag)
        {
            dbOpera.clearDB(dbName);
        }
        if (infoSS.size() <= 0)
        {
            infoS["序号"] = "";
            infoS["typeName"] = "";
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