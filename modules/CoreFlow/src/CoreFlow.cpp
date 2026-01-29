#include "CoreFlow.h"
#include "rwDb.h"
#include "rwMethodDb.h"
#include "hgjson.h"
#include "json.hpp"

using json = nlohmann::json;

using namespace HGMACHINE;

void to_json(json& j, const StepOfFlow& p) {
    j = json{
        {"index", p.index},
        {"name", p.name},
        {"param", p.param},
        {"nameOfSameStep", p.nameOfSameStep},
        {"indexOfSameStep", p.indexOfSameStep}
    };
}

void from_json(const json& j, StepOfFlow& p) {
    j.at("index").get_to(p.index);
    j.at("name").get_to(p.name);
    j.at("param").get_to(p.param);
    j.at("nameOfSameStep").get_to(p.nameOfSameStep);
    j.at("indexOfSameStep").get_to(p.indexOfSameStep);
}

void to_json(json& j, const FlowOfTask& p) {
    j = json{
        {"indexStr", p.indexStr},
        {"name", p.name},
        {"type", p.type},
        {"createTime", p.createTime},
        {"dbName", p.dbName},
        {"steps", p.steps}
    };
}

void from_json(const json& j, FlowOfTask& p) {
    j.at("indexStr").get_to(p.indexStr);
    j.at("name").get_to(p.name);
    j.at("type").get_to(p.type);
    j.at("createTime").get_to(p.createTime);
    j.at("dbName").get_to(p.dbName);
    j.at("steps").get_to(p.steps);
}

class CoreFlow::Impl {
public:
    Impl() {
    }
    
    ~Impl() {
    }
};

CoreFlow::CoreFlow() : m_impl(new Impl()) {
}

CoreFlow::~CoreFlow() {
    delete m_impl;
}

bool CoreFlow::initialize() {
    return true;
}

void CoreFlow::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreFlow::readFlowInfo() {
    return RWDb::readFlowInfo();
}

std::string CoreFlow::readFlowOfTask(const std::string& dbName) {
    FlowOfTask flow = RWDb::readFlowOfTask(dbName);
    return HGJson::serialize(flow);
}

std::vector<std::string> CoreFlow::getAllTables(const std::string& dbName) {
    return RWDb::getAllTables(dbName);
}

std::vector<std::string> CoreFlow::getMethodNames() {
    return RWMethodDb::getMethodNames();
}

void CoreFlow::writeFlowManageRecord(const std::map<std::string, std::string>& info) {
    RWDb::writeFlowManageRecord(info);
}

void CoreFlow::writeFlowRecord(const std::string& dbName, bool coverFlag, 
                               const std::vector<std::map<std::string, std::string>>& info) {
    RWDb::writeFlowRecord(dbName, coverFlag, info);
}

void CoreFlow::deleteRecord(const std::string& tableName, const std::string& key, 
                           const std::string& value) {
    RWDb::deleteRecord(tableName, key, value);
}

void CoreFlow::deleteDB(const std::string& dbName) {
    RWDb::deleteDB(dbName);
}

void CoreFlow::clearFlowManageRecord() {
    RWDb::clearFlowManageRecord();
}

std::map<std::string, std::string> CoreFlow::getFlowMap(int index, 
                                                          const std::string& indexStr,
                                                          const std::string& name,
                                                          const std::string& type,
                                                          const std::string& createTime,
                                                          const std::string& dbName) {
    FlowOfTask flowoftask;
    flowoftask.indexStr = indexStr;
    flowoftask.name = name;
    flowoftask.type = type;
    flowoftask.createTime = createTime;
    flowoftask.dbName = dbName;
    return RWDb::getFlowMap(index, flowoftask);
}

std::vector<std::map<std::string, std::string>> CoreFlow::getFlowStepMap(const std::string& indexStr,
                                                                           const std::string& name,
                                                                           const std::string& type,
                                                                           const std::string& createTime,
                                                                           const std::string& dbName,
                                                                           const std::vector<std::map<std::string, std::string>>& steps) {
    FlowOfTask flowoftask;
    flowoftask.indexStr = indexStr;
    flowoftask.name = name;
    flowoftask.type = type;
    flowoftask.createTime = createTime;
    flowoftask.dbName = dbName;
    flowoftask.steps.clear();
    for (const auto& step : steps) {
        StepOfFlow stepOfFlow;
        stepOfFlow.index = std::stoi(step.at("index"));
        stepOfFlow.name = step.at("name");
        stepOfFlow.param = step.at("param");
        stepOfFlow.nameOfSameStep = step.at("nameOfSameStep");
        stepOfFlow.indexOfSameStep = std::stoi(step.at("indexOfSameStep"));
        flowoftask.steps.push_back(stepOfFlow);
    }
    return RWDb::getFlowStepMap(flowoftask);
}
