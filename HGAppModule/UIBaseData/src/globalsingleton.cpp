#include "globalsingleton.h"
#include "rwDb.h"
#include "hgxml.h"
#include "hgcommonutility.h"

using namespace HGMACHINE;

GlobalSingleton::GlobalSingleton()
{
}

GlobalSingleton& GlobalSingleton::instance()
{
    static GlobalSingleton instance;
    return instance;
}
void GlobalSingleton::saveAll(){
    saveSystemInfo();
    saveUsersInfo();
    saveUserGroupInfo();
    // savePrintInfo();
    saveDataChartInfo();
}

void GlobalSingleton::loadSystemInfo(){
    m_systemInfo.load();
}
void GlobalSingleton::saveSystemInfo(){
    m_systemInfo.save();
}
std::string GlobalSingleton::getSystemInfo(const std::string &key){
    m_systemInfo.load();
    std::string temp=m_systemInfo.getValue(key);
    if (key=="authority"){
        std::vector<std::string> authorities = splitStr(temp,';');
        temp="";
        for(int i=0;i<authorities.size();i++){
            if (authorities[i]=="") continue;
            temp+=findTranslationKey("zh",authorities[i])+';';
        }
    }
    return temp;
}
void GlobalSingleton::setSystemInfo(const std::string &key, const std::string &value){
    m_systemInfo.setValue(key,value);
}
void GlobalSingleton::addSystemInfo(const std::string &key, const std::string &value){
    m_systemInfo.addValue(key,value);
}
void GlobalSingleton::delSystemInfo(const std::string &key, const std::string &value){
    m_systemInfo.delValue(key,value);
}

void GlobalSingleton::loadDataChartInfo(){
    m_dataChartInfo.load();
}
void GlobalSingleton::saveDataChartInfo(){
    m_dataChartInfo.save();
}
std::string GlobalSingleton::getDataChartInfo(const std::string &key){
    m_dataChartInfo.load();
    return m_dataChartInfo.getValue(key);
}
void GlobalSingleton::setDataChartInfo(const std::string &key, const std::string &value){
    m_dataChartInfo.setValue(key,value);
}
void GlobalSingleton::addDataChartInfo(const std::string &key, const std::string &value){
    m_dataChartInfo.addValue(key,value);
}


std::vector<std::string> GlobalSingleton::getAuthorityDetail(const std::string &authority){
    return m_authorityInfo.getAuthorityDetail(authority,false);
}
std::vector<std::string> GlobalSingleton::getAuthorityDetail(){
    return m_authorityInfo.getAuthorityDetail(getSystemInfo("authority"),
                                              (getSystemInfo("免密登录")=="true")?true:false);
}
void GlobalSingleton::loadUserGroupInfo(){
    m_authorityInfo.load();
}
void GlobalSingleton::saveUserGroupInfo(){
    m_authorityInfo.save();
}
std::vector<std::map<std::string,std::string>> GlobalSingleton::getAuthorityInfo(){
    return m_authorityInfo.getInfo();
}
std::string GlobalSingleton::getAuthorityField(int index, const std::string& fieldName) const {
    return m_authorityInfo.getField(index,fieldName);
}
void GlobalSingleton::setAuthorityField(int index, const std::string& fieldName, const std::string& value){
    m_authorityInfo.setField(index,fieldName,value);
}
void GlobalSingleton::addAuthorityField(int index, const std::string &fieldName, const std::string &value){
    m_authorityInfo.addFieldValue(index, fieldName,value);
}
void GlobalSingleton::addAuthorityRecord(const std::map<std::string,std::string> &info){
    m_authorityInfo.addRecord(info);
}
void GlobalSingleton::delAuthorityRecord(const std::string& value){
    m_authorityInfo.delRecord(value);
}
std::vector<std::string> GlobalSingleton::getWholeAuthority(){
    return m_authorityInfo.getWholeAuthority();
}
void GlobalSingleton::setWholeAuthority(const std::vector<std::string> &authority){
    m_authorityInfo.setWholeAuthority(authority);
}

