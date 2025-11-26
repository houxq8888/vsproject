#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <iomanip>
#include <fstream>
#include <QDebug>
#include "common.h"


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

// 程序的主入口点
int main(int argc, char *argv[])
{
    try {
        const int maxInstances = 1;
        const std::string appName = "ProductFlow";

        // int instanceCount = countInstances(appName);
        // // if (instanceCount > maxInstances) {
        // if (isParentRunning()){
        //     std::cerr << "Too many instances running > (" << maxInstances << "). Exiting." << std::endl;
        //     return 1;
        // }

        // std::cout << "Application is running..." << std::endl;
        // // 记录父进程 PID
        // std::ofstream pidFile(PARENT_PID_FILE);
        // if (pidFile)
        // {
        //     pidFile << getpid();
        //     pidFile.close();
        // }
        // 创建QApplication对象，用于管理图形界面的应用程序
        QApplication a(argc, argv);
        QString basePath = qApp->applicationDirPath();
        std::string dbDir = basePath.toStdString() + "/database";
        HGMkDir(dbDir);
        loadConfig(basePath.toStdString());
        saveConfig();
        RWDb::openDB(basePath.toStdString());
        // // 加载默认语言
        // QTranslator translator;
        // if (translator.load(":/translations/HGOnlinePlatformApp_zh.qm")) {
        //     a.installTranslator(&translator);
        // }
        // 创建主窗口对象
        MainWindow w("zh");

        // 显示主窗口
        w.show();

        // 进入程序主循环，等待用户操作
        return a.exec();
    } catch (const std::exception& e) {
        // 处理异常，输出错误信息并退出程序
        std::cerr << "Exception caught: " << e.what() << std::endl;
        // remove(PARENT_PID_FILE);  // 退出时删除父进程 PID 文件
        return 1;
    }
}

