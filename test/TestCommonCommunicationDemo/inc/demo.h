#pragma once
#include "ProtocolMiddleware.h"
#include <memory>


class Demo {
public:
    Demo();
    ~Demo();
    
    bool initialize(const std::string& port, uint32_t baudrate, const std::string& protocol_file);
    void run();
    
private:
    void onCommandReceived(uint32_t cmd_id, const std::vector<uint8_t>& payload);
    // void onResponseReceived(uint32_t cmd_id, bool success, const std::vector<uint8_t>& data);
    
private:
    std::unique_ptr<SerialTransport> transport_;
    std::shared_ptr<ProtocolSchema> schema_;
    std::unique_ptr<ProtocolMiddleware> protocol_;

    // 定义命令ID
    enum CommandIds {
        CMD_GET_VERSION = 1,
        CMD_SET_PARAMETER = 2,
        CMD_START_MEASUREMENT = 3,
        CMD_STOP_MEASUREMENT = 4,
        CMD_GET_DATA = 5
    };
};