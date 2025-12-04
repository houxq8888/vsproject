#include "authorityinfocache.h"
#include "rwDb.h"
#include "hgcommonutility.h"
#include "hgxml.h"
#include <mutex>
#include <algorithm>

using namespace HGMACHINE;

bool AuthorityInfoCache::load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_loaded) {
        return true;
    }

    m_data = RWDb::readUserGroupInfo();
    m_loaded = true;
    m_dirty = false;
    return true;
}

bool AuthorityInfoCache::save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_dirty) {
        return true;
    }
    
    // 然后写入新的用户组信息
    bool result = true;
    for (const auto& record : m_data) {
        RWDb::writeUserGroupInfo(record);
    }
    
    if (result) {
        m_dirty = false;
    }
    return result;
}
void AuthorityInfoCache::setWholeAuthority(const std::vector<std::string> &wholeAuthority) {
    m_whole_authority.clear();
    m_whole_authority=wholeAuthority;
}
std::vector<std::string> AuthorityInfoCache::getAuthorityDetail(const std::string &key, bool noPass){
    std::vector<std::string> authorities=splitStr(key,';');
    std::string authorityDetail="";
    if (authorities.size() <= 0)
    {
        for (int i = 0; i < int(m_data.size()); i++)
        {
            if (key == m_data[i]["GroupName"])
            {
                authorityDetail = m_data[i]["Authority"];
                break;
            }
        }
        if (noPass)
        {
            for (auto authority:m_whole_authority)
                authorityDetail += authority+':';
        }
    }
    else
    {
        if (noPass)
        {
            for (auto authority:m_whole_authority)
                authorityDetail += authority+':';
        }
        else
        {
            for (int m = 0; m < int(authorities.size()); m++)
            {
                if (authorities[m] == "")
                    continue;
                std::string authoritiesKey = findTranslationKey("zh",authorities[m]);
                for (int i = 0; i < int(m_data.size()); i++)
                {
                    if (authoritiesKey == m_data[i]["GroupName"])
                    {
                        std::vector<std::string> authoritiesTemp=splitStr(m_data[i]["Authority"],':');
                        for (int n=0;n<int(authoritiesTemp.size());n++){
                            if (authorityDetail.find(authoritiesTemp[n]+":")!=std::string::npos) continue;
                            authorityDetail += authoritiesTemp[n]+':';
                        }
                        break;
                    }
                }
            }
        }
    }

    std::vector<std::string> permissionInfo = splitStr(authorityDetail, ':');
    if (!permissionInfo.empty()) {
        permissionInfo.erase(permissionInfo.begin()+permissionInfo.size()-1);
    }
    return permissionInfo;
}







std::string AuthorityInfoCache::getField(int index, const std::string& fieldName) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // 如果在编辑状态，从临时数据读取
    const std::vector<std::map<std::string,std::string>>& data = m_editing ? m_tempData : m_data;
    
    if (index < 0 || index >= static_cast<int>(data.size())) {
        return "";
    }
    
    auto it = data[index].find(fieldName);
    if (it != data[index].end()) {
        return it->second;
    }
    return "";
}

void AuthorityInfoCache::setField(int index, const std::string& fieldName, const std::string& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        load();
    }
    
    if (index < 0 || index >= static_cast<int>(m_data.size())) {
        return;
    }
    
    m_data[index][fieldName] = value;
    m_dirty = true;
}

void AuthorityInfoCache::addFieldValue(int index, const std::string& fieldName, const std::string& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        load();
    }
    
    if (index < 0 || index >= static_cast<int>(m_data.size())) {
        return;
    }
    
    auto& currentValue = m_data[index][fieldName];
    if (!currentValue.empty()) {
        currentValue += ":";
    }
    currentValue += value;
    m_dirty = true;
}









bool AuthorityInfoCache::commitEdit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_editing) {
        return false; // 不在编辑状态
    }
    
    // 将临时数据提交到主数据
    m_data = m_tempData;
    m_tempData.clear();
    m_editing = false;
    m_dirty = true; // 设置dirty标记，表示需要保存
    
    return true;
}

bool AuthorityInfoCache::rollbackEdit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_editing) {
        return false; // 不在编辑状态
    }
    
    // 丢弃临时数据
    m_tempData.clear();
    m_editing = false;
    
    return true;
}

std::vector<std::map<std::string,std::string>> AuthorityInfoCache::getInfo() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data;
}

void AuthorityInfoCache::addRecord(const std::map<std::string,std::string> &info)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        load();
    }
    
    m_data.push_back(info);
    m_dirty = true;
}

void AuthorityInfoCache::delRecord(const std::string &value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        load();
    }
    
    // 查找并删除包含指定值的记录
    for (auto it = m_data.begin(); it != m_data.end(); ) {
        for (const auto& pair : *it) {
            if (pair.second == value) {
                it = m_data.erase(it);
                m_dirty = true;
                return;
            }
        }
        ++it;
    }
}

bool AuthorityInfoCache::beginEdit()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_editing) {
        return false; // 已经在编辑状态
    }
    
    if (!m_loaded) {
        load();
    }
    
    // 创建临时数据副本
    m_tempData = m_data;
    m_editing = true;
    
    return true;
}

bool AuthorityInfoCache::isEditing()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_editing;
}

void AuthorityInfoCache::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.clear();
    m_tempData.clear();
    m_whole_authority.clear();
    m_dirty = true;
    m_editing = false;
    m_loaded = false;
}

void AuthorityInfoCache::addFieldValueToEdit(int index, const std::string& fieldName, const std::string& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_editing) {
        return; // 不在编辑状态，直接返回
    }
    
    if (index < 0 || index >= static_cast<int>(m_tempData.size())) {
        return;
    }
    
    auto& currentValue = m_tempData[index][fieldName];
    if (!currentValue.empty()) {
        currentValue += ":";
    }
    currentValue += value;
}

void AuthorityInfoCache::setFieldToEdit(int index, const std::string& fieldName, const std::string& value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_editing) {
        return; // 不在编辑状态，直接返回
    }
    
    if (index < 0 || index >= static_cast<int>(m_tempData.size())) {
        return;
    }
    
    m_tempData[index][fieldName] = value;
}


