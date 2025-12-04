#include "demo.h"
#include <iostream>
#include <thread>
#include <chrono>

Demo::Demo() {
    transport_ = std::unique_ptr<SerialTransport>(new SerialTransport());
    schema_ = std::make_shared<ProtocolSchema>();
}

Demo::~Demo() {
    if (protocol_!=nullptr)
        protocol_->getTransport()->close();
}

bool Demo::initialize(const std::string& port, uint32_t baudrate, const std::string& protocol_file) {
    // 加载协议定义
    if (!schema_->load(protocol_file)) {
        std::cerr << "Failed to load protocol schema" << std::endl;
        return false;
    }
    // 初始化中间件
    protocol_ = std::unique_ptr<ProtocolMiddleware>(
        new ProtocolMiddleware(std::move(transport_),schema_));

    // 初始化串口
    if (!protocol_->getTransport()->open(port, baudrate)) {
        std::cerr << "Failed to open serial port: " << port << std::endl;
        return false;
    }
    
    // 注册命令回调
    protocol_->registerCallback(CMD_GET_VERSION, [this](uint32_t cmd_id, const std::vector<uint8_t>& payload) {
        std::cout << "回调注册成功！命令ID: " << cmd_id<<std::endl;  // 添加调试输出
        onCommandReceived(cmd_id, payload);
    });
    
    protocol_->registerCallback(CMD_SET_PARAMETER, [this](uint32_t cmd_id, const std::vector<uint8_t>& payload) {
        std::cout << "回调注册成功！命令ID: " << cmd_id<<std::endl;  // 添加调试输出
        onCommandReceived(cmd_id, payload);
    });
    
    // 其他命令注册...
    
    return true;
}

void Demo::run() {
    // 发送获取版本命令
    // std::vector<uint8_t> empty_payload; // ={0x0E,0x0F,0x0C,0x0D};
    json params1 = json::array();
    if (!protocol_->sendCommand(CMD_GET_VERSION, "GetVersion", params1)) {
        std::cerr << "Failed to send command" << std::endl;
    }
    
    // 构建SetParameter请求
    json params = {
        {"param_id", 1},
        {"value", 0x02},
        {"value1",0x03},
        {"value2",0x04},
        {"value3",true},
        {"value4",0x06},
        {"value5",0x07},
        {"value6",0x08},
        {"value7",0x09},
        {"value8",3.56},
        {"value9",2.889},
        {"value10","hello world"},
        {"value11","hi"}
    };
    // 发送设置参数命令
    if (!protocol_->sendCommand(CMD_SET_PARAMETER, "SetParameter", params)) {
        std::cerr << "Failed to send command" << std::endl;
    }
    
    // 主循环
    while (true) {
        // 处理接收数据
        protocol_->processReceivedData();
        
        // 其他业务逻辑...
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Demo::onCommandReceived(uint32_t cmd_id, const std::vector<uint8_t>& payload) {
    try {
        json response;
        
        switch (cmd_id) {
            case CMD_GET_VERSION:
                response = {
                    {"major", 1},
                    {"minor", 0},
                    {"patch", 2}
                };
                protocol_->sendResponse(cmd_id, "sucess", response);
                break;
                
            case CMD_SET_PARAMETER: {
                json decoded = protocol_->decode_payload(cmd_id, payload);
                std::cout << "Received params: " 
                          << decoded["param_id"] << ", "
                          << decoded["value"] << ", "
                          << decoded["value1"] << ", "
                          << decoded["value2"] << ", "
                          << decoded["value3"] << ", "
                          << decoded["value4"] << ", "
                          << decoded["value5"] << ", "
                          << decoded["value6"] << ", "
                          << decoded["value7"] << ", "
                          << decoded["value8"] << ", "
                          << decoded["value9"] << ", "
                          << decoded["value10"] << ", "
                          << decoded["value11"] << std::endl;
                          
                protocol_->sendResponse(cmd_id, "error_code", {{"error_code", 1}});
                break;
            }
            default:
                // 保持原有二进制方式
                std::vector<uint8_t> response_data = {0x01};
                protocol_->sendResponse(cmd_id, "success", response_data);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing command: " << e.what() << std::endl;
        protocol_->sendResponse(cmd_id, "error_code", {{"error_code", 1}});
    }
}