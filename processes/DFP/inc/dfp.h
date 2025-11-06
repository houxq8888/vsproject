#ifndef DFP_H
#define DFP_H

#include <stdint.h>
#include "mosquitto.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <future>
#include <thread>
#include <chrono>
#include <atomic>
#include <unordered_map>

class DFP {
    public:
        DFP();
        ~DFP();
        void monitorCSV();
        std::map<std::string, std::string> getMSG();

        bool recvMQTTMessage();

    private:
        static int count;
        std::unordered_map<std::string, std::string> con;

        std::mutex msgMutex;
        std::condition_variable msgCond;
        std::string latestMessage;

        std::queue<std::string> messageQueue;
        std::mutex queueMutex;

        std::string csvFileName;
        std::string mqttHost;
        std::string mqttUser;
        std::string mqttTopic;
        int mqttPort;
        std::streampos lastPos;
        std::atomic<bool> running;

        struct mosquitto* mosq;

        bool initConfig();
        bool initMQTT();

        bool parseCON(const std::string& conName);

        static bool parseCSVLine(const std::string& line,std::string& time,std::string& chemical,std::string& result);
        bool sendMQTTMessage(const std::string& key,const std::string& value,const std::string& time) const;

        static std::map<std::string,std::string> parseMSG(const std::string& msg);
        static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

        bool upLastPos();
        int loadLastPos();
        
};

#endif