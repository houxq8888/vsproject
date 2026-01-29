#include "CoreSystemData.h"
#include "rwDb.h"

using namespace HGMACHINE;

class CoreSystemData::Impl {
public:
    Impl();
    
    std::map<std::string, std::string> m_data;
    bool m_dirty;
    bool m_loaded;
};

CoreSystemData::Impl::Impl()
    : m_dirty(false)
    , m_loaded(false)
{
}

CoreSystemData::CoreSystemData() : m_impl(new Impl()) {
}

CoreSystemData::~CoreSystemData() {
    shutdown();
    delete m_impl;
}

bool CoreSystemData::initialize() {
    return true;
}

void CoreSystemData::shutdown() {
    saveSystemInfo();
}

bool CoreSystemData::loadSystemInfo() {
    if (!m_impl->m_loaded) {
        m_impl->m_data = {
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
            {"WLAN", ""},
            {"CameraType", ""},
            {"CameraName", ""},
            {"CameraTemplateName", ""},
            {"CameraScore", ""},
            {"ROIX1", ""},
            {"ROIY1", ""},
            {"ROIX2", ""},
            {"ROIY2", ""},
        };
        RWDb::readSingleInfo(SYSTEMINFODBNAME, m_impl->m_data);
        m_impl->m_loaded = true;
    }
    return m_impl->m_loaded;
}

bool CoreSystemData::saveSystemInfo() {
    if (m_impl->m_dirty) {
        RWDb::recordSingleInfo(SYSTEMINFODBNAME, m_impl->m_data);
        m_impl->m_dirty = false;
    }
    return !m_impl->m_dirty;
}

std::string CoreSystemData::getSystemInfo(const std::string& key) {
    auto it = m_impl->m_data.find(key);
    if (it != m_impl->m_data.end()) {
        return it->second;
    }
    return "";
}

void CoreSystemData::setSystemInfo(const std::string& key, const std::string& value) {
    m_impl->m_data[key] = value;
    m_impl->m_dirty = true;
}

void CoreSystemData::addSystemInfo(const std::string& key, const std::string& value) {
    m_impl->m_data[key] = value;
    m_impl->m_dirty = true;
}

void CoreSystemData::delSystemInfo(const std::string& key, const std::string& value) {
    auto it = m_impl->m_data.find(key);
    if (it != m_impl->m_data.end()) {
        m_impl->m_data.erase(it);
        m_impl->m_dirty = true;
    }
}

std::map<std::string, std::string> CoreSystemData::getAllSystemInfo() {
    return m_impl->m_data;
}

void CoreSystemData::setAllSystemInfo(const std::map<std::string, std::string>& info) {
    m_impl->m_data = info;
    m_impl->m_dirty = true;
}

