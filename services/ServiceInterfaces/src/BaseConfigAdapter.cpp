#include "BaseConfigAdapter.h"
#include "hgjson.h"
#include "hgxml.h"
#include "hgtxt.h"
#include "hgcsv.h"
#include "hgpdf.h"

class BaseConfigAdapter::Impl {
public:
    HGMACHINE::HGJson* json;
    std::string configFile;
    
    Impl() : json(new HGMACHINE::HGJson()) {}
    
    ~Impl() {
        if (json) {
            delete json;
            json = nullptr;
        }
    }
};

BaseConfigAdapter::BaseConfigAdapter() : m_impl(new Impl()) {
}

BaseConfigAdapter::~BaseConfigAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

bool BaseConfigAdapter::GetConfig(const std::string& key, std::string& value) {
    if (!m_impl || !m_impl->json) return false;
    auto configMap = m_impl->json->decodeJsonStr("{}");
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool BaseConfigAdapter::SetConfig(const std::string& key, const std::string& value) {
    return true;
}

bool BaseConfigAdapter::GetIntConfig(const std::string& key, int& value) {
    std::string strValue;
    if (GetConfig(key, strValue)) {
        try {
            value = std::stoi(strValue);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool BaseConfigAdapter::SetIntConfig(const std::string& key, int value) {
    return SetConfig(key, std::to_string(value));
}

bool BaseConfigAdapter::GetBoolConfig(const std::string& key, bool& value) {
    std::string strValue;
    if (GetConfig(key, strValue)) {
        value = (strValue == "true" || strValue == "1");
        return true;
    }
    return false;
}

bool BaseConfigAdapter::SetBoolConfig(const std::string& key, bool value) {
    return SetConfig(key, value ? "true" : "false");
}

bool BaseConfigAdapter::GetDoubleConfig(const std::string& key, double& value) {
    std::string strValue;
    if (GetConfig(key, strValue)) {
        try {
            value = std::stod(strValue);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool BaseConfigAdapter::SetDoubleConfig(const std::string& key, double value) {
    return SetConfig(key, std::to_string(value));
}

std::vector<std::string> BaseConfigAdapter::GetAllKeys() const {
    return {};
}

bool BaseConfigAdapter::SaveConfig() {
    if (!m_impl || !m_impl->json || m_impl->configFile.empty()) return false;
    m_impl->json->jsonWriter(m_impl->configFile);
    return true;
}

bool BaseConfigAdapter::LoadConfig() {
    if (!m_impl || !m_impl->json || m_impl->configFile.empty()) return false;
    m_impl->json->jsonReader(m_impl->configFile);
    return true;
}

std::string BaseConfigAdapter::LoadTranslation(const std::string& language, const std::string& key) {
    return HGMACHINE::loadTranslation(language, key);
}
std::string BaseConfigAdapter::FindTranslationKey(const string& language, const string& key) {
    return HGMACHINE::findTranslationKey(language, key);
}


int BaseConfigAdapter::SaveTableToTxt(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) {
    return HGMACHINE::saveTableToTxt(content, savename);
}

int BaseConfigAdapter::SaveTableToCsv(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename) {
    return HGMACHINE::saveTableToCsv(content, savename);
}

int BaseConfigAdapter::SaveTableToPdf(const std::vector<std::map<std::string, std::string>>& content, const std::string& savename, const std::string& charset) {
    return HGMACHINE::saveTableToPdf(content, savename, charset);
}
