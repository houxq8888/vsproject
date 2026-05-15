#ifndef HGCOMWITHMQTT_H
#define HGCOMWITHMQTT_H

#include "hgbasecom.h"
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <iostream>
#else
#include <mqtt/client.h>
#endif
#include <mosquitto.h>
#include <vector>
#include <map>
#include "json.hpp"

using json=nlohmann::json;

namespace HGMACHINE {
    #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #else
    class HgComWithMqtt : public HGBaseCommon{
        public:
            HgComWithMqtt(const std::string& server_address,const std::string& client_id,const std::string& topic_name);
            
            void connect();
            void publish(const std::string& message);
            void subscribe(const std::string& message);
            void changeServer(const std::string& new_server);

            void disconnect();
        private:
            std::unique_ptr<mqtt::client> m_client;
            std::string m_topic;
            std::string m_server_address;
            std::string m_client_id;
    };
    #endif

    class HgComWithLibMQTT{
        public:
            // HgComWithLibMQTT();
            HgComWithLibMQTT(const std::string& host, const std::string& user,int port);
            ~HgComWithLibMQTT();

            std::vector<std::map<std::string, std::string>> recvMQTTMessage(const std::string &topic = "v1/devices/me/telemetry");

            
            static void start();


        private:
            // 订阅MQTT pub发布的消息
            static void subscribe(const char *topic);
            // 启动事件循环
            static void loop();
            void sendMQTTMessage(const std::string& key, const std::string& value, const std::string& time, const std::string topic="v1/devices/me/telemetry");

            // 获取订阅信息
            static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message);

            // 解析订阅到的消息
            static std::map<std::string, std::string> parseMSG(const std::string& msg);
    
            static void on_connect(struct mosquitto* mosq, void* obj, int rc) {
                if (rc == 0) {
                    std::cout << "✅ 连接成功\n";
                    start();
                } else {
                    std::cerr << "❌ 连接失败，错误代码: " << rc << "\n";
                }
            }
        
            static void on_disconnect(struct mosquitto* mosq, void* obj, int rc) {
                std::cerr << "⚠️ 连接断开，尝试重新连接...\n";
                mosquitto_reconnect(mosq);
            }
            
        private:
            static std::vector<std::map<std::string, std::string>> resultMap;
            static std::string result;
            static struct mosquitto* mosq; // MQTT 客户端实例
            std::string mqttHost;
            std::string mqttUser;
            int mqttPort;
            bool running;
    
    };
}
#endif