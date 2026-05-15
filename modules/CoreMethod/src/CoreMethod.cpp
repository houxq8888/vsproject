#include "CoreMethod.h"
#include "rwMethodDb.h"
#include "DatabaseManagerAdapter.h"

using namespace HGMACHINE;

class CoreMethod::Impl {
public:
    Impl() {
        m_dbManager = &DatabaseManagerAdapter::instance();
    }
    
    ~Impl() {
    }
    
    DatabaseManagerAdapter* m_dbManager;
    RWMethodDb m_rwMethodDb;
};

CoreMethod::CoreMethod() : m_impl(new Impl()) {
}

CoreMethod::~CoreMethod() {
    delete m_impl;
}

bool CoreMethod::initialize() {
    if (m_impl->m_dbManager && !m_impl->m_dbManager->IsConnected()) {
        std::string basePath = RWDb::readCurDirPath();
        RWDb::openDB(basePath);
    }
    return true;
}

void CoreMethod::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreMethod::readMethodInfo() {
    return m_impl->m_rwMethodDb.readMethodInfo();
}

std::string CoreMethod::readMethodParam(const std::string& dbName) {
    return m_impl->m_rwMethodDb.readMethodParam(dbName).param;
}

std::vector<std::string> CoreMethod::getAllTables(const std::string& dbName) {
    return RWDb::getAllTables(dbName);
}

void CoreMethod::writeMethodManageRecord(const std::map<std::string, std::string>& info) {
    m_impl->m_rwMethodDb.writeMethodManageRecord(info);
}

void CoreMethod::writeMethodRecord(const std::string& dbName, bool coverFlag, 
                                   const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_rwMethodDb.writeMethodRecord(dbName, coverFlag, info);
}
std::string CoreMethod::getMethodPrefix(){
    return MethodDBName;
}
std::string CoreMethod::getMethodTableName(){
    return METHODMANAGENAME;
}
void CoreMethod::deleteRecord(const std::string& key, 
                             const std::string& value) {
    RWDb::deleteRecord(METHODMANAGENAME, key, value);
}

void CoreMethod::deleteDB(const std::string& dbName) {
    RWDb::deleteDB(dbName);
}

void CoreMethod::clearMethodManageRecord() {
    m_impl->m_rwMethodDb.clearMethodManageRecord();
}

std::map<std::string, std::string> CoreMethod::getMethodMap(int index, 
                                                              const std::string& indexStr,
                                                              const std::string& name,
                                                              const std::string& type,
                                                              const std::string& createTime,
                                                              const std::string& dbName,
                                                              const std::string& param) {
    Method method;
    method.indexStr = indexStr;
    method.name = name;
    method.type = type;
    method.createTime = createTime;
    method.dbName = dbName;
    method.param = param;
    return m_impl->m_rwMethodDb.getMethodMap(index, method);
}

std::vector<std::map<std::string, std::string>> CoreMethod::getMethodParamMap(const std::string& indexStr,
                                                                                 const std::string& name,
                                                                                 const std::string& type,
                                                                                 const std::string& createTime,
                                                                                 const std::string& dbName,
                                                                                 const std::string& param) {
    Method method;
    method.indexStr = indexStr;
    method.name = name;
    method.type = type;
    method.createTime = createTime;
    method.dbName = dbName;
    method.param = param;
    return m_impl->m_rwMethodDb.getMethodParamMap(method);
}
