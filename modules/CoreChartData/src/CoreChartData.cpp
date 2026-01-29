#include "CoreChartData.h"
#include "rwDb.h"

namespace HGMACHINE {

class CoreChartData::Impl {
public:
    Impl();
    
    std::map<std::string, std::string> m_data;
    bool m_dirty;
    bool m_loaded;
    std::vector<std::string> m_recordNames;
    std::vector<std::string> m_displayNames;
    std::vector<std::string> m_recordInTimeNames;
};

CoreChartData::Impl::Impl()
    : m_dirty(false)
    , m_loaded(false)
{
    m_recordNames = {"流程","方法","检测器","滴定剂","反应液","溶剂","分析时长","运行状态"};
    m_displayNames = {"时间~信号值","时间~信号微分值","体积~信号值","时间~体积","时间~信号二次微分","时间~电压","水质量~电压","时间~水质量","时间~电解速率"};
    m_recordInTimeNames = {"测定实时","趋势实时"};
}

CoreChartData::CoreChartData() : m_impl(new Impl()) {
}

CoreChartData::~CoreChartData() {
    shutdown();
    delete m_impl;
}

bool CoreChartData::initialize() {
    return true;
}

void CoreChartData::shutdown() {
    save();
}

void CoreChartData::load() {
    if (!m_impl->m_loaded) {
        for (int i = 0; i < int(m_impl->m_recordNames.size()); i++) {
            m_impl->m_data[m_impl->m_recordNames[i]] = "";
        }
        for (int i = 0; i < int(m_impl->m_recordInTimeNames.size()); i++) {
            m_impl->m_data[m_impl->m_recordInTimeNames[i]] = "";
        }
        for (int i = 0; i < int(m_impl->m_displayNames.size()); i++) {
            m_impl->m_data[m_impl->m_displayNames[i]] = "";
        }
        RWDb::readSingleInfo(DATACHARTMANAGENAME, m_impl->m_data);
        m_impl->m_loaded = true;
    }
}

void CoreChartData::save() {
    if (m_impl->m_dirty) {
        RWDb::recordSingleInfo(DATACHARTMANAGENAME, m_impl->m_data);
        m_impl->m_dirty = false;
    }
}

void CoreChartData::setValue(const std::string& key, const std::string& value) {
    if (m_impl->m_data[key] != value) {
        m_impl->m_data[key] = value;
        m_impl->m_dirty = true;
    }
}

void CoreChartData::addValue(const std::string& key, const std::string& value) {
    m_impl->m_data[key] += value;
    m_impl->m_dirty = true;
}

std::string CoreChartData::getValue(const std::string& key) const {
    auto it = m_impl->m_data.find(key);
    if (it != m_impl->m_data.end()) {
        return it->second;
    }
    return "";
}

std::vector<std::string> CoreChartData::getRecordNames() const {
    return m_impl->m_recordNames;
}

std::vector<std::string> CoreChartData::getDisplayNames() const {
    return m_impl->m_displayNames;
}

std::vector<std::string> CoreChartData::getRecordInTimeNames() const {
    return m_impl->m_recordInTimeNames;
}

void CoreChartData::setRecordNames(const std::vector<std::string>& names) {
    m_impl->m_recordNames = names;
}

void CoreChartData::setDisplayNames(const std::vector<std::string>& names) {
    m_impl->m_displayNames = names;
}

void CoreChartData::setRecordInTimeNames(const std::vector<std::string>& names) {
    m_impl->m_recordInTimeNames = names;
}

}
