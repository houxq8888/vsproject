#include "HGComDebugWidget.h"
#include <QApplication>
#include "ProtocolMiddleware.h"
#include "SerialTransport.h"
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char *argv[])
{
        // 创建传输层
    auto transport = std::unique_ptr<SerialTransport>(new SerialTransport());
    
    // 创建中间件层
    auto comm = std::unique_ptr<ProtocolMiddleware>(
        new ProtocolMiddleware(std::move(transport)));
    
    // 注册回调
    comm->registerCallback(0x1001, [](uint32_t cmd_id, const std::vector<uint8_t>& payload) {
        std::cout << "Received command: " << cmd_id << std::endl;
    });
    
    // 打开串口
    if (!comm->getTransport()->open("COM3", 115200)) {
        return 1;
    }
    
    // 主循环
    while (true) {
        // 发送命令示例
        comm->sendCommand(0x2001, "TEST_CMD", {0x01, 0x02, 0x03});
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
    QApplication a(argc, argv);
    HGComDebugWidget w;
    w.show();
    return a.exec();
}