// 新增临时数据管理方法实现
bool GlobalSingleton::beginAuthorityEdit(){
    return m_authorityInfo.beginEdit();
}

bool GlobalSingleton::commitAuthorityEdit(){
    return m_authorityInfo.commitEdit();
}

bool GlobalSingleton::rollbackAuthorityEdit(){
    return m_authorityInfo.rollbackEdit();
}

bool GlobalSingleton::isAuthorityEditing(){
    return m_authorityInfo.isEditing();
}
void GlobalSingleton::loadUsersInfo(){
    m_usersInfo.load();
}
void GlobalSingleton::saveUsersInfo(){
    m_usersInfo.save();
}
std::vector<std::map<std::string,std::string>> GlobalSingleton::getUsersInfo(){
    return m_usersInfo.getInfo();
}
std::map<std::string,std::string> GlobalSingleton::getUserInfo(int index){
    return m_usersInfo.getUserInfo(index);
}
void GlobalSingleton::deleteUserInfo(const std::string &userAccount){
    m_usersInfo.deleteUserInfo(userAccount);
}
std::string GlobalSingleton::getUserAuthority(const std::string& key){
    return m_usersInfo.getAuthority(key);
}
std::vector<std::string> GlobalSingleton::getUsersNo(){
    return m_usersInfo.getUsersNo();
}
void GlobalSingleton::setUserField(int index, const std::string& fieldName,const std::string &value){
    m_usersInfo.setField(index,fieldName,value);
}
void GlobalSingleton::addUserField(int index, const std::string& fieldName,const std::string &value){
    m_usersInfo.addFieldValue(index,fieldName,value);
}
int GlobalSingleton::getUserFieldIndex(const std::string &userAccount){
    return m_usersInfo.getUserFieldIndex(userAccount);
}
void GlobalSingleton::setUserInfoWithIndex(int index, const std::map<std::string,std::string> &info){
    m_usersInfo.setUserInfoWithIndex(index,info);
}
std::string GlobalSingleton::getUserField(int index, const std::string& fieldName) const {
    return m_usersInfo.getField(index,fieldName);
}
void GlobalSingleton::addUserRecord(const std::map<std::string,std::string> &info){
    m_usersInfo.addRecord(info);
}

void GlobalSingleton::addAuthorityFieldToEdit(const std::string& authorityName, const std::string& fieldValue) {
    // 在编辑模式下，将权限字段添加到临时编辑缓冲区
    if (isAuthorityEditing()) {
        // 查找对应的权限组
        auto authorityInfo = getAuthorityInfo();
        for (int i = 0; i < authorityInfo.size(); i++) {
            if (authorityInfo[i].at("GroupName") == authorityName) {
                // 直接添加到临时编辑缓冲区
                m_authorityInfo.addFieldValueToEdit(i, "Authority", fieldValue);
                break;
            }
        }
    }
}

void GlobalSingleton::removeAuthorityFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) {
    // 在编辑模式下，从临时编辑缓冲区移除权限字段
    if (isAuthorityEditing()) {
        // 查找对应的权限组
        auto authorityInfo = getAuthorityInfo();
        for (int i = 0; i < authorityInfo.size(); i++) {
            if (authorityInfo[i].at("GroupName") == authorityName) {
                // 获取临时数据中的当前权限列表
                std::string currentAuthority = m_authorityInfo.getField(i, "Authority");
                std::vector<std::string> authorities = splitStr(currentAuthority, ':');
                
                // 从权限列表中移除指定的权限
                std::vector<std::string> newAuthorities;
                for (const auto& auth : authorities) {
                    if (auth != fieldValue && !auth.empty()) {
                        newAuthorities.push_back(auth);
                    }
                }
                
                // 重新构建权限字符串
                std::string newAuthority = "";
                for (const auto& auth : newAuthorities) {
                    if (!newAuthority.empty()) {
                        newAuthority += ":";
                    }
                    newAuthority += auth;
                }
                
                // 更新临时编辑缓冲区
                m_authorityInfo.setFieldToEdit(i, "Authority", newAuthority);
                break;
            }
        }
    }
}

