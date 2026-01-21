#include "protocoldecoder.h"
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdint.h>


ProtocolDecoder::ProtocolDecoder(std::shared_ptr<ProtocolSchema> schema)
    : schema_(std::move(schema)) {}


json ProtocolDecoder::decode_payload(uint32_t cmd_id, const std::vector<uint8_t>& payload) {
    std::cout << "Decoding payload (" << payload.size() << " bytes): ";
    for (auto byte : payload) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    if (!schema_) {
        throw std::runtime_error("Schema not initialized");
    }

    json cmd;
    try {
        cmd = schema_->get_command(cmd_id);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Command lookup failed: ") + e.what());
    }

    if (cmd.is_null()) {
        throw std::runtime_error("Unknown command ID: " + std::to_string(cmd_id));
    }

    if (!cmd.contains("request_fields") || !cmd["request_fields"].is_array()) {
        throw std::runtime_error("Invalid command format: missing request_fields");
    }

    json result;
    size_t pos = 0;
    const json& fields = cmd["request_fields"];

    while (pos < payload.size()) {
        if (pos + 1 > payload.size()) {
            throw std::runtime_error("Unexpected end of payload while reading tag");
        }

        uint8_t tag = payload[pos++];
        if (tag == 0) break; // Protobuf合法结束标记

        uint8_t field_num = tag >> 3;
        uint8_t wire_type = tag & 0x07;

         // 调试输出（显示原始字节位置）
        std::cout << "Tag: 0x" << std::hex << static_cast<int>(tag)
                  << " at pos=" << std::dec << (pos-1)
                  << " (field_num=" << static_cast<int>(field_num)
                  << ", wire_type=" << static_cast<int>(wire_type) << ")\n";

        if (field_num == 0) {
            // 尝试恢复：可能是前一个字段解析错误导致的错位
            throw("Attempting recovery from invalid field...");
            continue;
        }

        bool field_found = false;
        for (const auto& field : fields) {
            if (field["field_num"].get<uint8_t>() == field_num) {
                try {
                    std::string name = field["name"].get<std::string>();
                    result[name] = decode_value(wire_type, payload, pos, field);
                    std::cout << "Decoded field: " << name << " = " << result[name].dump() << "\n";
                    field_found = true;
                    break;
                } catch (const std::exception& e) {
                    throw std::runtime_error(std::string("Error decoding field ") + 
                                           field["name"].get<std::string>() + ": " + e.what());
                }
            }
        }

        if (!field_found) {
            std::cerr << "Warning: Unknown field number " << static_cast<int>(field_num) 
                      << ", skipping\n";
            skip_field(wire_type, payload, pos);
        }
    }

    return result;
}

