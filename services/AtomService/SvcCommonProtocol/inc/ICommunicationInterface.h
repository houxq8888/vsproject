#ifndef ICOMMUNICATIONINTERFACE_H
#define ICOMMUNICATIONINTERFACE_H

#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

// 命令回调函数类型
using CommandCallback = std::function<void(uint32_t cmd_id, const std::vector<uint8_t>& payload)>;

class ICommunicationInterface {
public:
    virtual ~ICommunicationInterface() = default;
    
    // 发送命令到设备
    virtual bool sendCommand(uint32_t cmd_id, const std::string& cmd_name, 
                           const std::vector<uint8_t>& payload = {}) = 0;
    
    // 注册命令回调
    virtual void registerCallback(uint32_t cmd_id, CommandCallback callback) = 0;
    
    // 处理接收数据
    virtual void processReceivedData() = 0;
    
    // 发送响应
    virtual bool sendResponse(uint32_t cmd_id,  const std::string& cmd_name, 
                            const std::vector<uint8_t>& payload) = 0;
};

#endif