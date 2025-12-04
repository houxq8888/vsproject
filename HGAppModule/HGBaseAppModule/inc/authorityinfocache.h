#ifndef AUTHORITYINFO_CACHE_H
#define AUTHORITYINFO_CACHE_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

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
    
    // 新增临时数据管理方法
    bool beginEdit();
    bool commitEdit();
    bool rollbackEdit();
    bool isEditing();
    
    // 新增临时编辑操作方法
    void addFieldValueToEdit(int index, const std::string& fieldName, const std::string& value);
    void setFieldToEdit(int index, const std::string& fieldName, const std::string& value);
    
    // 新增clear方法
    void clear();
    
private:
    std::vector<std::string> m_whole_authority;
    std::vector<std::map<std::string,std::string>> m_data;
    std::vector<std::map<std::string,std::string>> m_tempData; // 临时数据存储
    bool m_dirty = false;
    bool m_editing = false; // 编辑状态标记
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

#endif