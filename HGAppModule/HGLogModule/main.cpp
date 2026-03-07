#include "hglogwidget.h"
#include <QApplication>
#include <iostream>
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

void openDB(const std::string&dbName){
    RWDb::openDB(dbName);
}

int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        std::string dbDir = basePath.toStdString() + "/database";
        HGMkDir(dbDir);
        loadConfig(basePath.toStdString());
        openDB(basePath.toStdString());
        saveConfig();

        // 插入201条测试数据
        std::vector<std::string> testLogs = {
            "用户admin登录系统", "启动校准流程", "采样泵启动",
            "检测器初始化完成", "试剂余量不足警告", "通道1开始检测",
            "方法A加载成功", "流程执行完毕", "数据保存成功",
            "用户admin退出系统", "系统自检通过", "温度传感器异常",
            "网络连接断开", "数据库备份完成", "固件版本更新",
            "权限验证失败", "样品池清洗中", "标准液配制完成",
            "报警: 压力超限", "维护提醒: 请更换滤芯"
        };
        printf("正在插入201条测试日志...\n");
        for (int i = 0; i < 201; i++) {
            std::string logContent = testLogs[i % testLogs.size()] 
                + " [测试#" + std::to_string(i+1) + "]";
            RWDb::writeAuditTrailLog(logContent);
        }
        printf("测试日志插入完成！共201条\n");

        HGLogWidget w("zh");
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}