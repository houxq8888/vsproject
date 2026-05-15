#include "CoreChannel.h"
#include "rwChannelDb.h"
#include "DatabaseManagerAdapter.h"
#include "hgjson.h"

using namespace HGMACHINE;

class CoreChannel::Impl {
public:
    Impl() {
        m_dbManager = &DatabaseManagerAdapter::instance();
    }
    
    ~Impl() {
    }
    
    DatabaseManagerAdapter* m_dbManager;
    RWChannelDb m_rwChannelDb;
};

CoreChannel::CoreChannel() : m_impl(new Impl()) {
}

CoreChannel::~CoreChannel() {
    delete m_impl;
}

bool CoreChannel::initialize() {
    if (m_impl->m_dbManager && !m_impl->m_dbManager->IsConnected()) {
        std::string basePath = RWDb::readCurDirPath();
        RWDb::openDB(basePath);
    }
    return true;
}

void CoreChannel::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreChannel::readChannelInfo() {
    return m_impl->m_rwChannelDb.readChannelInfo();
}

std::string CoreChannel::readModulesParam(const std::string& dbName) {
    std::vector<ModuleOfChannel> modules = m_impl->m_rwChannelDb.readModulesParam(dbName);
    return HGJson::serialize(modules);
}

void CoreChannel::deleteRecord(const std::string& tableName, const std::string& key, 
                               const std::string& value) {
    RWDb::deleteRecord(tableName, key, value);
}

void CoreChannel::deleteDB(const std::string& dbName) {
    RWDb::deleteDB(dbName);
}

void CoreChannel::clearChannelManageRecord() {
    m_impl->m_rwChannelDb.clearChannelManageRecord();
}

void CoreChannel::deleteAllChannelModuleDB() {
    m_impl->m_rwChannelDb.deleteAllChannelModuleDB();
}

void CoreChannel::writeChannelManageRecord(const std::map<std::string, std::string>& info) {
    m_impl->m_rwChannelDb.writeChannelManageRecord(info);
}

void CoreChannel::writeModulesRecord(const std::string& dbName, bool coverFlag, 
                                     const std::vector<std::map<std::string, std::string>>& info) {
    m_impl->m_rwChannelDb.writeModulesRecord(dbName, coverFlag, info);
}

std::map<std::string, std::string> CoreChannel::getChannelMap(int index,
                                                                int channel,
                                                                const std::string& dbName,
                                                                const std::vector<std::map<std::string, std::string>>& modules) {
    Channel channelInfo;
    channelInfo.channel = channel;
    channelInfo.dbName = dbName;
    channelInfo.modules.clear();
    for (const auto& moduleMap : modules) {
        ModuleOfChannel module;
        module.index = std::stoi(moduleMap.at("index"));
        module.name = moduleMap.at("name");
        module.param = moduleMap.at("param");
        module.nameOfSameModule = moduleMap.at("nameOfSameModule");
        module.indexOfSameModule = std::stoi(moduleMap.at("indexOfSameModule"));
        module.typeName = moduleMap.at("typeName");
        channelInfo.modules.push_back(module);
    }
    return m_impl->m_rwChannelDb.getChannelMap(index, channelInfo);
}

std::vector<std::map<std::string, std::string>> CoreChannel::getModulesMap(int channel,
                                                                            const std::string& dbName,
                                                                            const std::vector<std::map<std::string, std::string>>& modules) {
    Channel channelInfo;
    channelInfo.channel = channel;
    channelInfo.dbName = dbName;
    channelInfo.modules.clear();
    for (const auto& moduleMap : modules) {
        ModuleOfChannel module;
        module.index = std::stoi(moduleMap.at("index"));
        module.name = moduleMap.at("name");
        module.param = moduleMap.at("param");
        module.nameOfSameModule = moduleMap.at("nameOfSameModule");
        module.indexOfSameModule = std::stoi(moduleMap.at("indexOfSameModule"));
        module.typeName = moduleMap.at("typeName");
        channelInfo.modules.push_back(module);
    }
    return m_impl->m_rwChannelDb.getModulesMap(channelInfo);
}
