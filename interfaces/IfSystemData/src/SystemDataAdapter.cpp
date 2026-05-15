#include "SystemDataAdapter.h"
#include "CoreSystemData.h"

namespace HGMACHINE {

class SystemDataAdapter::Impl {
public:
    CoreSystemData coreSystemData;
};

SystemDataAdapter::SystemDataAdapter() : m_impl(new Impl()) {
}

SystemDataAdapter::~SystemDataAdapter() {
    delete m_impl;
}

bool SystemDataAdapter::initialize() {
    return m_impl->coreSystemData.initialize();
}

void SystemDataAdapter::shutdown() {
    m_impl->coreSystemData.shutdown();
}

bool SystemDataAdapter::loadSystemInfo() {
    return m_impl->coreSystemData.loadSystemInfo();
}

bool SystemDataAdapter::saveSystemInfo() {
    return m_impl->coreSystemData.saveSystemInfo();
}

std::string SystemDataAdapter::getSystemInfo(const std::string& key) {
    return m_impl->coreSystemData.getSystemInfo(key);
}

void SystemDataAdapter::setSystemInfo(const std::string& key, const std::string& value) {
    m_impl->coreSystemData.setSystemInfo(key, value);
}

void SystemDataAdapter::addSystemInfo(const std::string& key, const std::string& value) {
    m_impl->coreSystemData.addSystemInfo(key, value);
}

void SystemDataAdapter::delSystemInfo(const std::string& key, const std::string& value) {
    m_impl->coreSystemData.delSystemInfo(key, value);
}

std::map<std::string, std::string> SystemDataAdapter::getAllSystemInfo() {
    return m_impl->coreSystemData.getAllSystemInfo();
}

void SystemDataAdapter::setAllSystemInfo(const std::map<std::string, std::string>& info) {
    m_impl->coreSystemData.setAllSystemInfo(info);
}

}
