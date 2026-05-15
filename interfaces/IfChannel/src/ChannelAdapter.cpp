#include "ChannelAdapter.h"
#include "CoreChannel/CoreChannel.h"

using namespace HGMACHINE;

class ChannelAdapter::Impl {
public:
    CoreChannel m_coreChannel;
};

ChannelAdapter::ChannelAdapter() : m_impl(new Impl()) {
}

ChannelAdapter::~ChannelAdapter() {
    delete m_impl;
}

bool ChannelAdapter::initialize() {
    return m_impl->m_coreChannel.initialize();
}

void ChannelAdapter::shutdown() {
    m_impl->m_coreChannel.shutdown();
}

std::vector<std::map<std::string, std::string>> ChannelAdapter::readChannelInfo() {
    return m_impl->m_coreChannel.readChannelInfo();
}

std::vector<std::map<std::string, std::string>> ChannelAdapter::readModulesParam(const std::string& dbName) {
    return m_impl->m_coreChannel.readModulesParam(dbName);
}

void ChannelAdapter::deleteRecord(const std::string& tableName, const std::string& key, 
                                 const std::string& value) {
    m_impl->m_coreChannel.deleteRecord(tableName, key, value);
}

void ChannelAdapter::deleteDB(const std::string& dbName) {
    m_impl->m_coreChannel.deleteDB(dbName);
}

void ChannelAdapter::clearChannelManageRecord() {
    m_impl->m_coreChannel.clearChannelManageRecord();
}

void ChannelAdapter::deleteAllChannelModuleDB() {
    m_impl->m_coreChannel.deleteAllChannelModuleDB();
}

void ChannelAdapter::writeChannelManageRecord(const std::map<std::string, std::string>& info) {
    m_impl->m_coreChannel.writeChannelManageRecord(info);
}

void ChannelAdapter::writeModulesRecord(const std::string& dbName, bool coverFlag, 
                                       const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_coreChannel.writeModulesRecord(dbName, coverFlag, info);
}

std::map<std::string, std::string> ChannelAdapter::getChannelMap(int index,
                                                                    int channel,
                                                                    const std::string& dbName,
                                                                    const std::vector<std::map<std::string, std::string>>& modules) {
    return m_impl->m_coreChannel.getChannelMap(index, channel, dbName, modules);
}

std::vector<std::map<std::string, std::string>> ChannelAdapter::getModulesMap(int channel,
                                                                            const std::string& dbName,
                                                                            const std::vector<std::map<std::string, std::string>>& modules) {
    return m_impl->m_coreChannel.getModulesMap(channel, dbName, modules);
}
