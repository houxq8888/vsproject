#include "hgcamerarecognizewidget.h"
#include <QApplication>
#include <iostream>
#include "hgcommonutility.h"
#include "config.h"
#include "rwDb.h"
#include "common.h"

#define CONFIG_DIR "/config"

bool loadConfig(const std::string &curPath){
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


int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        std::string dbDir = basePath.toStdString() + "/database";
        HGMkDir(dbDir);
        loadConfig(basePath.toStdString());
        saveConfig();
        RWDb::openDB(basePath.toStdString());
        HGCameraRecognizeWidget w("zh");
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}