json ProtocolDecoder::decode_value(uint8_t wire_type,
                                 const std::vector<uint8_t>& payload,
                                 size_t& pos,
                                 const json& field_definition) {  // 添加字段定义参数
    if (!field_definition.is_object()) {
        throw std::runtime_error("Field definition must be a JSON object");
    }

    const std::string& type = field_definition["type"].get<std::string>();

    switch (wire_type) {
        case 0: { // Varint
            if (type == "uint8") {
                if (pos + 1 > payload.size()) throw std::out_of_range("8-bit data truncated");
                uint8_t val8 = payload[pos];
                pos += 1;
                if (val8 > 0xFF) throw std::overflow_error("uint8 overflow");
                return val8;
            }
            else if (type == "uint16")
            {
             // 16-bit
                if (pos + 2 > payload.size())
                    throw std::out_of_range("16-bit data truncated");
                uint16_t val16 = (payload[pos] << 8) | payload[pos + 1];
                pos += 2;
                if (val16 > 0xFFFF) throw std::overflow_error("uint16 overflow");
                return val16;
            }
            else if (type == "uint32") {
                if (pos + 4 > payload.size())
                    throw std::out_of_range("32-bit data truncated");
                uint32_t val32 = 0;
                for (int i=0;i<4;i++){
                    val32 = (val32 << 8) | payload[pos + i];
                }
                pos += 4;
                if (val32 > 0xFFFF) throw std::overflow_error("uint32 overflow");
                return val32;
            } else if (type == "uint64"){
                if (pos + 8 > payload.size())
                    throw std::out_of_range("64-bit data truncated");
                uint64_t val64 = 0;
                for (int i=0;i<8;i++){
                    val64 = (val64 << 8) | payload[pos + i];
                }
                pos += 8;

                if (val64 > 0xFFFF) throw std::overflow_error("uint64 overflow");
                return val64;
            }
            else if (type == "bool")
            {
                if (pos + 1 > payload.size())
                    throw std::out_of_range("bool truncated");
                bool val = payload[pos] != 0;
                pos += 1;
                return val;
            }
            else if (type == "int8")
            {
                if (pos + 1 > payload.size())
                    throw std::out_of_range("int8 truncated");
                int8_t val = static_cast<int8_t>(payload[pos]);
                pos += 1;
                return val;
            }
            else if (type == "int16")
            {
                if (pos + 2 > payload.size())
                    throw std::out_of_range("int16 truncated");
                int16_t val = (payload[pos] << 8) | payload[pos + 1];
                pos += 2;
                return val;
            }
            else if (type == "int32")
            {
                if (pos + 4 > payload.size())
                    throw std::out_of_range("int32 truncated");
                int32_t val = 0;
                for (int i = 0; i < 4; ++i)
                    val = (val << 8) | payload[pos + i];
                pos += 4;
                return val;
            }
            else if (type == "int64")
            {
                if (pos + 8 > payload.size()) throw std::out_of_range("64-bit data truncated");
                int64_t val64 = 0;
                for (int i=0;i<8;i++){
                    val64 = (val64 << 8) | payload[pos + i];
                }
                pos += 8;
                return val64;
            }
            else {
                // return static_cast<int64_t>(val); // 默认返回int64
            }
        }
            
        case 1: { // 64-bit
            if (pos + 8 > payload.size()) throw std::out_of_range("64-bit data truncated");
            
            uint64_t val64;
            std::memcpy(&val64, &payload[pos], 8);
            pos += 8;
            
            if (type == "double") {
                double d;
                std::memcpy(&d, &val64, 8);
                return d;
            }
            return val64; // 默认返回uint64
        }
            
        case 2: { // Length-delimited
            uint32_t len = 0;
            if (pos + 4 > payload.size())
                throw std::out_of_range("String data len truncated");
            for (int i = 0; i < 4; i++)
            {
                len = (len << 8) | payload[pos + i];
            }
            pos += 4;
            if (len > 0xFFFF)
                throw std::overflow_error("uint32 overflow");

            if (type == "string" || type == "bytes") {
                std::string str(reinterpret_cast<const char*>(&payload[pos]), len);
                pos += len;
                return str;
            }
        }
        case 5: { // 32-bit
            if (pos + 4 > payload.size()) throw std::out_of_range("32-bit data truncated");
            
            uint32_t val32;
            std::memcpy(&val32, &payload[pos], 4);
            pos += 4;

            if (type == "float" || type == "float32") {
                float f;
                std::memcpy(&f, &val32, 4);
                return f;
            }
            else if (type == "uint32") {
                return val32;
            }
            else {
                throw std::runtime_error("Type mismatch for 32-bit wire type");
            }
        }

        default:
            throw std::runtime_error("Unsupported wire type: " + std::to_string(wire_type));
    }
}
uint64_t ProtocolDecoder::decode_varint(const std::vector<uint8_t>& data, size_t& pos) {
    uint64_t result = 0;
    int shift = 0;
    
    do {
        if (pos >= data.size()) {
            throw std::out_of_range("Unexpected end of data while reading varint");
        }
        
        uint8_t byte = data[pos++];
        result |= (uint64_t)(byte & 0x7F) << shift;
        shift += 7;
        
        if (shift >= 64) {
            throw std::runtime_error("Varint too large (overflow)");
        }
    } while ((data[pos-1] & 0x80) != 0);  // 检查前一个字节的continue标志
    
    return result;
}

bool ProtocolDecoder::verify_checksum(const std::vector<uint8_t>& data) {
    if (data.size() < 4) return false; // AA 55 + CRC + 55 AA
    
    // 改进的CRC16计算
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < data.size() - 2; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    uint16_t received_crc = (data[data.size()-2] << 8) | data.back();
    return crc == received_crc;
}

void ProtocolDecoder::skip_field(uint8_t wire_type,
                               const std::vector<uint8_t>& payload,
                               size_t& pos) {
    switch (wire_type) {
        case 0:
            decode_varint(payload, pos);
            break;
        case 1:
            pos += 8;
            break;
        case 2:
            pos += static_cast<size_t>(decode_varint(payload, pos));
            break;
        case 5:
            pos += 4;
            break;
        default:
            throw std::runtime_error("Unknown wire type while skipping: " + std::to_string(wire_type));
    }
}