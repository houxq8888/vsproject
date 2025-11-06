#include "systeminfocache.h"
#include "rwDb.h"

using namespace HGMACHINE;

void SystemInfoCache::load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        m_data = {
            {"authority",""},
            {"loginName", ""},
            {"enterUsersManageAuthority", ""},
            {"enterUsersManageName", ""},
            {"免密登录", ""},
            {"debug", ""},
            {"track", ""},
            {"loginNames",""},
            {"userGroupName",""},
            {"用户标识",""},
            {"服务商", ""},
            {"客服专员", ""},
            {"激活码",""},
            {"授权日期", ""},
            {"授权期限", ""},
            {"AuthorityStatus", ""},
            {"设备名称", ""},
            {"设备型号", ""},
            {"设备编号", ""},
            {"当前版本", ""},
            {"出厂日期", ""},
            {"basePath", ""},
            {"lastLoginTime", ""},
            {"自动设置时区", ""},
            {"时区", ""},
            {"自动设置时间", ""},
            {"手动设置日期和时间", ""},
            {"显示器分辨率", ""},
            {"自动调整亮度", ""},
            {"亮度", ""},
            {"显示语言", ""},
            {"声音报警", ""},
            {"声音value", ""},
            {"灯光报警", ""},
            {"WLAN", ""}
          //  {"显示器分辨率", ""}
        };
        RWDb::readSingleInfo(SYSTEMINFODBNAME, m_data);
        m_data["免密登录"]="true";
        m_loaded = true;
    }
}

void SystemInfoCache::save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_dirty) {
        RWDb::recordSingleInfo(SYSTEMINFODBNAME, m_data);
        m_dirty = false;
    }
}

void SystemInfoCache::setValue(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_data[key] != value) {
        m_data[key] = value;
        m_dirty = true;
    }
}
void SystemInfoCache::addValue(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data[key] += value;
    m_dirty = true;
}
const std::string& SystemInfoCache::getValue(const std::string& key) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    static const std::string empty_string="";
    if (m_data.find(key) == m_data.end()) {
        return empty_string;
    } else {
        return m_data.at(key);
    }
}
