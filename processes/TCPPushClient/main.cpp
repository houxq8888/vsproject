#include "tcpvideosender.h"  // 你的 TCPVideoSender 类
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#include <sstream>
#include <regex>
typedef struct tagMResultValue {
    std::string time;
    float value;
    std::string name;
} MResultValue;
std::string formatTimestamp(const std::string& timestamp) {
    if (timestamp.size() != 12) {
        return "Invalid timestamp length";  // 确保是12位数字
    }

    // 提取日期和时间部分
    std::string dateStr = timestamp.substr(0, 6);  // "250829"
    std::string timeStr = timestamp.substr(6, 6);  // "135145"

    // 从日期部分提取年、月、日
    std::string yearStr = "20" + dateStr.substr(0, 2);  // "2025"
    std::string monthStr = dateStr.substr(2, 2);        // "08"
    std::string dayStr = dateStr.substr(4, 2);          // "29"

    // 从时间部分提取时、分、秒
    std::string hourStr = timeStr.substr(0, 2);         // "13"
    std::string minuteStr = timeStr.substr(2, 2);       // "51"
    std::string secondStr = timeStr.substr(4, 2);       // "45"

    // 拼接成 "YYYY-MM-DD HH:MM:SS.000" 格式
    std::stringstream formattedTime;
    formattedTime << yearStr << "-" << monthStr << "-" << dayStr << " "
                  << hourStr << ":" << minuteStr << ":" << secondStr << ".000";

    return formattedTime.str();
}
std::vector<MResultValue> parseMResultValue(const std::string &data, const std::string &delStr = "") {
    std::vector<MResultValue> results;
    std::string input = data;

    // 如果有delStr，移除字符串中的所有delStr中的字符
    if (!delStr.empty()) {
        for (char ch : delStr) {
            // 删除所有出现的delStr中的字符
            input.erase(std::remove(input.begin(), input.end(), ch), input.end());
        }
    }

    printf("input = %s",input.c_str());

    // 调整正则表达式来匹配 mXX=value*time 格式的字符串，允许前后有任意字符
    std::regex pattern(R"(m(\d{2})=([+-]?\d+(\.\d+)?)\*(\d+))");
    std::smatch matches;

    // 使用正则表达式来匹配并解析字符串
    std::string::const_iterator searchStart(input.cbegin());
    while (std::regex_search(searchStart, input.cend(), matches, pattern)) {
        MResultValue resultValue;
        
        // 提取 mXX 的数字部分，并存入 name 字段
        resultValue.name = "m" + matches[1].str(); // "mXX" 格式

        // 提取 value（浮动点数）并存入 value 字段
        resultValue.value = std::stof(matches[2].str()); // 将 value 转换为 float

        // 提取 time（字符串）并存入 time 字段
        resultValue.time = matches[4].str(); // time 部分保持为字符串
        resultValue.time = formatTimestamp(resultValue.time);
        // 输出结构体的内容
        std::cout << "name = " << resultValue.name << std::endl;
        std::cout << "value = " << resultValue.value << std::endl;
        std::cout << "time = " << resultValue.time << std::endl;

        // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("name = " + resultValue.name,LOGINFO);
        // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("value = " + std::to_string(resultValue.value),LOGINFO);
        // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("time = " + resultValue.time,LOGINFO);
        results.push_back(resultValue);

        // 更新搜索位置，跳到下一个可能的匹配
        searchStart = matches.suffix().first;
    }

    if (results.empty()) {
        std::cout << "No match found for input: " << input << std::endl;
        // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("No match found for input: " + input,LOGERROR);
    }

    return results;
}
std::string bytesToAscii(const uint8_t* data, size_t len) {
    std::string result;
    result.reserve(len);

    for (size_t i = 0; i < len; ++i) {
        // 只将不可打印字符替换为 '.'，但保留 0x2E 作为 '.'
        if (data[i] == 0x2E) {
            result += '.';  // 如果是 '.' 字符 (0x2E)，直接添加
        } else if (!std::isprint(data[i])) {
            // result += '.';  // 其他不可打印字符，替换为 '.'
        } else {
            result += static_cast<char>(data[i]);  // 可打印字符，直接添加
        }
    }

    return result;
}
int main() {
    std::vector<uint8_t> vecdata={
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x6D, 0x33, 0x31, 0x3D, 0x31, 0x39, 0x39, 0x33, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x2E, 0x34, 0x34, 0x2A, 0x32, 0x35, 0x30, 0x31, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x30, 0x32, 0x30, 0x33, 0x30, 0x34, 0x30, 0x35, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x01, 0x00, 0x00, 0x00, 0x00, 
        0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x6D, 0x33, 0x31, 0x3D, 0x31, 0x39, 0x39, 0x33, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x2E, 0x34, 0x34, 0x2A, 0x32, 0x35, 0x30, 0x31, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x30, 0x32, 0x30, 0x33, 0x30, 0x34, 0x30, 0x35, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC,  
        0x01, 0x00, 0x00, 0x00, 0x00,
        0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC,  
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x6D, 0x33, 0x31, 0x3D, 0x31, 0x39, 0x39, 0x33, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x2E, 0x34, 0x34, 0x2A, 0x32, 0x35, 0x30, 0x31, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x08, 0x00, 0x00, 0x00, 0x00, 
        0x30, 0x32, 0x30, 0x33, 0x30, 0x34, 0x30, 0x35, 0x50, 0x78, 0x7D, 0x40, 0x17, 0xBC, 
        0x01, 0x00, 0x00, 0x00, 0x00, 
        0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    std::string asciiStr = bytesToAscii(vecdata.data(),vecdata.size());
    printf("asciiStr = %s\n", asciiStr.c_str());
    return 0;

    std::string inputstr = "cha=65,471,14.4,8.4,7#Px}@cha=66,470,13.9,8.4,5#Px}@end=8.45#Px}@m31=13.3Px}@70*25102Px}@0101318#";
    parseMResultValue(inputstr,"Px}@");
    return 0;


    // 推送服务器地址
    std::string url = "tcp://192.168.1.131:8885"; // 替换成服务端IP和端口

    // 视频源，可以是文件路径或者摄像头索引（摄像头请在类里改 VideoCapture 调用）
    std::string videoFile = "0"; // 0 表示默认摄像头，文件路径则写 "test.mp4"

    try {
        // 创建推流器
        TCPVideoSender sender(url);

        // 启动推流
        if (!sender.start(videoFile)) {
            std::cerr << "Failed to start TCPVideoSender!" << std::endl;
            return -1;
        }

        std::cout << "Streaming started. Press Ctrl+C to stop." << std::endl;

        // 主线程可以做其他事情，这里简单 sleep
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // 停止推流（这里永远不会执行，除非你加信号处理退出）
        sender.stop();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
