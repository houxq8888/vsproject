#ifndef AUTHORITYINFO_CACHE_H
#define AUTHORITYINFO_CACHE_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

namespace HGMACHINE {

class AuthorityInfoCache
{
public:
    bool load();
    bool save();
    void setWholeAuthority(const std::vector<std::string> &wholeAuthority);
    std::vector<std::string> getWholeAuthority() {return m_whole_authority;};
    std::vector<std::map<std::string,std::string>> getInfo() const ;
    std::vector<std::string> getAuthorityDetail(const std::string &key, bool noPass);
    std::string getField(int index, const std::string& fieldName) const ;
    void setField(int index, const std::string& fieldName, const std::string& value);
    void addFieldValue(int index, const std::string& fieldName, const std::string& value);
    void addRecord(const std::map<std::string,std::string> &info);
    void delRecord(const std::string &value);
    
    bool beginEdit();
    bool commitEdit();
    bool rollbackEdit();
    bool isEditing();
    
    void addFieldValueToEdit(int index, const std::string& fieldName, const std::string& value);
    void setFieldToEdit(int index, const std::string& fieldName, const std::string& value);
    
    void clear();
    
private:
    std::vector<std::string> m_whole_authority;
    std::vector<std::map<std::string,std::string>> m_data;
    std::vector<std::map<std::string,std::string>> m_tempData;
    bool m_dirty = false;
    bool m_editing = false;
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

}

#endif
