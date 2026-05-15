#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>

bool parseCON(const std::string &conName,std::unordered_map<std::string,std::string> &con){
    std::ifstream ifs(conName);
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

int main(){
    std::unordered_map<std::string, std::string> con;
    parseCON(R"(config.ini)", con);

    for (auto & it : con){
        std::cout<<it.first<<" "<<it.second<<std::endl;
    }

    return 0;
}