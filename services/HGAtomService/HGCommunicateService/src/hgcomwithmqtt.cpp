#include "hgcomwithmqtt.h"
#include <memory>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include "hglog4cplus.h"

namespace std {
    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args){
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}


namespace HGMACHINE{
const std::string SERVER_ADDRESS = "tcp://localhost:1883";  // MQTT 服务器地址
const std::string CLIENT_ID = "mqtt_cpp_publisher";
//const std::string CLIENT_ID = "mqtt_cpp_subscriber";
const std::string TOPIC = "test/topic";

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
class Callback: public virtual mqtt::callback{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout<<"Received message: "<<msg->to_string()<<std::endl;
    }
};

    HgComWithMqtt::HgComWithMqtt(const std::string& server_address,const std::string& client_id, const std::string& topic_name)
        : m_server_address(server_address),
        m_client_id(client_id),
        m_topic(topic_name)
    {
        m_client=std::make_unique<mqtt::client>(m_server_address,m_client_id);       
    }
   
    void HgComWithMqtt::connect(){
        try{
            m_client->connect();
            std::cout<<"Connected to MQTT broker: "<<m_server_address<<std::endl;
        } catch (const mqtt::exception& exec){
            std::cerr<<"MQTT Error: "<<exec.what()<<std::endl;
        }
    }  
    void HgComWithMqtt::changeServer(const std::string& new_server){
        m_server_address = new_server;
        m_client = std::make_unique<mqtt::client>(m_server_address, m_client_id);
        std::cout << "MQTT server changed to: " << new_server << std::endl;
    }
    void HgComWithMqtt::publish(const std::string& message){
        try{
            mqtt::message_ptr msg=mqtt::make_message(m_topic,message);
            msg->set_qos(1);
            m_client->publish(msg);
            std::cout<<"Message published: "<<message<<std::endl;
        } catch (const mqtt::exception& exec){
            std::cerr<<"Publish Error: "<<exec.what()<<std::endl;
        }
    }
    void HgComWithMqtt::subscribe(const std::string &message)
    {
        try
        {
            // m_client->set_callback(mqtt::callback(Callback));
            m_client->subscribe(m_topic);
            std::cout << "Already subscribe topic: " << m_topic << std::endl;
        }
        catch (const mqtt::exception &e)
        {
            std::cerr << "Subscribe Error: " << e.what() << std::endl;
        }
    }
    // 断开连接
    void HgComWithMqtt::disconnect(){
        try{
            m_client->disconnect();
            std::cout<<"Disconnected from MQTT broker."<<std::endl;
        } catch (const mqtt::exception& exec){
            std::cerr<<"Disconnect Error: "<<exec.what()<<std::endl;
        }
    }
