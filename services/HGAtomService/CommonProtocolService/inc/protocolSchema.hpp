#ifndef PROTOCOLSCHEMA_H
#define PROTOCOLSCHEMA_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "json.hpp"
#include <mutex>
#include <sys/stat.h>

using json = nlohmann::json;

    constexpr uint8_t FRAME_HEADER[] = {0xEE, 0xFF};
    constexpr uint8_t FRAME_FOOTER[] = {0xFF, 0xFC, 0xFF, 0xFF};

class ProtocolSchema {
public:
    // 添加默认构造函数
    ProtocolSchema(): data_(json::object()) {}
    // 保留现有构造函数
    explicit ProtocolSchema(const std::string& json_path) {
        load(json_path);
    }
    
    // 允许移动
    ProtocolSchema(ProtocolSchema&&) = default;
    ProtocolSchema& operator=(ProtocolSchema&&) = default;
    
    // 禁用拷贝
    ProtocolSchema(const ProtocolSchema&) = delete;
    ProtocolSchema& operator=(const ProtocolSchema&) = delete;

    // 新增状态检查
    std::string check_data_state() const {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            std::stringstream ss;
            ss << "Type: " << data_.type_name()
               << ", Valid: " << (data_.is_object() && data_.contains("commands"))
               << ", Size: " << (data_.is_object() ? data_.size() : 0);
            return ss.str();
        }
        catch (...)
        {
            return "Invalid JSON state (exception occurred)";
        }
    }
    bool load(const std::string &json_path)
    {
        struct stat buffer;
        if (stat(json_path.c_str(), &buffer) != 0)
        {
            std::cerr << "Protocol file not found: " << json_path << std::endl;
            return false;
        }

        try
        {
            std::ifstream f(json_path);
            json new_data = json::parse(f);

            // 强化验证
            if (!new_data.is_object())
            {
                throw std::runtime_error("Root is not a JSON object");
            }

            if (!new_data.contains("commands") || !new_data["commands"].is_array())
            {
                throw std::runtime_error("Missing or invalid 'commands' array");
            }

            // 验证至少有一个命令
            if (new_data["commands"].empty())
            {
                throw std::runtime_error("Empty commands list");
            }

            // 原子性更新
            {
                std::lock_guard<std::mutex> lock(mutex_);
                data_ = std::move(new_data);
            }

            std::cout << "✅ Loaded protocol with "
                      << data_["commands"].size() << " commands" << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "💥 Protocol load failed: " << e.what() << std::endl;
            reset_data();
            return false;
        }
    }
    bool is_valid() const {
        return data_.is_object() && data_.contains("commands");
    }
    json get_command(uint32_t cmd_id) const
    {
        // 先检查对象基本有效性
        if (!this)
        {
            std::cerr << "CRITICAL: Method called on null object!" << std::endl;
            return json();
        }

        // 强化类型检查
        // std::cout << "Object address: " << (void *)this << std::endl;
        // std::cout << "Data address: " << &data_ << std::endl;

        std::cout << "Current data state: "
                  << (data_.is_null() ? "null" : data_.type_name()) << std::endl;

        std::lock_guard<std::mutex> lock(mutex_);

        // 深度状态检查
        if (data_.type() == json::value_t::discarded)
        {
            std::cerr << "Data is in discarded state!" << std::endl;
            return json();
        }

        if (!data_.is_object()) {
            std::cerr << "Data corrupted! Current type: " << data_.type_name() << std::endl;
            return json();
        }

        std::cout << "Searching for command ID: " << cmd_id << std::endl;

        if (!data_.is_object())
        {
            std::cerr << "Data is not a JSON object" << std::endl;
            return json();
        }

        if (!data_.contains("commands"))
        {
            std::cerr << "Missing 'commands' field" << std::endl;
            return json();
        }

        const json &commands = data_["commands"];
        if (!commands.is_array())
        {
            std::cerr << "'commands' is not an array (actual type: "
                      << commands.type_name() << ")" << std::endl;
            return json();
        }

        std::cout << "Available command IDs: ";
        for (const auto &cmd : commands)
        {
            if (cmd.is_object() && cmd.contains("id"))
            {
                std::cout << cmd["id"] << " ";
            }
        }
        std::cout << std::endl;

        for (const auto &cmd : commands)
        {
            try
            {
                if (cmd.is_object() &&
                    cmd.contains("id") &&
                    cmd["id"].is_number_unsigned() &&
                    cmd["id"].get<uint32_t>() == cmd_id)
                {
                    // std::cout << "Found command: " << cmd.dump() << std::endl;
                    return cmd;
                }
            }
            catch (const json::exception &e)
            {
                std::cerr << "Command parse error: " << e.what() << std::endl;
            }
        }

        std::cerr << "Command ID " << cmd_id << " not found" << std::endl;
        return json();
    }

    const json& get_data() const { return data_; }

    std::vector<uint32_t> get_all_command_ids() const {
        std::vector<uint32_t> ids;
        if (data_.is_object() && data_.contains("commands")) {
            for (const auto& cmd : data_["commands"]) {
                if (cmd.is_object() && cmd.contains("id") && cmd["id"].is_number_unsigned()) {
                    ids.push_back(cmd["id"].get<uint32_t>());
                }
            }
        }
        return ids;
    }

    json get_all_commands() const {
        if (data_.is_object() && data_.contains("commands")) {
            return data_["commands"];
        }
        return json::array();
    }

private:
    mutable std::mutex mutex_;
    json data_ = json::object();
    void reset_data() {
        data_ = json::object(); // 确保始终是有效对象
    }
};

#endif