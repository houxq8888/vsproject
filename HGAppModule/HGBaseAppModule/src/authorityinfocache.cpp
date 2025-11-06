#include "authorityinfocache.h"
#include "rwDb.h"
#include "hgcommonutility.h"
#include "hgxml.h"

using namespace HGMACHINE;

void AuthorityInfoCache::load() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded) {
        m_data = RWDb::readUserGroupInfo();
        m_loaded = true;
    }
}

void AuthorityInfoCache::save() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_dirty) {
        for (int i=0;i<int(m_data.size());i++)
            RWDb::writeUserGroupInfo(m_data[i]);
        m_dirty = false;
    }
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
    permissionInfo.erase(permissionInfo.begin()+permissionInfo.size()-1);
    return permissionInfo;
}

std::vector<std::map<std::string, std::string>> AuthorityInfoCache::getInfo() const {
    return m_data;
}
std::string AuthorityInfoCache::getField(int index, const std::string& fieldName) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        auto it = m_data[index].find(fieldName);
        if (it != m_data[index].end())
            return it->second;
    }
    return "";
}

void AuthorityInfoCache::setField(int index, const std::string& fieldName, const std::string& value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        m_data[index][fieldName] = value;
        m_dirty = true;
    }
}
void AuthorityInfoCache::addFieldValue(int index, const std::string& fieldName, const std::string& value){
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= 0 && index < int(m_data.size())) {
        m_data[index][fieldName] += value;
        m_dirty = true;
    }
}
void AuthorityInfoCache::addRecord(const std::map<std::string,std::string> &info){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data.push_back(info);
    m_dirty = true;
}

