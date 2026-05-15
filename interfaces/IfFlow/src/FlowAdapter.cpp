#include "FlowAdapter.h"
#include "CoreFlow/CoreFlow.h"

using namespace HGMACHINE;

class FlowAdapter::Impl {
public:
    CoreFlow m_coreFlow;
};

FlowAdapter::FlowAdapter() : m_impl(new Impl()) {
}

FlowAdapter::~FlowAdapter() {
    delete m_impl;
}

bool FlowAdapter::initialize() {
    return m_impl->m_coreFlow.initialize();
}

void FlowAdapter::shutdown() {
    m_impl->m_coreFlow.shutdown();
}

std::vector<std::map<std::string, std::string>> FlowAdapter::readFlowInfo() {
    return m_impl->m_coreFlow.readFlowInfo();
}

std::string FlowAdapter::readFlowOfTask(const std::string& dbName) {
    return m_impl->m_coreFlow.readFlowOfTask(dbName);
}

std::vector<std::string> FlowAdapter::getAllTables(const std::string& dbName) {
    return m_impl->m_coreFlow.getAllTables(dbName);
}

std::vector<std::string> FlowAdapter::getMethodNames() {
    return m_impl->m_coreFlow.getMethodNames();
}

void FlowAdapter::writeFlowManageRecord(const std::map<std::string, std::string>& info) {
    m_impl->m_coreFlow.writeFlowManageRecord(info);
}

void FlowAdapter::writeFlowRecord(const std::string& dbName, bool coverFlag, 
                                  const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_coreFlow.writeFlowRecord(dbName, coverFlag, info);
}

void FlowAdapter::deleteRecord(const std::string& tableName, const std::string& key, 
                               const std::string& value) {
    m_impl->m_coreFlow.deleteRecord(tableName, key, value);
}

void FlowAdapter::deleteDB(const std::string& dbName) {
    m_impl->m_coreFlow.deleteDB(dbName);
}

void FlowAdapter::clearFlowManageRecord() {
    m_impl->m_coreFlow.clearFlowManageRecord();
}

std::map<std::string, std::string> FlowAdapter::getFlowMap(int index, 
                                                              const std::string& indexStr,
                                                              const std::string& name,
                                                              const std::string& type,
                                                              const std::string& createTime,
                                                              const std::string& dbName) {
    return m_impl->m_coreFlow.getFlowMap(index, indexStr, name, type, createTime, dbName);
}

std::vector<std::map<std::string, std::string>> FlowAdapter::getFlowStepMap(const std::string& indexStr,
                                                                           const std::string& name,
                                                                           const std::string& type,
                                                                           const std::string& createTime,
                                                                           const std::string& dbName,
                                                                           const std::vector<std::map<std::string, std::string>>& steps) {
    return m_impl->m_coreFlow.getFlowStepMap(indexStr, name, type, createTime, dbName, steps);
}
