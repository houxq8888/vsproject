/***************************************************************************
 * File Name  : main.cpp
 * Description:
 * Version    : V1.0.0.0
 * Author     : xiaoqin.hou
 * Date       : 2025-03
 * History    :
 *
***************************************************************************/

#include "HGDashboardWidget.h"
#include <QApplication>
#include <iostream>
#include <stdio.h>
#include <QProcess>
#include "hgcommonutility.h"
#include "QRoundProgressBar.h"
#include <cstdlib>
#include <csignal>
#include <thread>
#include <chrono>
// #include <QtWebEngine/QtWebEngine>
// #include <QWebEngineView>

using namespace HGMACHINE;

// 运行 Mosquitto 代理
void startMosquitto() {
    std::cout << "启动 Mosquitto 代理...\n";
    std::system("sudo systemctl start mosquitto"); // 在后台运行 Mosquitto
}

// 关闭 Mosquitto 代理
void shutdownMosquitto() {
    std::cout << "关闭 Mosquitto 代理...\n";
    std::system("kill $(pgrep mosquitto)");  // 终止 Mosquitto
}

// 信号处理
void signalHandler(int signum) {
    std::cout << "捕获信号 " << signum << ", 退出程序...\n";
    shutdownMosquitto();
    exit(signum);
}



// int main() {
//     // 模拟 TCP 接收到的数据
//     std::vector<uint8_t> tcpData = {
//         0x08,0x00,0x00,0x00,0x01,0x61,0x69,0x3D,0x2B,0x30,0x35,0x33,0x31,
//         0x02,0x00,0x00,0x00,0x01,0x37,0x23,0x00,0x00,0x00,0x00,0x00,0x00
//     };

//     std::string asciiStr = bytesToAscii(tcpData.data(), tcpData.size());
//     std::cout << "ASCII output: " << asciiStr << std::endl;

//     return 0;
// }


// 程序的主入口点
int main(int argc, char *argv[])
{    

    // // 监听异常信号
    // signal(SIGINT, signalHandler);  // Ctrl+C
    // signal(SIGTERM, signalHandler); // kill 命令
    // signal(SIGSEGV, signalHandler); // 段错误
    // signal(SIGABRT, signalHandler); // abort() 终止
    // std::atexit(shutdownMosquitto);  // 注册退出回调

    // startMosquitto(); // 启动 Mosquitto
    // std::this_thread::sleep_for(std::chrono::seconds(2)); // 等待 Mosquitto 启动

    try {
        const int maxInstances = 1;
        const std::string appName = "HGDashboardApp";

        int instanceCount = countInstances(appName);
        // if (instanceCount > maxInstances) {
        if (isParentRunning()){
            std::cerr << "Too many instances running > (" << maxInstances << "). Exiting." << std::endl;
            return 1;
        }

        std::cout << "Application is running..." << std::endl;
        // QtWebEngine::initialize();
        // 创建QApplication对象，用于管理图形界面的应用程序
        QApplication a(argc, argv);
        // QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

        // qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox --disable-gpu --single-process");
        // qputenv("QTWEBENGINEPROCESS_PATH", "/opt/HGDashboardApp/QtWebEngineProcess");
        // qputenv("QTWEBENGINE_RESOURCES_PATH", "/opt/HGDashboardApp/resources");
        // qputenv("QTWEBENGINE_LOCALES_PATH", "/opt/HGDashboardApp/qtwebengine_locales");

        // QWebEngineView view;
        // view.load(QUrl::fromLocalFile("/opt/HGDashboardApp/resources/web/map.html"));
        // view.show();

        QString basePath = qApp->applicationDirPath();

        // 创建主窗口对象
        HGDashboardWidget w;

        // 显示主窗口
        w.show();

        // 进入程序主循环，等待用户操作
        return a.exec();
    } catch (const std::exception& e) {
        // 处理异常，输出错误信息并退出程序
        std::cerr << "Exception caught: " << e.what() << std::endl;
        remove(PARENT_PID_FILE);  // 退出时删除父进程 PID 文件
        // shutdownMosquitto();
        return 1;
    }
}

