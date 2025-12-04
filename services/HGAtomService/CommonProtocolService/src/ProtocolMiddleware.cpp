// ProtocolMiddleware.cpp
#include "ProtocolMiddleware.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

constexpr size_t MAX_BUFFER_SIZE = 1024 * 1024; // 1MB
namespace {

    constexpr size_t LENGTH_FIELD_SIZE = 4;
    constexpr size_t CHECKSUM_SIZE = 2;
    constexpr size_t MAX_FRAME_SIZE = 1024;

    // Protobuf字段标签
    constexpr uint8_t CMD_ID_TAG = 0x08;      // (1 << 3) | 0 (varint)
    constexpr uint8_t CMD_NAME_TAG = 0x12;     // (2 << 3) | 2 (length-delimited)
    constexpr uint8_t PAYLOAD_TAG = 0x1A;      // (3 << 3) | 2 (length-delimited)
    constexpr uint8_t SUCCESS_TAG = 0x10;      // (2 << 3) | 0 (varint)
    constexpr uint8_t RESPONSE_DATA_TAG = 0x1A;// (3 << 3) | 2 (length-delimited)
}

ProtocolMiddleware::ProtocolMiddleware(std::unique_ptr<SerialTransport> transport,
    std::shared_ptr<ProtocolSchema> schema)
    : transport_(std::move(transport)),
    command_builder_(schema),
    protocol_decoder_(schema),
    connected_(transport_->isOpen())
{
    
    transport_->setDataReceivedCallback([this](const std::vector<uint8_t>& data) {
        std::cout << "数据到达! 大小: " << data.size() << "字节\n"; // 调试输出
        processIncomingData(data);
    });
    
    running_ = true;
    worker_thread_ = std::thread(&ProtocolMiddleware::workerThread, this);
}

ProtocolMiddleware::~ProtocolMiddleware() {
    running_ = false;
    tx_cv_.notify_all();
    
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}
bool ProtocolMiddleware::sendResponse(uint32_t cmd_id, const std::string& cmd_name, const json& data) {
    if (!connected_) return false;
    
    try {
        // 1. 通过CommandBuilder构建二进制数据
        std::vector<uint8_t> binary_data;
        if (data.is_array()){
            binary_data.reserve(data.size());
            for (const auto& item : data){
                binary_data.push_back(item.get<uint8_t>());
            } 
        }else {
            binary_data = command_builder_.build_response(cmd_id, data);
        }
        return sendResponse(cmd_id, cmd_name, binary_data);
    } catch (const std::exception& e) {
        std::cerr << "Build response failed: " << e.what() << std::endl;
        return false;
    }
}
bool ProtocolMiddleware::sendCommand(uint32_t cmd_id, const std::string& cmd_name,
                                   const json& payload) {
    try {
        std::cout << "Schema state: " << command_builder_.check_data_state() << std::endl;
        // 1. 通过CommandBuilder构建二进制数据
        std::vector<uint8_t> binary_data;
        if (payload.is_array()) {
            // 如果payload是数组，直接作为二进制参数
            binary_data.reserve(payload.size());
            for (const auto& item : payload) {
                binary_data.push_back(item.get<uint8_t>());
            }
        } else {
            // 如果是对象，使用CommandBuilder的JSON接口
            binary_data = command_builder_.build_request(cmd_id, payload);
        }
        
        // 2. 使用原有编码流程
        return sendCommand(cmd_id, cmd_name, binary_data);
    } catch (const std::exception& e) {
        std::cerr << "Command build failed: " << e.what() << std::endl;
        return false;
    }
}

