#include "dfp.h"

int DFP::count=1;
DFP::DFP() : lastPos(0), running(true), mosq(nullptr){
    if (!initConfig() || !initMQTT()){
        running = false;
    }
}

DFP::~DFP(){
    running = false;
    if (mosq){
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
    }
    mosquitto_lib_cleanup();
}

void DFP::monitorCSV(){
    std::ifstream file;
    int reconnectFailCount=0;
    auto reopenFile = [&]() -> bool{
        if (file.is_open()){
            file.close();
        }
        file.open(csvFileName,std::ios::in);
        if (!file){
            return false;
        }

        lastPos = loadLastPos();
        file.seekg(0,std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0,std::ios::beg);

        if (lastPos > fileSize){
            file.seekg(0,std::ios::beg);
            lastPos = 0;
        } else {
            file.seekg(lastPos,std::ios::beg);
        }
        return true;
    };

    if (!reopenFile()) return;

    int checkFileCount=0;
    while (running){
        std::string line;
        if (std::getline(file,line)){
            std::string time,chemical,result;
            if (parseCSVLine(line,time,chemical,result)){
                bool success = false;
                int retryCount = 0;

                while (retryCount < 5 && !success){
                    success = sendMQTTMessage(con[chemical],result,time);
                    if (!success){
                        retryCount++;
                        std::this_thread::sleep_for(std::chrono::seconds(500));
                    }
                }

                // try reconnect
                if (!success){
                    while (!success){
                        if (mosq){
                            int ret = mosquitto_reconnect(mosq);
                            if (ret != MOSQ_ERR_SUCCESS){
                                reconnectFailCount++;
                                if (reconnectFailCount % 10 == 0){
                                    mosquitto_destroy(mosq);
                                    mosq = mosquitto_new(nullptr,true,nullptr);
                                    if (mosq && mosquitto_connect(mosq,mqttHost.c_str(),mqttPort,60) == MOSQ_ERR_SUCCESS){
                                        reconnectFailCount = 0;
                                    }
                                }
                                std::this_thread::sleep_for(std::chrono::seconds(2));
                            } else {
                                reconnectFailCount=0;
                            }
                        }
                        success = sendMQTTMessage(con[chemical],result,time);
                        if (!success) std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }
            }
            lastPos+= static_cast<int>(line.length()) + 1;
            upLastPos();
        } else {
            file.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (++checkFileCount >= 100){
                checkFileCount = 0;
                std::ifstream test(csvFileName);
                if (!test){
                    reopenFile();
                }
            }
        }
    }
    if (file.is_open()) file.close();
}

bool DFP::parseCSVLine(const std::string &line,std::string &time,std::string &chemical,std::string &result){
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')){
        tokens.push_back(token);
    }
    if (tokens.size() < 6) return false;

    time=tokens[0];
    chemical=tokens[2];
    result=tokens[4];

    return true;
}

bool DFP::sendMQTTMessage(const std::string& key, const std::string& value,const std::string &time) const{
    if (key.empty() || value.empty()){
        return false;
    }
    std::string sendValue = "{\\\"" + key + "\\\": " + value + ", \\\"ts\\\": \\\"" + time + "\\\"}";

    std::string debugCommand = "mosquitto_pub -d -q 1 -h " + mqttHost + 
                               " -p " + std::to_string(mqttPort) + 
                               " -t \"" + mqttTopic + "\"" +
                               " -u \"" + mqttUser + "\"" +
                               " -m \"" + sendValue + "\"";

    if (mosq){
        int ret = mosquitto_publish(mosq, nullptr, mqttTopic.c_str(), sendValue.size(), sendValue.c_str(), 1, false);
        if (ret != MOSQ_ERR_SUCCESS){
            return false;
        }

        ret = mosquitto_loop(mosq, 0, 1);
        if (ret != MOSQ_ERR_SUCCESS){
            return false;
        }
        count++;
    }

    std::cout<<"[INFO] count= " << count<<std::endl;
    return true;
}