#endif

    std::string HgComWithLibMQTT::result;
    std::vector<std::map<std::string, std::string>> HgComWithLibMQTT::resultMap;
    struct mosquitto* HgComWithLibMQTT::mosq; // MQTT 客户端实例
    // HgComWithLibMQTT::HgComWithLibMQTT() : 
    //     mqttHost("localhost"), mqttUser("jwp9Gz8CQhr4DLiCPURT"), mqttPort(1883), running(true) {
    
    //     // 一个日志文件
    //     // logger = spdlog::basic_logger_mt("DFP", "dfpLog.txt");
    //     // 定期日志文件
    //     // logger = spdlog::daily_logger_mt("dfpLogger", "logs/dfpLog.txt", 0, 0, true, 7);
    
    //     // logger->set_level(spdlog::level::info);
    
    //     mosquitto_lib_init();
    //     mosq = mosquitto_new("sub_client", false, this);
    //     mosquitto_connect_callback_set(mosq, on_connect);
    //     mosquitto_disconnect_callback_set(mosq, on_disconnect);
    //     if (mosq) {
    //         int ret = mosquitto_connect(mosq, mqttHost.c_str(), mqttPort, 60);
    //         if (ret != MOSQ_ERR_SUCCESS) {
    //             // logger->error("[MQTT] 连接失败: host={}, port={}, 错误码={}", mqttHost, mqttPort, ret);
    //             // logger->flush();
    //         } else {
    //             // logger->info("[MQTT] 连接成功: host={}, port={}", mqttHost, mqttPort);
    //             // logger->flush();
    //         }
    //     } else {
    //         // logger->error("[MQTT] 初始化失败!");
    //     }
    //     // 设置回调函数，处理收到的消息
    //     mosquitto_message_callback_set(mosq, on_message);
    // }

    HgComWithLibMQTT::HgComWithLibMQTT(const std::string& host, const std::string& user,int port):
        mqttHost(host),mqttUser(user),mqttPort(port),running(true)
    {
        mosquitto_lib_init();
        mosq = mosquitto_new("sub_client", false, this);
        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_disconnect_callback_set(mosq, on_disconnect);
        if (mosq) {
            int ret = mosquitto_connect(mosq, mqttHost.c_str(), mqttPort, 60);
            if (ret != MOSQ_ERR_SUCCESS) {
                printf("[MQTT] 连接失败: host={%s}, port={%d}, 错误码={%d}", mqttHost.c_str(), mqttPort, ret);
                // logger->error("[MQTT] 连接失败: host={}, port={}, 错误码={}", mqttHost, mqttPort, ret);
                // logger->flush();
            } else {
                printf("[MQTT] 连接成功: host={%s}, port={%d}, 错误码={%d}", mqttHost.c_str(), mqttPort, ret);
                // logger->info("[MQTT] 连接成功: host={}, port={}", mqttHost, mqttPort);
                // logger->flush();
            }
        } else {
            printf("[MQTT] 初始化失败!");
            // logger->error("[MQTT] 初始化失败!");
        }
        // 设置回调函数，处理收到的消息
        mosquitto_message_callback_set(mosq, on_message);
        printf("[MQTT] callback\n");
    }
    HgComWithLibMQTT::~HgComWithLibMQTT() {
        running = false;
        if (mosq) {
            mosquitto_disconnect(mosq);
            mosquitto_destroy(mosq);
        }
        mosquitto_lib_cleanup();
    }
    // std::map<std::string, std::string> HgComWithLibMQTT::parseMSG(const std::string &msg)
    // {
    //     std::map<std::string, std::string> result;

    //     // 去掉开头和结尾的花括号
    //     std::string s = msg;
    //     if (!s.empty() && s.front() == '{' && s.back() == '}')
    //     {
    //         s = s.substr(1, s.size() - 2);
    //     }

    //     // 清除转义字符和多余的反斜杠
    //     s.erase(std::remove(s.begin(), s.end(), '\\'), s.end());

    //     // 使用字符串流来逐个读取键值对
    //     std::stringstream ss(s);
    //     std::string pair;

    //     // 按逗号分割各个 key:value
    //     while (std::getline(ss, pair, ','))
    //     {
    //         size_t colonPos = pair.find(':');
    //         if (colonPos == std::string::npos)
    //             continue;

    //         std::string key = pair.substr(0, colonPos);
    //         std::string value = pair.substr(colonPos + 1);

    //         // 去掉 key 和 value 中的引号
    //         key.erase(remove(key.begin(), key.end(), '\"'), key.end());
    //         value.erase(remove(value.begin(), value.end(), '\"'), value.end());

    //         // 去除 value 的前后空格
    //         key=key.substr(key.find_first_not_of(" \t\r\n"));
    //         key=key.substr(0,key.find_last_not_of(" \t\r\n")+1);

    //         value=value.substr(value.find_first_not_of(" \t\r\n"));
    //         value=value.substr(0,value.find_last_not_of(" \t\r\n")+1);

    //         // 存入 map
    //         result[key] = value;
    //     }

    //     return result;
    // }

    std::string safe_trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        size_t end = str.find_last_not_of(" \t\r\n");
        if (start != std::string::npos && end != std::string::npos) {
            return str.substr(start, end - start + 1);
        }
        return "";
    }
    
    std::map<std::string, std::string> HgComWithLibMQTT::parseMSG(const std::string &msg) {
        std::map<std::string, std::string> result;
    
        std::string s = msg;
    
        // 去掉开头和结尾的花括号（如果有）
        if (!s.empty() && s.front() == '{' && s.back() == '}') {
            s = s.substr(1, s.size() - 2);
        }
    
        // 清除所有转义字符和反斜杠
        s.erase(std::remove(s.begin(), s.end(), '\\'), s.end());
    
        std::stringstream ss(s);
        std::string pair;
    
        while (std::getline(ss, pair, ',')) {
            size_t colonPos = pair.find(':');
            if (colonPos == std::string::npos) {
                std::cerr << "[WARNING] 跳过无效字段（没有冒号）: " << pair << std::endl;
                continue;
            }
    
            std::string key = pair.substr(0, colonPos);
            std::string value = pair.substr(colonPos + 1);
    
            // 去掉引号
            key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
            value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
    
            // 去掉前后空格
            key = safe_trim(key);
            value = safe_trim(value);
    
            if (!key.empty()) {
                result[key] = value;
            } else {
                std::cerr << "[WARNING] 空 key 被跳过。原始内容: " << pair << std::endl;
            }
        }
    
        return result;
    }

    void HgComWithLibMQTT::sendMQTTMessage(const std::string &key, const std::string &value, const std::string &time, const std::string topic)
    {
        if (key.empty() || value.empty())
        {
            std::cerr << "[ERROR] key 或 value 为空!" << std::endl;
            return;
        }

        std::string sendValue = "{\\\"" + key + "\\\": " + value + ", \\\"ts\\\": \\\"" + time + "\\\"}";

        // 构造 mosquitto_pub 命令
        std::string debugCommand = "mosquitto_pub -d -q 1 -h " + mqttHost +
                                   " -p " + std::to_string(mqttPort) +
                                   " -t \"" + topic + "\"" +
                                   " -u \"" + mqttUser + "\"" +
                                   " -m \"" + sendValue + "\"";

        // 使用 mosquitto 库发布消息
        if (mosq)
        {
            int ret = mosquitto_publish(mosq, nullptr, topic.c_str(), sendValue.size(), sendValue.c_str(), 1, false);
            if (ret != MOSQ_ERR_SUCCESS)
            {
                // logger->error("[MQTT] 发送失败: topic={}, message={}, 错误码={}", topic, sendValue, ret);
                // logger->flush();
            }
            else
            {
                // logger->info("[MQTT] 发送成功: topic={}, message={}", topic, sendValue);
                // logger->flush();
            }

            ret = mosquitto_loop(mosq, 0, 1);
            if (ret != MOSQ_ERR_SUCCESS)
            {
                // logger->error("[MQTT] 代理消息确认失败，错误码={}", ret);
                // logger->flush();
            }
        }
    }
    // 时间戳转换函数（毫秒级时间戳 -> YYYY-MM-DD HH:MM:SS.mmm）
    string timestampToFullDateTime(long long timestamp)
    {
        // 1️⃣ 将毫秒级时间戳转换为秒和毫秒
        time_t seconds = timestamp / 1000;
        int milliseconds = timestamp % 1000;

        // 2️⃣ 转换为 tm 结构（本地时间）
        struct tm timeStruct;
        localtime_r(&seconds, &timeStruct); // 线程安全版本（Linux/macOS）
        // localtime_s(&timeStruct, &seconds); // Windows 版本（如果需要）

        // 3️⃣ 格式化时间为 "YYYY-MM-DD HH:MM:SS.mmm"
        char buffer[25]; // 足够存储 "YYYY-MM-DD HH:MM:SS.mmm"
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeStruct);

        // 4️⃣ 组合字符串，加入毫秒部分
        string formattedTime = string(buffer) + "." + to_string(milliseconds);

        return formattedTime;
    }

    std::map<std::string,std::string> decodeJsonStr(const std::string& jsonString){
        // 解析 JSON
        json jsonData = json::parse(jsonString);
    
        // 创建 std::map
        std::map<string, string> dataMap;
    
        // 解析 deviceNum 和 sessionId
        dataMap["deviceNum"] = jsonData["deviceNum"];
        dataMap["sessionId"] = jsonData["sessionId"];
    
        // 解析 dataList 数组
        for (const auto& item : jsonData["dataList"]) {
            string key = item["dataName"];  // 例如 "温度"
            string value = item["dataValue"];  // 例如 "-10.1"
            if (key=="电导率"){
                dataMap[key] = value;
                dataMap["ts"]=timestampToFullDateTime(item["dataLogTime"]);
            } else if (key=="pH"){
                dataMap[key] = value;
                dataMap["ts"]=timestampToFullDateTime(item["dataLogTime"]);
            }
        }
        return dataMap;
    }
    void HgComWithLibMQTT::on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
    {
        std::ostringstream logtext;
        HgComWithLibMQTT *self = static_cast<HgComWithLibMQTT *>(obj); // 获取类的实例

        // 打印收到的消息
        std::string receivedMessage(static_cast<const char *>(message->payload), message->payloadlen);
        logtext << "[INFO] 收到消息: topic=" << message->topic << ", message=" << receivedMessage;
        // std::cout << "[INFO] 收到消息: topic=" << message->topic << ", message=" << receivedMessage << std::endl;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);
#endif
        // 解析消息并存储结果
        // self->message_result[message->topic] = receivedMessage;
        result.clear(); // 清空上次的结果
        result = receivedMessage;
        if (result.find("dataList")==std::string::npos){
            resultMap.push_back(parseMSG(result));
        } else {
            resultMap.push_back(decodeJsonStr(result));
        }
    }
    void HgComWithLibMQTT::loop()
    {
        int ret = mosquitto_loop_start(mosq); // 启动事件循环
        if (ret != MOSQ_ERR_SUCCESS)
        {
            std::cerr << "Unable to start loop: " << mosquitto_strerror(ret) << std::endl;
            // exit(1);
        }
    }
    void HgComWithLibMQTT::start(){
        std::string topic="v1/devices/me/telemetry";
        subscribe(topic.c_str());
        // 启动事件循环
        loop();
    }
    // 订阅 MQTT 主题
    void HgComWithLibMQTT::subscribe(const char *topic)
    {
        int ret = mosquitto_subscribe(mosq, nullptr, topic, 2);
        if (ret != MOSQ_ERR_SUCCESS)
        {
            std::cerr << "Unable to subscribe to topic: " << topic << std::endl;
            // exit(1);
        }
    }
    std::vector<std::map<std::string, std::string>> HgComWithLibMQTT::recvMQTTMessage(const std::string &topic)
    {
        if (mosq)
        {
            
        }
        else
        {
            std::cerr << "[ERROR] mosquitto 对象未初始化!" << std::endl;
        }
        std::vector<std::map<std::string,std::string>> resultMap_bak=resultMap;
        resultMap.clear();
        return resultMap_bak;
    }
}
