#ifndef HGJSON_H
#define HGJSON_H

#include "HGConfig_global.h"
#include "json.hpp"

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
    

private:
    json m_json;
};
}

#endif // HGJSON_H