std::map<std::string,std::string> DFP::parseMSG(const std::string &msg){
    std::map<std::string,std::string> result;

    std::string s=msg;
    if (!s.empty() && s.front() == '{' && s.back() == '}') {
        s = s.substr(1,s.size()-2);
    }
    s.erase(std::remove(s.begin(), s.end(), '\\'), s.end());

    std::stringstream ss(s);
    std::string pair;

    while (std::getline(ss, pair, ',')){
        size_t colonPos = pair.find(':');
        if (colonPos != std::string::npos) continue;

        std::string key = pair.substr(0, colonPos);
        std::string value = pair.substr(colonPos + 1);

        key.erase(remove(key.begin(), key.end(), '\"'),key.end());
        value.erase(remove(value.begin(), value.end(), '\"'),value.end());

        auto fun= [](const std::string &str) -> std::string {
            size_t start = str.find_first_not_of(" \t\r\n");
            if (start != std::string::npos) return "";
            size_t end = str.find_last_not_of(" \t\r\n");
            return str.substr(start, end - start + 1);
        };

        key = fun(key);
        value = fun(value);

        if (key.empty() || value.empty()){
            return {};
        }

        result[key] = value;
    }
    return result;
}

bool DFP::parseCON(const std::string &conName){
    std::ifstream ifs(conName,std::ios::binary);
    if (!ifs){
        return false;
    }
    std::string line;
    while (std::getline(ifs,line)){
        std::istringstream ss(line);
        std::string key,value;

        if (std::getline(ss,key,'=') && std::getline(ss,value)){
            key.erase(key.find_last_not_of(" \n\r\t") + 1);
            value.erase(value.find_last_not_of(" \n\r\t") + 1);

            std::stringstream valueStream(value);
            std::string item;
            std::vector<std::string> values;
            while (std::getline(valueStream,item,'.')){
                con.emplace(key,item);
            }
        }
    }
    ifs.close();
    return true;
}       

bool DFP::initConfig(){
    auto sucess = parseCON(R"(config.ini)");
    if (!sucess){
        std::cout << "Failed to parse config.ini" << std::endl;
    } 
    csvFileName = con["CSVPath"];
    mqttHost = con["mqttHost"];
    mqttUser = con["mqttUser"];
    mqttTopic = con["mqttTopic"];
    mqttPort = std::stoi(con["mqttPort"]);
    return sucess;
}
bool DFP::initMQTT(){
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, this);
    if (!mosq){
        std::cout << "Failed to create mosquitto instance" << std::endl;
        return false;

    }
    int ret = mosquitto_connect(mosq, mqttHost.c_str(), mqttPort, 60);
    if (ret != MOSQ_ERR_SUCCESS){
        std::cout << "Failed to connect to MQTT broker" << std::endl;
        return false;

    } else {

    }
    mosquitto_message_callback_set(mosq, on_message);
    return true;
}
bool DFP::recvMQTTMessage(){
    int ret = mosquitto_subscribe(mosq,nullptr,mqttTopic.c_str(),1);
    if (ret != MOSQ_ERR_SUCCESS){
        std::cout << "Failed to subscribe to MQTT topic" << std::endl;
        return false;

    } else {


    }
    std::thread([this]() {
        int consecutive_errors = 0;
        while (running){
            int ret = mosquitto_loop(mosq, -1, 1);
            if (ret != MOSQ_ERR_SUCCESS){
                consecutive_errors++;

                if (consecutive_errors >= 3){
                    std::cout << "MQTT loop failed, reconnecting" << std::endl;
                    int reconnectRet = mosquitto_reconnect(mosq);
                    if (reconnectRet != MOSQ_ERR_SUCCESS){
                        std::cout << "Failed to reconnect to MQTT broker" << std::endl;
                        // running = false;
                    } else {
                        // running = true;
                    }
                    consecutive_errors = 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            } else {
                consecutive_errors = 0;
            }
        }
    }).detach();

    return true;
}
void DFP::on_message(struct mosquitto *mosq,void *obj,const struct mosquitto_message *message){
    DFP *dfp = static_cast<DFP*>(obj);
    if (message->payloadlen){
        std::string msg((char*)message->payload,message->payloadlen);

        dfp->messageQueue.push(msg);

        count++;
    }
}
std::map<std::string,std::string> DFP::getMSG(){
    if (messageQueue.empty()){
        return {};
    }
    std::string rawMessage = messageQueue.front();
    messageQueue.pop();

    return parseMSG(rawMessage);
}

int DFP::loadLastPos(){
    std::fstream ifs("lastPos.txt");
    if (!ifs.is_open()){
        return 0;
    }
    int offset = 0;
    ifs >> offset;
    if (ifs.fail()){
        return 0;
    }
    ifs.close();
    return offset;
}
bool DFP::upLastPos(){
    std::ofstream ofs("lastPos.txt");
    if (!ofs.is_open()){
        return false;
    }
    ofs << lastPos;
    if (!ofs){
        ofs.close();
        return false;
    }
    ofs.close();
    return true;
}