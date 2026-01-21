#include "packfuncinterface.h"
#include "rwDb.h"
#include "config.h"
#include "hgcommonutility.h"
#include <fstream>
#include <sstream>

namespace HGMACHINE{

#define CONFIG_DIR "/config"
    // database
void openDB(const std::string&dbName){
    RWDb::openDB(dbName);
}
void closeDB(){
    RWDb::closeDB();
}

// config
bool loadConfig(std::string curPath){
    std::string configPath=curPath+CONFIG_DIR;
    HGMkDir(configPath);
    std::string path=configPath+"/config.xml";
    bool valid=isFileExist(path);
    if (!valid){
        FileConfig::createConfigFile(path,"V6");
    } else {
        FileConfig::loadConfigFile(path);
    }
    FileConfig::setDirPath(curPath);
    return true;
}
void saveConfig(){
    FileConfig::saveConfigFile("V6");
}
void setAppVersion(const std::string &version){
    FileConfig::setVersion(version);
}
std::string getAppVersion(){
    return FileConfig::getVersion();
}


void changeUserLoginAndQuitTime(const std::string &userNo, const std::string &key)
{
    std::vector<std::map<std::string, std::string>> userInfos = RWDb::readUserInfo();
    int index=-1;
    for (int i = 0; i < int(userInfos.size()); i++)
    {
        if (userInfos[i]["UserAccount"] == userNo)
        {
            index=i;
            break;
        }
    }
    if (index==-1) return;
    userInfos[index][key]+=(getStandardCurTime()+";");
    RWDb::writeUserInfo(userInfos[index]);
}
}