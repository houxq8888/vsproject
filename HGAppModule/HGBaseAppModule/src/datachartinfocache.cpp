#include "datachartinfocache.h"
#include "rwDb.h"

using namespace HGMACHINE;

std::vector<std::string> recordNames={"流程","方法","检测器","滴定剂","反应液","溶剂","分析时长","运行状态"};
std::vector<std::string> displayNames={"时间~信号值","时间~信号微分值","体积~信号值","时间~体积","时间~信号二次微分","时间~电压","水质量~电压","时间~水质量","时间~电解速率"};
std::vector<std::string> recordInTimeNames={"测定实时","趋势实时"};

void DataChartInfoCache::load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        for (int i=0;i<int(recordNames.size());i++){
            m_data[recordNames[i]] = "";
        }
        for (int i=0;i<int(recordInTimeNames.size());i++){
            m_data[recordInTimeNames[i]] = "";
        }
        for (int i=0;i<int(displayNames.size());i++){
            m_data[displayNames[i]] = "";
        }
        RWDb::readSingleInfo(DATACHARTMANAGENAME, m_data);
        m_loaded = true;
    }
}

void DataChartInfoCache::save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_dirty) {
        RWDb::recordSingleInfo(DATACHARTMANAGENAME, m_data);
        m_dirty = false;
    }
}

void DataChartInfoCache::setValue(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data[key] != value) {
        m_data[key] = value;
        m_dirty = true;
    }
}
void DataChartInfoCache::addValue(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data[key] += value;
    m_dirty = true;
}
const std::string& DataChartInfoCache::getValue(const std::string& key) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data.at(key);
}
