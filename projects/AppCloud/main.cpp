#include <QApplication>
#include <iostream>
#include "hgcommonutility.h"
#include <QMessageBox>
#include <iomanip>
#include <fstream>
#include <QDebug>
#include <QPrinterInfo>
#include "CloudMainWidget.h"


using namespace HGMACHINE;


// 程序的主入口点
int main(int argc, char *argv[])
{
    try {
        const int maxInstances = 1;
        const std::string appName = "CloudApp";

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
        QString basePath = qApp->applicationDirPath();
   
        // 创建主窗口对象
        CloudMainWidget w;

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

