#ifndef HGJSON_H
#define HGJSON_H

#include "HGConfig_global.h"
#include "json.hpp"
#include <map>
#include <vector>
#include <string>

using json=nlohmann::json;

namespace HGMACHINE {

class HGJson
{
public:    
    HGJson();
    ~HGJson();

    void jsonReader(const std::string& filename);
    void jsonWriter(const std::string& filename);
    std::map<std::string,std::string> decodeJsonStr(const std::string& jsonString);

    template<typename T>
    static std::string serialize(const T& obj);

    template<typename T>
    static bool deserialize(const std::string& jsonStr, T& obj);

private:
    json m_json;
};

template<typename T>
std::string HGJson::serialize(const T& obj) {
    json j = obj;
    return j.dump();
}

template<typename T>
bool HGJson::deserialize(const std::string& jsonStr, T& obj) {
    try {
        json j = json::parse(jsonStr);
        obj = j.get<T>();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

}

#endif // HGJSON_H
