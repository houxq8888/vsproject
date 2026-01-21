#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include "protocolSchema.hpp"

class CommandBuilder {
public:
    explicit CommandBuilder(std::shared_ptr<ProtocolSchema> schema) : schema_(std::move(schema)) {}
    std::string check_data_state() const {
        return schema_->check_data_state();
    }
    // 构建请求帧
    std::vector<uint8_t> build_request(uint32_t cmd_id, const std::vector<uint8_t>& params);
    std::vector<uint8_t> build_request(uint32_t cmd_id, const json& params);
    
    // 构建响应帧
    std::vector<uint8_t> build_response(uint32_t cmd_id, bool success, const std::vector<uint8_t>& data);
    std::vector<uint8_t> build_response(uint32_t cmd_id, const json& data=json::object());
    
private:
    // 类型明确的封装方法
    void encode_uint8(std::vector<uint8_t>& frame, uint8_t value) {
        validate_range(value, "uint8");
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }

    void encode_uint16(std::vector<uint8_t>& frame, uint16_t value) {
        validate_range(value, "uint16");
        frame.push_back(static_cast<uint8_t>(value >> 8));
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }

    void encode_uint32(std::vector<uint8_t>& frame, uint32_t value) {
        validate_range(value, "uint32");
        frame.push_back(static_cast<uint8_t>(value >> 24));
        frame.push_back(static_cast<uint8_t>(value >> 16));
        frame.push_back(static_cast<uint8_t>(value >> 8));
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }
    void encode_bool(std::vector<uint8_t>& frame, bool value) {
        validate_range(value, "bool");
        frame.push_back(value ? 1 : 0);
        // encode_varint(frame, value ? 1 : 0);
    }
    void encode_int8(std::vector<uint8_t>& frame, int8_t value) {
        validate_range(value, "int8");
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }
    void encode_int16(std::vector<uint8_t>& frame, int16_t value) {
        validate_range(value, "int16");
        frame.push_back(static_cast<uint8_t>(value >> 8));
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }
    void encode_int32(std::vector<uint8_t>& frame, int32_t value) {
        validate_range(value, "int32");
        frame.push_back(static_cast<uint8_t>(value >> 24));
        frame.push_back(static_cast<uint8_t>(value >> 16));
        frame.push_back(static_cast<uint8_t>(value >> 8));
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }
    void encode_uint64(std::vector<uint8_t> &frame, uint64_t value)
    {
        validate_range(value, "uint64");
        frame.push_back(static_cast<uint8_t>(value >> 56));
        frame.push_back(static_cast<uint8_t>(value >> 48));
        frame.push_back(static_cast<uint8_t>(value >> 40));
        frame.push_back(static_cast<uint8_t>(value >> 32));
        frame.push_back(static_cast<uint8_t>(value >> 24));
        frame.push_back(static_cast<uint8_t>(value >> 16));
        frame.push_back(static_cast<uint8_t>(value >> 8));
        frame.push_back(static_cast<uint8_t>(value));
        // encode_varint(frame, value);
    }
    // 编码字符串（Length-delimited）
    void encode_string(std::vector<uint8_t> &frame, const std::string &str)
    {
        // 1. 先编码长度（varint）
        uint32_t len=str.size();
        validate_range(len, "uint32");
        frame.push_back(static_cast<uint8_t>(len >> 24));
        frame.push_back(static_cast<uint8_t>(len >> 16));
        frame.push_back(static_cast<uint8_t>(len >> 8));
        frame.push_back(static_cast<uint8_t>(len));
        // encode_varint(frame, str.size());

        // 2. 编码实际数据
        frame.insert(frame.end(), str.begin(), str.end());
    }

    // 编码字节数组（Length-delimited）
    void encode_bytes(std::vector<uint8_t> &frame, const std::string &bytes)
    {
        // 1. 先编码长度（varint）
        uint32_t len=bytes.size();
        validate_range(len, "uint32");
        frame.push_back(static_cast<uint8_t>(len >> 24));
        frame.push_back(static_cast<uint8_t>(len >> 16));
        frame.push_back(static_cast<uint8_t>(len >> 8));
        frame.push_back(static_cast<uint8_t>(len));
        // encode_varint(frame, bytes.size());

        // 2. 编码实际数据
        frame.insert(frame.end(), bytes.begin(), bytes.end());
    }

    void validate_range(uint64_t value, const std::string& type) {
        static const std::map<std::string, uint64_t> limits = {
            {"uint8", 0xFF},
            {"uint16", 0xFFFF},
            {"uint32", 0xFFFFFFFF},
            {"int8", 0x7F},
            {"int16", 0x7FFF},
            {"int32", 0x7FFFFFFF},
            {"int64", 0x7FFFFFFFFFFFFFFF},
            {"uint64", 0xFFFFFFFFFFFFFFFF}
        };

        if (limits.count(type) && value > limits.at(type)) {
            throw std::overflow_error(type + " value out of range");
        }
    }
    void encode_binary_fields(std::vector<uint8_t>& frame, 
                            const json& field_defs,
                            const std::vector<uint8_t>& params);
    
    void encode_field(std::vector<uint8_t>& frame, const json& field, const json& value);
    void encode_varint(std::vector<uint8_t>& frame, uint32_t value);
    // void add_checksum(std::vector<uint8_t>& frame);
    
    std::shared_ptr<ProtocolSchema> schema_;
};

#endif