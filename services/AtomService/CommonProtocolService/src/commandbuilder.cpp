#include "commandbuilder.h"
#include <algorithm>
#include <cstring>  // for memcpy
#include <stdexcept>

// 构造函数已在头文件中实现
std::vector<uint8_t> CommandBuilder::build_request(uint32_t cmd_id, const json& params) {
    std::cout << "Schema state: " << schema_->check_data_state() << std::endl;
    if (!params.is_array() && !params.is_object()) {
        throw std::invalid_argument("Params must be array or object");
    }
    
    std::vector<uint8_t> binary_params;
    if (params.is_array()) {
        // 处理数组格式参数
        for (const auto& item : params) {
            binary_params.push_back(item.get<uint8_t>());
        }
    }
    else
    {
        // 处理对象格式参数
        json cmd = schema_->get_command(cmd_id);
        if (cmd.is_null())
        {
            // 打印所有可用命令辅助调试
            std::cerr << "Available commands: ";
            auto all_cmds = schema_->get_all_commands();
            for (const auto &c : all_cmds)
            {
                std::cerr << c["id"] << " ";
            }
            std::cerr << std::endl;

            throw std::runtime_error("Command ID " + std::to_string(cmd_id) + " not found");
        }
        const json& fields = cmd["request_fields"];
        for (json::const_iterator it = fields.begin(); it != fields.end(); ++it){
            const std::string field_name = (*it)["name"].get<std::string>();
            if (params.find(field_name)!=params.end()){
                encode_field(binary_params, *it, params);
            }
        }
    }

    // return build_request(cmd_id, binary_params);
    return binary_params;
}
std::vector<uint8_t> CommandBuilder::build_request(uint32_t cmd_id, 
                                                 const std::vector<uint8_t>& params) {
    std::vector<uint8_t> frame;
    
    // 1. 协议头
    frame.insert(frame.end(), std::begin(FRAME_HEADER), std::end(FRAME_HEADER));
    // 2. 获取命令定义
    json cmd = schema_->get_command(cmd_id);
    if (cmd.is_null()) {
        throw std::runtime_error("Unknown command ID: " + std::to_string(cmd_id));
    }
    
    // 3. 编码命令ID
    encode_varint(frame, cmd_id);
    
    // 4. 编码二进制参数字段
    encode_binary_fields(frame, cmd["request_fields"], params);
    
    // 5. 校验和
    // add_checksum(frame);
    return frame;
}
// 安全的wire_type获取
uint8_t get_wire_type(const std::string& type) {
    static const std::unordered_map<std::string, uint8_t> type_map = {
        {"uint8",   0},  // varint
        {"uint16",  0},  // varint
        {"uint32",  0},  // varint
        {"bool",    0},  // varint
        {"int8",    0},  // varint
        {"int16",   0},  // varint
        {"int32",   0},  // varint
        {"uint64",  0},  // varint
        {"double",  1},  // 64-bit
        {"float32", 5},  // 32-bit
        {"float",   5},  // 32-bit
        {"string",  2},  // length-delimited
        {"bytes",   2}  // length-delimited
    };
    
    auto it = type_map.find(type);
    if (it == type_map.end()) {
        throw std::runtime_error("Unknown type: " + type);
    }
    return it->second;
}
void CommandBuilder::encode_binary_fields(std::vector<uint8_t>& frame,
                                        const json& field_defs,
                                        const std::vector<uint8_t>& params) {
    size_t param_index = 0;
    for (const auto& field : field_defs) {
        if (param_index >= params.size()) break;
        
        uint8_t field_num = field["field_num"];
        std::string field_type = field["type"];
        
        // 编码字段标签
        uint8_t wire_type = get_wire_type(field_type);
        frame.push_back((field_num << 3) | wire_type);
        
        // 编码字段值
        switch (wire_type) {
            case 0: // Varint
                encode_varint(frame, params[param_index++]);
                break;
            case 2: // Length-delimited
                // 假设二进制模式下字符串是预编码好的
                frame.push_back(params[param_index++]); // 长度
                frame.insert(frame.end(), 
                           params.begin() + param_index,
                           params.begin() + param_index + frame.back());
                param_index += frame.back();
                break;
            // 其他类型...
        }
    }
}

