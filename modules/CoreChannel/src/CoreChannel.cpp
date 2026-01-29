#include "CoreChannel.h"
#include "rwDb.h"
#include "hgjson.h"

using namespace HGMACHINE;

class CoreChannel::Impl {
public:
    Impl() {
    }
    
    ~Impl() {
    }
};

CoreChannel::CoreChannel() : m_impl(new Impl()) {
}

CoreChannel::~CoreChannel() {
    delete m_impl;
}

bool CoreChannel::initialize() {
    return true;
}

void CoreChannel::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreChannel::readChannelInfo() {
    return RWDb::readChannelInfo();
}

std::string CoreChannel::readModulesParam(const std::string& dbName) {
    std::vector<ModuleOfChannel> modules = RWDb::readModulesParam(dbName);
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
    RWDb::clearChannelManageRecord();
}

void CoreChannel::deleteAllChannelModuleDB() {
    RWDb::deleteAllChannelModuleDB();
}

void CoreChannel::writeChannelManageRecord(const std::map<std::string, std::string>& info) {
    RWDb::writeChannelManageRecord(info);
}

void CoreChannel::writeModulesRecord(const std::string& dbName, bool coverFlag, 
                                     const std::vector<std::map<std::string, std::string>>& info) {
    RWDb::writeModulesRecord(dbName, coverFlag, info);
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
    return RWDb::getChannelMap(index, channelInfo);
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
    return RWDb::getModulesMap(channelInfo);
}
