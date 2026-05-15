#include "CoreReagent.h"
#include "rwReagentDb.h"
#include "DatabaseManagerAdapter.h"

using namespace HGMACHINE;

class CoreReagent::Impl {
public:
    Impl() {
        m_dbManager = &DatabaseManagerAdapter::instance();
    }
    
    ~Impl() {
    }
    
    DatabaseManagerAdapter* m_dbManager;
    RWReagentDb m_rwReagentDb;
};

CoreReagent::CoreReagent() : m_impl(new Impl()) {
}

CoreReagent::~CoreReagent() {
    delete m_impl;
}

bool CoreReagent::initialize() {
    if (m_impl->m_dbManager && !m_impl->m_dbManager->IsConnected()) {
        std::string basePath = RWDb::readCurDirPath();
        RWDb::openDB(basePath);
    }
    return true;
}

void CoreReagent::shutdown() {
}

std::vector<std::map<std::string, std::string>> CoreReagent::readReagentInfo(const std::string& dbName) {
    return m_impl->m_rwReagentDb.readReagentInfo(dbName);
}

std::map<std::string, std::string> CoreReagent::getMapFromReagent(int index,
                                                                    const std::string& type,
                                                                    const std::string& serialNumber,
                                                                    const std::string& name,
                                                                    const std::string& concertraction,
                                                                    const std::string& calibrationDate,
                                                                    const std::string& calibrationCircle,
                                                                    const std::string& reagentRemainAmount,
                                                                    const std::string& currentState,
                                                                    const std::string& dbName,
                                                                    const std::map<int, std::map<std::string, std::string>>& linkDevices) {
    REAGENT reagent;
    reagent.index = index;
    reagent.type = type;
    reagent.serialNumber = serialNumber;
    reagent.name = name;
    reagent.concertraction = concertraction;
    reagent.calibrationDate = calibrationDate;
    reagent.calibrationCircle = calibrationCircle;
    reagent.reagentRemainAmount = reagentRemainAmount;
    reagent.currentState = currentState;
    reagent.dbName = dbName;
    reagent.linkDevices.clear();
    for (const auto& devicePair : linkDevices) {
        ReagentLinkDevice linkDevice;
        linkDevice.index = std::stoi(devicePair.second.at("index"));
        linkDevice.name = devicePair.second.at("name");
        linkDevice.interfaceName = devicePair.second.at("interfaceName");
        linkDevice.linkState = devicePair.second.at("linkState");
        linkDevice.channel = devicePair.second.at("channel");
        linkDevice.choice = devicePair.second.at("choice");
        reagent.linkDevices[devicePair.first] = linkDevice;
    }
    return m_impl->m_rwReagentDb.getMapFromReagent(reagent);
}

std::map<std::string, std::string> CoreReagent::getMapFromDevices(const std::map<std::string, std::string>& device) {
    ReagentLinkDevice linkDevice;
    linkDevice.index = std::stoi(device.at("index"));
    linkDevice.name = device.at("name");
    linkDevice.interfaceName = device.at("interfaceName");
    linkDevice.linkState = device.at("linkState");
    linkDevice.channel = device.at("channel");
    linkDevice.choice = device.at("choice");
    return m_impl->m_rwReagentDb.getMapFromDevices(linkDevice);
}

std::vector<std::string> CoreReagent::getAllTables(const std::string& dbName) {
    return RWDb::getAllTables(dbName);
}

std::vector<std::string> CoreReagent::getReagentNoEditName() {
    return m_impl->m_rwReagentDb.getReagentNoEditName();
}

std::vector<std::string> CoreReagent::getReagentLinkDeviceNoEditName() {
    return m_impl->m_rwReagentDb.getReagentLinkDeviceNoEditName();
}

void CoreReagent::writeReagentRecord(bool coverFlag, const std::string& dbName, 
                                     const std::vector<std::map<std::string, std::string>>& reagentS) {
    m_impl->m_rwReagentDb.writeReagentRecord(coverFlag, dbName, reagentS);
}
