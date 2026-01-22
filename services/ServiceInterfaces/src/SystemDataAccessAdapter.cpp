#include "SystemDataAccessAdapter.h"
#include "systeminfocache.h"

class SystemDataAccessAdapter::Impl {
public:
    SystemInfoCache cache;
};

SystemDataAccessAdapter::SystemDataAccessAdapter() : m_impl(new Impl()) {
}

SystemDataAccessAdapter::~SystemDataAccessAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void SystemDataAccessAdapter::Load() {
    m_impl->cache.load();
}

void SystemDataAccessAdapter::Save() {
    m_impl->cache.save();
}

std::string SystemDataAccessAdapter::GetInfo(const std::string& key) {
    return m_impl->cache.getInfo(key);
}

void SystemDataAccessAdapter::SetInfo(const std::string& key, const std::string& value) {
    m_impl->cache.setInfo(key, value);
}

void SystemDataAccessAdapter::AddInfo(const std::string& key, const std::string& value) {
    m_impl->cache.addInfo(key, value);
}

void SystemDataAccessAdapter::DelInfo(const std::string& key, const std::string& value) {
    m_impl->cache.delInfo(key, value);
}
