#include "MethodAdapter.h"
#include "CoreMethod.h"

using namespace HGMACHINE;

class MethodAdapter::Impl {
public:
    CoreMethod m_coreMethod;
};

MethodAdapter::MethodAdapter() : m_impl(new Impl()) {
}

MethodAdapter::~MethodAdapter() {
    delete m_impl;
}

bool MethodAdapter::initialize() {
    return m_impl->m_coreMethod.initialize();
}

void MethodAdapter::shutdown() {
    m_impl->m_coreMethod.shutdown();
}

std::vector<std::map<std::string, std::string>> MethodAdapter::readMethodInfo() {
    return m_impl->m_coreMethod.readMethodInfo();
}

std::string MethodAdapter::readMethodParam(const std::string& dbName) {
    return m_impl->m_coreMethod.readMethodParam(dbName);
}

std::vector<std::string> MethodAdapter::getAllTables(const std::string& dbName) {
    return m_impl->m_coreMethod.getAllTables(dbName);
}

void MethodAdapter::writeMethodManageRecord(const std::map<std::string, std::string>& info) {
    m_impl->m_coreMethod.writeMethodManageRecord(info);
}

void MethodAdapter::writeMethodRecord(const std::string& dbName, bool coverFlag, 
                                       const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_coreMethod.writeMethodRecord(dbName, coverFlag, info);
}
std::string MethodAdapter::getMethodTableName() {
    return m_impl->m_coreMethod.getMethodTableName();
}
void MethodAdapter::deleteRecord(const std::string& key, 
                                 const std::string& value) {
    m_impl->m_coreMethod.deleteRecord(key, value);
}

void MethodAdapter::deleteDB(const std::string& dbName) {
    m_impl->m_coreMethod.deleteDB(dbName);
}
std::string MethodAdapter::getMethodPrefix() {
    return m_impl->m_coreMethod.getMethodPrefix();
}
void MethodAdapter::clearMethodManageRecord() {
    m_impl->m_coreMethod.clearMethodManageRecord();
}

std::map<std::string, std::string> MethodAdapter::getMethodMap(int index, 
                                                                 const std::string& indexStr,
                                                                 const std::string& name,
                                                                 const std::string& type,
                                                                 const std::string& createTime,
                                                                 const std::string& dbName,
                                                                 const std::string& param) {
    return m_impl->m_coreMethod.getMethodMap(index, indexStr, name, type, createTime, dbName, param);
}

std::vector<std::map<std::string, std::string>> MethodAdapter::getMethodParamMap(const std::string& indexStr,
                                                                                   const std::string& name,
                                                                                   const std::string& type,
                                                                                   const std::string& createTime,
                                                                                   const std::string& dbName,
                                                                                   const std::string& param) {
    return m_impl->m_coreMethod.getMethodParamMap(indexStr, name, type, createTime, dbName, param);
}
