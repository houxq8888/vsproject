#include "hgsystemsetwidget.h"

#define CONFIG_DIR "/config"

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
void openDB(const std::string&dbName){
    RWDb::openDB(dbName);
}

int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        std::string dbDir = basePath.toStdString()+"/database";
        HGMkDir(dbDir);
        loadConfig(basePath.toStdString());
        openDB(basePath.toStdString());
        saveConfig();

        std::vector<std::string> names={
            "SampleDetection",
            "SampleEditing",
            "ManualControl",
            "CreateMethod",
            "RectifyMethod",
            "CopyMethod",
            "DeleteMethod",
            "DeviceManage",
            "Recalculation",
            "ReportEditing",
            "PrintFunction",
            "RecordOutput",
            "UserManage(Manager)",
            "UserManage(Users)",
            "AuthorityManage",
            "AuditTrail",
            "TimeSetting",
            "DeviceInfo",
            "ScanAuditTrailRecord",
            "SampleStatisticsSettings"
        };
        AuthorityWidget w("zh",names);
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}