bool ProtocolMiddleware::sendCommand(uint32_t cmd_id, const std::string& cmd_name, 
                                   const std::vector<uint8_t>& payload) {
    if (!connected_) return false;
    
    auto encoded = encodeCommand(cmd_id, cmd_name, payload);
    auto frame = packFrame(encoded);
    
    enqueueFrame(frame);
    return true;
}
std::vector<uint8_t> ProtocolMiddleware::encodeCommand(uint32_t cmd_id, 
                                                      const std::string& cmd_name, 
                                                      const std::vector<uint8_t>& payload) {
    std::vector<uint8_t> encoded;
    
    // 编码cmd_id (varint)
    while (cmd_id >= 0x80) {
        encoded.push_back(static_cast<uint8_t>(cmd_id | 0x80));
        cmd_id >>= 7;
    }
    encoded.push_back(static_cast<uint8_t>(cmd_id));
    
    // 添加字段标签
    encoded.insert(encoded.begin(), CMD_ID_TAG);
    
    // 编码cmd_name (length-delimited)
    if (!cmd_name.empty()) {
        encoded.push_back(CMD_NAME_TAG);
        
        // 编码长度
        size_t name_len = cmd_name.size();
        if (name_len >= 0x80) {
            encoded.push_back(static_cast<uint8_t>(name_len | 0x80));
            encoded.push_back(static_cast<uint8_t>(name_len >> 7));
        } else {
            encoded.push_back(static_cast<uint8_t>(name_len));
        }
        
        // 添加字符串数据
        encoded.insert(encoded.end(), cmd_name.begin(), cmd_name.end());
    }
    
    // 编码payload (length-delimited)
    if (!payload.empty()) {
        encoded.push_back(PAYLOAD_TAG);
        
        // 编码长度
        size_t payload_len = payload.size();
        if (payload_len >= 0x80) {
            encoded.push_back(static_cast<uint8_t>(payload_len | 0x80));
            encoded.push_back(static_cast<uint8_t>(payload_len >> 7));
        } else {
            encoded.push_back(static_cast<uint8_t>(payload_len));
        }
        
        // 添加payload数据
        encoded.insert(encoded.end(), payload.begin(), payload.end());
    }
    
    return encoded;
}
std::vector<uint8_t> ProtocolMiddleware::encodeResponse(uint32_t cmd_id,  const std::string& cmd_name, 
                                                       const std::vector<uint8_t>& data) {
    std::vector<uint8_t> encoded;
    
    // 编码cmd_id (varint)
    while (cmd_id >= 0x80) {
        encoded.push_back(static_cast<uint8_t>(cmd_id | 0x80));
        cmd_id >>= 7;
    }
    encoded.push_back(static_cast<uint8_t>(cmd_id));
    
    // 添加字段标签
    encoded.insert(encoded.begin(), CMD_ID_TAG);
    
    // 编码success (varint)
    encoded.push_back(SUCCESS_TAG);
    if (cmd_name == "sucess") encoded.push_back(0x01);
    else encoded.push_back(0x00);
    
    // 编码data (length-delimited)
    if (!data.empty()) {
        encoded.push_back(RESPONSE_DATA_TAG);
        
        // 编码长度
        size_t data_len = data.size();
        if (data_len >= 0x80) {
            encoded.push_back(static_cast<uint8_t>(data_len | 0x80));
            encoded.push_back(static_cast<uint8_t>(data_len >> 7));
        } else {
            encoded.push_back(static_cast<uint8_t>(data_len));
        }
        
        // 添加数据
        encoded.insert(encoded.end(), data.begin(), data.end());
    }
    
    return encoded;
}
void ProtocolMiddleware::registerCallback(uint32_t cmd_id, CommandCallback callback) {
    std::cout << "注册回调: CMD=0x" << std::hex << cmd_id << std::dec << std::endl;
    std::lock_guard<std::mutex> lock(callback_mutex_);
    callbacks_[cmd_id] = std::move(callback);
}

