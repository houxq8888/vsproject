#ifndef PROTOCOLMIDDLEWARE_H
#define PROTOCOLMIDDLEWARE_H

#include "ICommunicationInterface.h"
#include "SerialTransport.h"
#include <queue>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <condition_variable>
#include "commandbuilder.h"
#include "protocoldecoder.h"

class ProtocolMiddleware : public ICommunicationInterface {
public:
    explicit ProtocolMiddleware(std::unique_ptr<SerialTransport> transport,
        std::shared_ptr<ProtocolSchema> schema);
    ~ProtocolMiddleware() override;

     // 新增解码封装方法
    json decode_payload(uint32_t cmd_id, const std::vector<uint8_t>& payload) {
        return protocol_decoder_.decode_payload(cmd_id, payload);
    }
    SerialTransport* getTransport() { return transport_.get(); }
    const SerialTransport* getTransport() const { return transport_.get(); }
    
    // 实现ICommunicationInterface接口
    bool sendCommand(uint32_t cmd_id, const std::string& cmd_name, 
                   const std::vector<uint8_t>& payload = {}) override;
    // 新增JSON接口
    bool sendCommand(uint32_t cmd_id, const std::string& cmd_name,
                   const json& payload);
    
    void registerCallback(uint32_t cmd_id, CommandCallback callback) override;
    
    void processReceivedData() override;
    
    bool sendResponse(uint32_t cmd_id, const std::string& cmd_name,const json& data = json::object());

    // 保留原有的二进制接口
    bool sendResponse(uint32_t cmd_id,  const std::string& cmd_name, const std::vector<uint8_t>& data) override;

    
private:
    bool enqueueFrame(const std::vector<uint8_t>& frame); // 提取公共队列操作
    void processIncomingData(const std::vector<uint8_t>& data);
    std::vector<uint8_t> packFrame(const std::vector<uint8_t>& payload);
    std::vector<uint8_t> unpackFrame(const std::vector<uint8_t>& raw_data);
    uint16_t calculateChecksum(const std::vector<uint8_t>& data);
    
    // Protobuf格式打包/解包
    std::vector<uint8_t> encodeCommand(uint32_t cmd_id, const std::string& cmd_name, 
                                      const std::vector<uint8_t>& payload);
    std::vector<uint8_t> encodeResponse(uint32_t cmd_id,  const std::string& cmd_name,
                                       const std::vector<uint8_t>& data);
    bool decodeMessage(const std::vector<uint8_t>& data);
    
    bool decodeProtocolFrame(
        const std::vector<uint8_t>& frame,
        size_t& pos,
        uint32_t& cmd_id,
        std::vector<uint8_t>& payload);
    uint32_t readVarLength(
        const std::vector<uint8_t>& data, 
        size_t& pos) ;
    // 串口通信线程
    void workerThread();
    
private:
    std::unique_ptr<SerialTransport> transport_;
    CommandBuilder command_builder_; // 组合编码器
    ProtocolDecoder protocol_decoder_;// 组合解码器
    // 回调函数映射
    std::unordered_map<uint32_t, CommandCallback> callbacks_;
    std::mutex callback_mutex_;
    
    // 数据缓冲区
    std::vector<uint8_t> rx_buffer_;
    std::mutex rx_mutex_;

    // 发送数据缓冲区
    std::queue<std::vector<uint8_t>> tx_queue_;
    std::mutex tx_mutex_;
    std::condition_variable tx_cv_;
    
    std::atomic_bool connected_{false};
    std::atomic_bool running_{false};
    std::thread worker_thread_;
};

#endif