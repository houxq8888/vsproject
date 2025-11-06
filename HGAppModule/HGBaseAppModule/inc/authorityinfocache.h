#ifndef AUTHORITYINFO_CACHE_H
#define AUTHORITYINFO_CACHE_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

// #define ALL_AUTHORITY "SystemSet:ReagentManage:DeviceManage:ChannelSet:RunningStatus:TaskSequence:Flow:Method:AnalysisRecord:WarningInfo:Log:DeviceMaintenance:MaterialManage:DeviceInfo:Date from:ExceptionHandle:ServiceInfo"

class AuthorityInfoCache
{
public:
    void load();
    void save();
    void setWholeAuthority(const std::vector<std::string> &wholeAuthority);
    std::vector<std::string> getWholeAuthority() {return m_whole_authority;};
    std::vector<std::map<std::string,std::string>> getInfo() const ;
    std::vector<std::string> getAuthorityDetail(const std::string &key, bool noPass);
    std::string getField(int index, const std::string& fieldName) const ;
    void setField(int index, const std::string& fieldName, const std::string& value);
    void addFieldValue(int index, const std::string& fieldName, const std::string& value);
    void addRecord(const std::map<std::string,std::string> &info);

private:
    std::vector<std::string> m_whole_authority;
    std::vector<std::map<std::string,std::string>> m_data;
    bool m_dirty = false;
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

#endif