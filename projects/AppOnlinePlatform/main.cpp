#include <QApplication>
#include "HGOnlineMainWidget.h"
#include <iostream>
#include <QTranslator>
#include "HGOnlinePlatformInterface.h"
#include <QMessageBox>
#include "hgcommonutility.h"
#include "demo.h"
#include <iomanip>
#include <fstream>
#include <QPrinterInfo>
#include "globalsingleton.h"


using namespace HGMACHINE;

std::map<std::vector<uint8_t>, std::string> parsePacketCAS6Batch(const std::vector<uint8_t> &data)
{
    size_t i = 0;
    std::map<std::vector<uint8_t>, std::string> rawDatas;

    // 1. 帧头
    if (data[i] == 0xEE && data[i + 1] == 0xB1)
    {
        std::cout << "帧头匹配 ✅" << std::endl;
        i += 2;
    }
    else
    {
        std::cout << "无效帧头 ❌" << std::endl;
        return rawDatas;
    }

    i+=3;

    // 4. 解析TLV
    while (i <= data.size())
    { // 至少要能读Tag+Len
        // 检查帧尾
        if (i + 1 < data.size() && data[i] == 0xFF && data[i + 1] == 0xFC)
        {
            std::cout << "帧尾到达 ✅" << std::endl;
            break;
        }
        printf("tag:0x%02X, 0x%02X, len:0x%02X, 0x%02X \n", data[i],data[i+1],data[i+2],data[i+3]);
        uint16_t tag = data[i] | (data[i + 1] << 8);
        uint16_t len = (data[i+2] << 8) | data[i+3];
        i += 4;
        printf("len:%d\n", len);
        // 检查长度是否合理
        if (i + len > data.size())
        {
            std::cout << "错误：数据超出范围 ❌" << std::endl;
            break;
        }

        std::string valueStr;
        for (size_t j = 0; j < len; ++j)
        {
            valueStr += static_cast<char>(data[i + j]);
        }
        i += len;

        std::cout << "Tag: 0x" << std::hex << std::setw(4) << std::setfill('0') << tag
                  << " | Len: " << std::dec << len
                  << " | Value: \"" << valueStr << "\"" << std::endl;

        std::vector<uint8_t> key;
        key.push_back(static_cast<uint8_t>((tag >> 8) & 0xFF));
        key.push_back(static_cast<uint8_t>(tag & 0xFF));
        rawDatas[key] = valueStr;
    }
}


// 程序的主入口点
int main(int argc, char *argv[])
{
    // std::vector<uint8_t> rawData = {
    //     0xEE, 0xB1, 0x12, 0x00, 0x03, 0x00,
    //     0x1D, 0x00, 0x03, 0x30, 0x20, 0x31,
    //     0x00, 0x24, 0x00, 0x05, 0x31, 0x36, 0x30, 0x2E, 0x30,
    //     0x00, 0x25, 0x00, 0x05, 0x31, 0x35, 0x34, 0x2E, 0x32,
    //     0x00, 0x37, 0x00, 0x03, 0x30, 0x2E, 0x30,
    //     0x00, 0x11, 0x00, 0x03, 0x34, 0x37, 0x36,
    //     0x00, 0x28, 0x00, 0x04, 0x30, 0x2E, 0x30, 0x30,
    //     0x00, 0x10, 0x00, 0x08, 0x30, 0x30, 0x3A, 0x30, 0x30, 0x3A, 0x30, 0x30,
    //     0x00, 0x0F, 0x00, 0x04, 0x30, 0x2E, 0x30, 0x30,
    //     0x00, 0x1B, 0x00, 0x04, 0x30, 0x31, 0x31, 0x31,
    //     0x00, 0x36, 0x00, 0x06, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    //     0xFF, 0xFC, 0xFF, 0xFF
    // };

    // parsePacketCAS6Batch(rawData);

    // // testSerialize();
    // return 0;
    
    try {
        const int maxInstances = 1;
        const std::string appName = "HGOnlinePlatformApp";

        int instanceCount = countInstances(appName);
        // if (instanceCount > maxInstances) {
        if (isParentRunning()){
            std::cerr << "Too many instances running > (" << maxInstances << "). Exiting." << std::endl;
            return 1;
        }

        std::cout << "Application is running..." << std::endl;
        // 记录父进程 PID
        std::ofstream pidFile(PARENT_PID_FILE);
        if (pidFile)
        {
            pidFile << getpid();
            pidFile.close();
        }
        // 创建QApplication对象，用于管理图形界面的应用程序
        QApplication a(argc, argv);

        QList<QPrinterInfo> printers = QPrinterInfo::availablePrinters();
        for (const QPrinterInfo &printer : printers){
            qDebug() << " - Name:" << printer.printerName();
        }
        QPrinterInfo defaultPrinter = QPrinterInfo::defaultPrinter();
        qDebug() << "Default Printer:" << defaultPrinter.printerName();

        QString basePath = qApp->applicationDirPath();
        loadConfig(basePath.toStdString());
        openDB(HGOnlineRWDB::readCurDirPath());
        GlobalSingleton::instance().loadSystemInfo();
        GlobalSingleton::instance().loadDataChartInfo();
        if (!HGOnlineRWDB::isLoginRightTime()) {
            RWDb::writeAuditTrailLog("系统时间设置落后于上一次软件退出时间，请修改！");
            if (QMessageBox::Ok == QMessageBox::warning(nullptr, QString::fromStdString(HG_DEVICE_NAME), "系统时间设置落后于上一次软件退出时间，请修改！"))
            {
                return 0;
            }
        }
   
        // // 加载默认语言
        // QTranslator translator;
        // if (translator.load(":/translations/HGOnlinePlatformApp_zh.qm")) {
        //     a.installTranslator(&translator);
        // }
        // 创建主窗口对象
        HGOnlineMainWidget w;

        // 显示主窗口
        w.show();

        // 进入程序主循环，等待用户操作
        return a.exec();
    } catch (const std::exception& e) {
        // 处理异常，输出错误信息并退出程序
        std::cerr << "Exception caught: " << e.what() << std::endl;
        remove(PARENT_PID_FILE);  // 退出时删除父进程 PID 文件
        return 1;
    }
}

