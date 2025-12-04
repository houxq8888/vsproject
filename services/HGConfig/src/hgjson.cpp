#include "hgjson.h"
#include <fstream>

namespace HGMACHINE {

HGJson::HGJson()
{
}

HGJson::~HGJson()
{
}


void HGJson::jsonReader(const std::string &filename)
{
    std::ifstream jfile(filename);
    jfile >> m_json;
}
void HGJson::jsonWriter(const std::string &filename)
{
    std::ofstream jfile(filename);
    jfile << m_json;
}
std::map<std::string,std::string> HGJson::decodeJsonStr(const std::string& jsonString){
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
        dataMap[key] = value;
    }
    return dataMap;
}
}