void ProtocolMiddleware::processReceivedData() {
    std::vector<uint8_t> local_buffer;
    // 加锁获取当前缓冲区内容
    {
        std::lock_guard<std::mutex> lock(rx_mutex_);
        if (rx_buffer_.empty()) return;
        local_buffer.swap(rx_buffer_);// 清空主缓冲区
    }
    // 处理可能存在的多帧数据
    size_t processed_bytes = 0;
    while (processed_bytes < local_buffer.size()){
        auto frame = unpackFrame(
            std::vector<uint8_t>(local_buffer.begin() + processed_bytes, local_buffer.end())
        );
        if (frame.empty()) {
            // 4.2 未找到完整帧，保留剩余数据
            std::lock_guard<std::mutex> lock(rx_mutex_);
            rx_buffer_.insert(
                rx_buffer_.begin(),
                local_buffer.begin() + processed_bytes,
                local_buffer.end()
            );
            break;
        }

        // 4.3 成功解析帧，处理协议数据
        size_t pos = 0;
        uint32_t cmd_id = 0;
        std::vector<uint8_t> payload;
        
        if (decodeProtocolFrame(frame, pos, cmd_id, payload)) {
            // 4.4 调用回调函数
            std::lock_guard<std::mutex> lock(callback_mutex_);
            if (auto it = callbacks_.find(cmd_id); it != callbacks_.end()) {
                try {
                    it->second(cmd_id, payload);
                } catch (const std::exception& e) {
                    std::cerr << "回调执行错误: " << e.what() << std::endl;
                }
            }
        }

        // 4.5 更新已处理字节数（帧头2 + 长度4 + 数据N + 校验2 + 帧尾2）
        processed_bytes += (frame.size() + 10); 
    }
}
// 新增辅助函数（原解析逻辑拆分出来）
bool ProtocolMiddleware::decodeProtocolFrame(
    const std::vector<uint8_t>& frame,
    size_t& pos,
    uint32_t& cmd_id,
    std::vector<uint8_t>& payload) 
{
    std::cout << "\n=== 开始解析协议帧 ===" << std::endl;
    try {
        // 打印原始数据
        std::cout << "帧数据: ";
        for (size_t i = pos; i < frame.size(); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(frame[i]) << " ";
        }
        std::cout << std::dec << std::endl;

        // 解析cmd_id (字段标签0x08)
        if (pos >= frame.size() || frame[pos] != 0x08) {
            std::cerr << "错误: 缺少CMD_ID字段" << std::endl;
            return false;
        }
        pos++; // 跳过字段标签

        // 解析varint格式的cmd_id
        cmd_id = 0;
        int shift = 0;
        while (true) {
            if (pos >= frame.size()) {
                std::cerr << "错误: cmd_id数据不完整" << std::endl;
                return false;
            }
            uint8_t byte = frame[pos++];
            cmd_id |= (byte & 0x7F) << shift;
            shift += 7;
            if (!(byte & 0x80)) break;
        }
        std::cout << "cmd_id: " << cmd_id << std::endl;

        // 解析后续字段
        while (pos < frame.size()) {
            uint8_t tag = frame[pos++];
            uint8_t field_num = tag >> 3;
            uint8_t wire_type = tag & 0x07;

            switch (field_num) {
                case 2: { // cmd_name (字段号2)
                    if (wire_type != 2) {
                        std::cerr << "错误: cmd_name字段类型应为length-delimited" << std::endl;
                        return false;
                    }
                    uint32_t len = readVarLength(frame, pos);
                    std::string cmd_name(frame.begin() + pos, frame.begin() + pos + len);
                    pos += len;
                    std::cout << "cmd_name: " << cmd_name << std::endl;
                    if (pos >= frame.size()) return true;
                    else break;
                }
                case 3: { // payload (字段号3)
                    if (wire_type != 2) {
                        std::cerr << "错误: payload字段类型应为length-delimited" << std::endl;
                        return false;
                    }
                    uint32_t len = readVarLength(frame, pos);
                    payload.assign(frame.begin() + pos, frame.begin() + pos + len);
                    pos += len;
                    std::cout << "payload: ";
                    for (auto b : payload) {
                        std::cout << std::hex << std::setw(2) << static_cast<int>(b) << " ";
                    }
                    std::cout << std::dec << std::endl;
                    return true; // 解析成功
                }
                default:
                    std::cerr << "警告: 跳过未知字段#" << (int)field_num << std::endl;
                    // 跳过未知字段
                    if (wire_type == 0) { // varint
                        while (pos < frame.size() && (frame[pos++] & 0x80));
                    } else if (wire_type == 2) { // length-delimited
                        uint32_t len = readVarLength(frame, pos);
                        pos += len;
                    }
                    break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "解析异常: " << e.what() << std::endl;
    }
    return false;
}
/**
 * @brief 读取Protobuf变长长度字段
 * @param data 输入数据缓冲区
 * @param pos 当前读取位置（会被更新）
 * @return 解析出的长度值，如果失败返回0并标记pos为无效
 */
uint32_t ProtocolMiddleware::readVarLength(
    const std::vector<uint8_t>& data, 
    size_t& pos) 
{
    uint32_t length = 0;
    int shift = 0;
    constexpr uint8_t MSB_MASK = 0x80; // 最高位掩码
    
    try {
        while (true) {
            if (pos >= data.size()) {
                throw std::out_of_range("数据不足");
            }

            uint8_t byte = data[pos++];
            length |= (byte & 0x7F) << shift;
            shift += 7;

            // 最高位为0表示结束
            if (!(byte & MSB_MASK)) {
                break;
            }

            // 防止恶意数据导致整数溢出
            if (shift >= 32) {
                throw std::overflow_error("长度字段溢出");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "读取长度错误: " << e.what() << std::endl;
        pos = data.size(); // 标记为无效位置
        return 0;
    }

    return length;
}
bool ProtocolMiddleware::decodeMessage(const std::vector<uint8_t>& data) {
    if (data.empty()) return false;
    
    size_t pos = 0;
    uint8_t field_tag = data[pos++];
    
    try {
        if (field_tag == CMD_ID_TAG) {
            // 解码命令消息
            uint32_t cmd_id = 0;
            int shift = 0;
            
            while (pos < data.size()) {
                uint8_t byte = data[pos++];
                cmd_id |= (byte & 0x7F) << shift;
                shift += 7;
                
                if (!(byte & 0x80)) break;
            }
            
            std::string cmd_name;
            std::vector<uint8_t> payload;
            
            // 解析剩余字段
            while (pos < data.size()) {
                uint8_t next_tag = data[pos++];
                
                if (next_tag == CMD_NAME_TAG) {
                    // 解析命令名称
                    uint32_t name_len = data[pos++];
                    if (name_len & 0x80) {
                        // 多字节长度
                        name_len = (name_len & 0x7F) | (data[pos++] << 7);
                    }
                    
                    cmd_name.assign(data.begin() + pos, data.begin() + pos + name_len);
                    pos += name_len;
                } else if (next_tag == PAYLOAD_TAG) {
                    // 解析payload
                    uint32_t payload_len = data[pos++];
                    if (payload_len & 0x80) {
                        // 多字节长度
                        payload_len = (payload_len & 0x7F) | (data[pos++] << 7);
                    }
                    
                    payload.assign(data.begin() + pos, data.begin() + pos + payload_len);
                    pos += payload_len;
                } else {
                    // 未知字段，跳过
                    pos++;
                }
            }
            
            // 调用回调函数
            std::lock_guard<std::mutex> lock(callback_mutex_);
            auto it = callbacks_.find(cmd_id);
            if (it != callbacks_.end()) {
                it->second(cmd_id, payload);
                return true;
            }
            
        } else if (field_tag == SUCCESS_TAG) {
            // 解码响应消息 (简化处理)
            return true;
        }
    } catch (...) {
        std::cerr << "Error decoding message" << std::endl;
    }
    
    return false;
}


bool ProtocolMiddleware::sendResponse(uint32_t cmd_id, const std::string& cmd_name, 
                                    const std::vector<uint8_t>& data) {
    if (!connected_) return false;
    
    auto encoded = encodeResponse(cmd_id, cmd_name, data);
    auto frame = packFrame(encoded);
    return enqueueFrame(frame);
}
bool ProtocolMiddleware::enqueueFrame(const std::vector<uint8_t>& frame) {
    std::lock_guard<std::mutex> lock(tx_mutex_);
    tx_queue_.push(frame);
    tx_cv_.notify_one();
    return true;
}
void ProtocolMiddleware::processIncomingData(const std::vector<uint8_t>& data) {
    // 1. 首先将数据存入缓冲区（保证不丢失任何字节）
    {
        std::lock_guard<std::mutex> lock(rx_mutex_);
        rx_buffer_.insert(rx_buffer_.end(), data.begin(), data.end());
        
        // 安全限制：防止缓冲区无限增长
        if (rx_buffer_.size() > MAX_BUFFER_SIZE) {
            std::cerr << "警告：接收缓冲区溢出，清空数据！" << std::endl;
            rx_buffer_.clear();
        }
    }

    // 2. 尝试处理缓冲区中的数据
    processReceivedData();
}
// 私有方法实现
std::vector<uint8_t> ProtocolMiddleware::packFrame(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> frame;
    frame.reserve(data.size() + 10); // 预留足够空间
    
    // 添加帧头
    frame.insert(frame.end(), std::begin(FRAME_HEADER), std::end(FRAME_HEADER));
    
    // 添加长度字段 (小端)
    uint32_t length = static_cast<uint32_t>(data.size());
    frame.push_back(static_cast<uint8_t>(length & 0xFF));
    frame.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
    frame.push_back(static_cast<uint8_t>((length >> 16) & 0xFF));
    frame.push_back(static_cast<uint8_t>((length >> 24) & 0xFF));
    
    // 添加数据
    frame.insert(frame.end(), data.begin(), data.end());
    
    std::cout<<"send checksum:";
    for (int i=0;i<frame.size();i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(frame[i]) << " ";
    }
    std::cout<<std::endl;
    // 计算并添加校验和
    uint16_t checksum = calculateChecksum(frame);
    frame.push_back(static_cast<uint8_t>(checksum & 0xFF));
    frame.push_back(static_cast<uint8_t>((checksum >> 8) & 0xFF));
    
    // 添加帧尾
    frame.insert(frame.end(), std::begin(FRAME_FOOTER), std::end(FRAME_FOOTER));
    
    return frame;
}

std::vector<uint8_t> ProtocolMiddleware::unpackFrame(const std::vector<uint8_t>& raw_data) {
    if (raw_data.size() < 10) return {}; // 最小帧长度
    
    // 查找帧头
    auto header_pos = std::search(raw_data.begin(), raw_data.end(), 
                                 std::begin(FRAME_HEADER), std::end(FRAME_HEADER));
    if (header_pos == raw_data.end()) return {};
    
    // 检查是否有足够数据
    size_t header_offset = std::distance(raw_data.begin(), header_pos);
    if (raw_data.size() - header_offset < 10) return {}; // 不完整帧
    
    // 提取长度字段 (小端)
    uint32_t data_len = static_cast<uint32_t>(raw_data[header_offset + 2]) |
                       (static_cast<uint32_t>(raw_data[header_offset + 3]) << 8) |
                       (static_cast<uint32_t>(raw_data[header_offset + 4]) << 16) |
                       (static_cast<uint32_t>(raw_data[header_offset + 5]) << 24);
    
    // 检查帧完整性
    size_t expected_frame_size = header_offset + 8 + data_len + 4; // 头2+长4+数据+校验2+尾2
    if (raw_data.size() < expected_frame_size) return {};
    
    // 查找帧尾
    auto footer_pos = std::search(raw_data.begin() + header_offset, raw_data.end(),
                                std::begin(FRAME_FOOTER), std::end(FRAME_FOOTER));
    if (footer_pos == raw_data.end()) return {};
    
    // 提取数据部分
    size_t data_start = header_offset + 6;
    size_t data_end = data_start + data_len;
    
    // 验证校验和
    uint16_t received_checksum = static_cast<uint16_t>(raw_data[data_end]) |
                                (static_cast<uint16_t>(raw_data[data_end + 1]) << 8);
    
    std::cout<<"recieved_checksum: "<<std::hex<<std::setw(2)<<std::setfill('0')
            <<static_cast<uint16_t>(raw_data[data_end])<<"   "<<
            static_cast<uint16_t>(raw_data[data_end+1]) << std::endl;
    std::vector<uint8_t> check_data(raw_data.begin() + header_offset, 
                                   raw_data.begin() + data_end);

    for (int i=0;i<check_data.size();i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(check_data[i]) << " ";
    }
    std::cout<<std::endl;
    uint16_t calculated_checksum = calculateChecksum(check_data);
    
    if (received_checksum != calculated_checksum) {
        std::cerr << "Checksum mismatch: received " << received_checksum 
                  << ", calculated " << calculated_checksum << std::endl;
        return {};
    }
    
    return std::vector<uint8_t>(raw_data.begin() + data_start, 
                               raw_data.begin() + data_end);
}

uint16_t ProtocolMiddleware::calculateChecksum(const std::vector<uint8_t>& data) {
    const uint16_t POLY = 0xA001;  // CRC-16-IBM多项式
    uint16_t crc = 0xFFFF;
    
    for (uint8_t byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= POLY;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void ProtocolMiddleware::workerThread() {
    while (running_) {
        // 检查物理连接状态
        bool is_actually_connected = transport_->isOpen();
        if (connected_.load() != is_actually_connected){
            connected_.store(is_actually_connected);
            std::cout << "连接状态变更: " << (connected_ ? "已连接" : "已断开") << std::endl;
        }
        std::vector<uint8_t> frame_to_send;
        
        {
            std::unique_lock<std::mutex> lock(tx_mutex_);
            tx_cv_.wait_for(lock, std::chrono::milliseconds(100), [this] {
                return !tx_queue_.empty() || !running_;
            });
            
            if (!running_) break;
            
            if (!tx_queue_.empty()) {
                frame_to_send = tx_queue_.front();
                tx_queue_.pop();
            }
        }
        
        if (!frame_to_send.empty()) {
            transport_->writeData(frame_to_send);
        }
    }
}
