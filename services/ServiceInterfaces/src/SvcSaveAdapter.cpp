#include "SvcSaveAdapter.h"
#include "hgsavedatatodb.h"

class SvcSaveAdapter::Impl {
public:
    HGMACHINE::HGSaveDataToDB* saveDB;
    std::string tableName;
    
    Impl() : saveDB(new HGMACHINE::HGSaveDataToDB()), tableName("config") {}
    
    ~Impl() {
        if (saveDB) {
            saveDB->closeDB();
            delete saveDB;
            saveDB = nullptr;
        }
    }
};

SvcSaveAdapter::SvcSaveAdapter() : m_impl(new Impl()) {
}

SvcSaveAdapter::~SvcSaveAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

bool SvcSaveAdapter::SaveString(const std::string& key, const std::string& value) {
    if (!m_impl || !m_impl->saveDB) return false;
    return m_impl->saveDB->setStr(m_impl->tableName, key, value);
}

bool SvcSaveAdapter::LoadString(const std::string& key, std::string& value) {
    if (!m_impl || !m_impl->saveDB) return false;
    return m_impl->saveDB->getStr(m_impl->tableName, key, value);
}

bool SvcSaveAdapter::SaveInt(const std::string& key, int value) {
    return SaveString(key, std::to_string(value));
}

bool SvcSaveAdapter::LoadInt(const std::string& key, int& value) {
    std::string strValue;
    if (LoadString(key, strValue)) {
        try {
            value = std::stoi(strValue);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool SvcSaveAdapter::SaveDouble(const std::string& key, double value) {
    return SaveString(key, std::to_string(value));
}

bool SvcSaveAdapter::LoadDouble(const std::string& key, double& value) {
    std::string strValue;
    if (LoadString(key, strValue)) {
        try {
            value = std::stod(strValue);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool SvcSaveAdapter::SaveBinary(const std::string& key, const std::vector<uint8_t>& data) {
    return false;
}

bool SvcSaveAdapter::LoadBinary(const std::string& key, std::vector<uint8_t>& data) {
    return false;
}

bool SvcSaveAdapter::DeleteKey(const std::string& key) {
    if (!m_impl || !m_impl->saveDB) return false;
    std::string sql = "DELETE FROM " + m_impl->tableName + " WHERE key='" + key + "'";
    return m_impl->saveDB->writeData(sql);
}

bool SvcSaveAdapter::ClearAll() {
    if (!m_impl || !m_impl->saveDB) return false;
    m_impl->saveDB->clearDB(m_impl->tableName);
    return true;
}

bool SvcSaveAdapter::Exists(const std::string& key) const {
    std::string value;
    return const_cast<SvcSaveAdapter*>(this)->LoadString(key, value);
}

std::vector<std::string> SvcSaveAdapter::GetAllKeys() const {
    if (!m_impl || !m_impl->saveDB) return {};
    std::string sql = "SELECT key FROM " + m_impl->tableName;
    std::vector<std::map<std::string, std::string>> results;
    std::vector<std::string> keys;
    if (const_cast<HGMACHINE::HGSaveDataToDB*>(m_impl->saveDB)->readData(sql, results)) {
        for (const auto& row : results) {
            auto it = row.find("key");
            if (it != row.end()) {
                keys.push_back(it->second);
            }
        }
    }
    return keys;
}