std::vector<uint8_t> CommandBuilder::build_response(uint32_t cmd_id, const json& data) {
    if (!data.is_array() && !data.is_object()) {
        throw std::invalid_argument("data must be array or object");
    }
    std::vector<uint8_t> frame;
    if (data.is_array())
    {
        // 处理数组格式参数
        for (const auto &item : data)
        {
            frame.push_back(item.get<uint8_t>());
        }
    }
    else
    {
        json cmd = schema_->get_command(cmd_id);
        if (!cmd.is_null())
        {
            const json &fields = cmd["response_fields"];
            for (json::const_iterator it = fields.begin(); it != fields.end(); ++it)
            {
                const std::string field_name = (*it)["name"].get<std::string>();
                if (data.find(field_name) != data.end())
                {
                    encode_field(frame, *it, data);
                }
            }
        }
    }
    return frame;
}

// 私有方法实现
void CommandBuilder::encode_field(std::vector<uint8_t>& frame, 
                                const json& field, 
                                const json& params) {
    std::string name = field["name"].get<std::string>();
    std::string type = field["type"].get<std::string>();
    
    if (!params.contains(name)) {
        throw std::runtime_error("Missing parameter: " + name);
    }

    // 编码字段头 (Protobuf风格)
    uint8_t field_num = field["field_num"].get<uint8_t>();
    uint8_t wire_type = get_wire_type(type);
    frame.push_back((field_num << 3) | wire_type);

    // 按类型编码值
    if (type == "uint8") {
        encode_uint8(frame, params[name].get<uint8_t>());
    } 
    else if (type == "uint16") {
        // uint16_t val = params[name].get<uint16_t>();
        // frame.push_back(val >> 8);    // 高位字节
        // frame.push_back(val & 0xFF);  // 低位字节
        encode_uint16(frame, params[name].get<uint16_t>());
    }
    else if (type == "uint32") {
        encode_uint32(frame, params[name].get<uint32_t>());
    } else if (type == "bool"){
        encode_bool(frame, params[name].get<bool>());
    } else if (type == "int8"){
        encode_int8(frame, params[name].get<int8_t>());
    } else if (type == "int16"){
        encode_int16(frame, params[name].get<int16_t>());
    } else if (type == "int32"){
        encode_int32(frame, params[name].get<int32_t>());
    }
    else if (type == "uint64") {
        encode_uint64(frame, params[name].get<uint64_t>());
    }
    else if (type == "float32" || type == "float") {
        float val = params[name].get<float>();
        auto bytes = reinterpret_cast<const uint8_t*>(&val);
        frame.insert(frame.end(), bytes, bytes + 4);
    } else if (type == "double"){
        double val = params[name].get<double>();
        auto bytes = reinterpret_cast<const uint8_t*>(&val);
        frame.insert(frame.end(), bytes, bytes + 8);
    } else if (type == "string") { 
        encode_string(frame, params[name].get<std::string>());
    } else if (type == "bytes"){
        encode_bytes(frame, params[name].get<std::string>());
    }
    else {
        throw std::runtime_error("Unsupported field type: " + type);
    }
}

void CommandBuilder::encode_varint(std::vector<uint8_t>& frame, uint32_t value) {
    while (value >= 0x80) {
        frame.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    frame.push_back(static_cast<uint8_t>(value));
}

// void CommandBuilder::add_checksum(std::vector<uint8_t>& frame) {
//     // 简化的CRC16实现 (实际项目应使用标准算法)
//     uint16_t crc = 0;
//     for (uint8_t byte : frame) {
//         crc = (crc << 8) ^ byte;
//     }
//     frame.push_back(static_cast<uint8_t>(crc >> 8));
//     frame.push_back(static_cast<uint8_t>(crc & 0xFF));
// }