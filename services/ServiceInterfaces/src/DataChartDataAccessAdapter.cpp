#include "DataChartDataAccessAdapter.h"
#include "datachartinfocache.h"

class DataChartDataAccessAdapter::Impl {
public:
    DataChartInfoCache cache;
};

DataChartDataAccessAdapter::DataChartDataAccessAdapter() : m_impl(new Impl()) {
}

DataChartDataAccessAdapter::~DataChartDataAccessAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void DataChartDataAccessAdapter::Load() {
    m_impl->cache.load();
}

void DataChartDataAccessAdapter::Save() {
    m_impl->cache.save();
}

void DataChartDataAccessAdapter::SetValue(const std::string& key, const std::string& value) {
    m_impl->cache.setValue(key, value);
}

void DataChartDataAccessAdapter::AddValue(const std::string& key, const std::string& value) {
    m_impl->cache.addValue(key, value);
}

const std::string& DataChartDataAccessAdapter::GetValue(const std::string& key) const {
    return m_impl->cache.getValue(key);
}
