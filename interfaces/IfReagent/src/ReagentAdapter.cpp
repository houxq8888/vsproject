#include "ReagentAdapter.h"
#include "CoreReagent/CoreReagent.h"

using namespace HGMACHINE;

class ReagentAdapter::Impl {
public:
    CoreReagent m_coreReagent;
};

ReagentAdapter::ReagentAdapter() : m_impl(new Impl()) {
}

ReagentAdapter::~ReagentAdapter() {
    delete m_impl;
}

bool ReagentAdapter::initialize() {
    return m_impl->m_coreReagent.initialize();
}

void ReagentAdapter::shutdown() {
    m_impl->m_coreReagent.shutdown();
}

std::vector<std::map<std::string, std::string>> ReagentAdapter::readReagentInfo(const std::string& dbName) {
    return m_impl->m_coreReagent.readReagentInfo(dbName);
}

std::map<std::string, std::string> ReagentAdapter::getMapFromReagent(int index,
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
    return m_impl->m_coreReagent.getMapFromReagent(index, type, serialNumber, name, concertraction,
                                                     calibrationDate, calibrationCircle, reagentRemainAmount,
                                                     currentState, dbName, linkDevices);
}

std::map<std::string, std::string> ReagentAdapter::getMapFromDevices(const std::map<std::string, std::string>& device) {
    return m_impl->m_coreReagent.getMapFromDevices(device);
}

std::vector<std::string> ReagentAdapter::getAllTables(const std::string& dbName) {
    return m_impl->m_coreReagent.getAllTables(dbName);
}

std::vector<std::string> ReagentAdapter::getReagentNoEditName() {
    return m_impl->m_coreReagent.getReagentNoEditName();
}

std::vector<std::string> ReagentAdapter::getReagentLinkDeviceNoEditName() {
    return m_impl->m_coreReagent.getReagentLinkDeviceNoEditName();
}

void ReagentAdapter::writeReagentRecord(bool coverFlag, const std::string& dbName, 
                                       const std::vector<std::map<std::string, std::string>>& reagentS) {
    m_impl->m_coreReagent.writeReagentRecord(coverFlag, dbName, reagentS);
}
