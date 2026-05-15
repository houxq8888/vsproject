#include "DatabaseManagerAdapter.h"
#include "SQLite.h"
#include <mutex>

class DatabaseManagerAdapter::Impl {
public:
    CSQLiteDB db;
    bool connected;
    int connectCount;
    std::mutex mutex;
    
    Impl() : connected(false), connectCount(0) {}
};

DatabaseManagerAdapter& DatabaseManagerAdapter::instance() {
    static DatabaseManagerAdapter instance;
    return instance;
}

DatabaseManagerAdapter::DatabaseManagerAdapter() : m_impl(new Impl()) {
}

DatabaseManagerAdapter::~DatabaseManagerAdapter() {
    if (m_impl) {
        Disconnect();
        delete m_impl;
        m_impl = nullptr;
    }
}

bool DatabaseManagerAdapter::Initialize() {
    return true;
}

bool DatabaseManagerAdapter::Connect(const std::string& dbPath) {
    std::lock_guard<std::mutex> lock(m_impl->mutex);
    
    if (m_impl->connected) {
        m_impl->connectCount++;
        return true;
    }
    
    try {
        m_impl->db.open(dbPath.c_str());
        m_impl->connected = true;
        m_impl->connectCount = 1;
        return true;
    } catch (...) {
        m_impl->connected = false;
        m_impl->connectCount = 0;
        return false;
    }
}

bool DatabaseManagerAdapter::Disconnect() {
    std::lock_guard<std::mutex> lock(m_impl->mutex);
    
    if (!m_impl->connected) {
        return true;
    }
    
    m_impl->connectCount--;
    
    if (m_impl->connectCount <= 0) {
        m_impl->db.close();
        m_impl->connected = false;
        m_impl->connectCount = 0;
    }
    
    return true;
}

bool DatabaseManagerAdapter::IsConnected() const {
    return m_impl->connected;
}

bool DatabaseManagerAdapter::Execute(const std::string& sql) {
    if (!m_impl->connected) return false;
    try {
        m_impl->db.execDML(sql.c_str());
        return true;
    } catch (...) {
        return false;
    }
}

bool DatabaseManagerAdapter::Query(const std::string& sql, std::vector<std::map<std::string, std::string>>& results) {
    if (!m_impl->connected) return false;
    try {
        CSQLiteTable table = m_impl->db.getTable(sql.c_str());
        results.clear();
        
        for (int row = 0; row < table.numRows(); row++) {
            std::map<std::string, std::string> rowData;
            for (int col = 0; col < table.numFields(); col++) {
                const char* fieldName = table.fieldName(col);
                const char* fieldValue = table.fieldValue(col);
                if (fieldValue) {
                    rowData[fieldName] = fieldValue;
                } else {
                    rowData[fieldName] = "";
                }
            }
            results.push_back(rowData);
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool DatabaseManagerAdapter::Insert(const std::string& table, const std::map<std::string, std::string>& data) {
    if (!m_impl->connected || data.empty()) return false;
    
    std::string fields;
    std::string values;
    bool first = true;
    
    for (const auto& item : data) {
        if (!first) {
            fields += ", ";
            values += ", ";
        }
        fields += item.first;
        values += "'" + item.second + "'";
        first = false;
    }
    
    std::string sql = "INSERT INTO " + table + " (" + fields + ") VALUES (" + values + ")";
    return Execute(sql);
}

bool DatabaseManagerAdapter::Update(const std::string& table, const std::map<std::string, std::string>& data, const std::string& condition) {
    if (!m_impl->connected || data.empty()) return false;
    
    std::string setClause;
    bool first = true;
    
    for (const auto& item : data) {
        if (!first) {
            setClause += ", ";
        }
        setClause += item.first + " = '" + item.second + "'";
        first = false;
    }
    
    std::string sql = "UPDATE " + table + " SET " + setClause + " WHERE " + condition;
    return Execute(sql);
}

bool DatabaseManagerAdapter::Delete(const std::string& table, const std::string& condition) {
    if (!m_impl->connected) return false;
    
    std::string sql = "DELETE FROM " + table + " WHERE " + condition;
    return Execute(sql);
}

bool DatabaseManagerAdapter::BeginTransaction() {
    if (!m_impl->connected) return false;
    try {
        return m_impl->db.Begin() == SQLITE_OK;
    } catch (...) {
        return false;
    }
}

bool DatabaseManagerAdapter::Commit() {
    if (!m_impl->connected) return false;
    try {
        return m_impl->db.Commit() == SQLITE_OK;
    } catch (...) {
        return false;
    }
}

bool DatabaseManagerAdapter::Rollback() {
    if (!m_impl->connected) return false;
    try {
        return m_impl->db.RollBack() == SQLITE_OK;
    } catch (...) {
        return false;
    }
}
