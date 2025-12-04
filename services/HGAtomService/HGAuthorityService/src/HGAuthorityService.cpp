#include "HGAuthorityService.h"
#include <algorithm>

namespace HGMACHINE{

bool isPermitted(const std::map<std::string,std::string> &authorityInfo,
    const std::string &userNo,
    const std::string& moduleName)
{
    if (userNo=="") return true;
    for (auto &authority : authorityInfo){
        std::string key=authority.first;
        std::string value=authority.second;
        std::vector<std::string> resultKey, resultValue;
        size_t start=0,end;
        while ((end=key.find('_',start)) != std::string::npos){
            resultKey.push_back(key.substr(start,end-start));
            start=end+1;
        }
        resultKey.push_back(key.substr(start));

        start=0;
        while ((end=value.find(':',start))!=std::string::npos){
            resultValue.push_back(value.substr(start,end-start));
            start=end+1;
        }
        resultValue.push_back(value.substr(start));

        if (resultKey.size() < 2)
            continue;
        if (resultKey[1]=="Account"){
            if (std::find(resultValue.begin(),resultValue.end(),userNo)!=resultValue.end()){
                std::string targetAuthority=resultKey[0]+"_Authority";
                start=0;
                std::vector<std::string> authorityValue;
                while ((end=authorityInfo.at(targetAuthority).find(':',start))!=std::string::npos){
                    authorityValue.push_back(authorityInfo.at(targetAuthority).substr(start,end-start));
                    start=end+1;
                }
                authorityValue.push_back(authorityInfo.at(targetAuthority).substr(start));
                if (std::find(authorityValue.begin(),authorityValue.end(),moduleName)!=authorityValue.end()){
                    return true;
                } else 
                    return false;
            }
        } else continue;
    }
    return false;
}
bool isPermitted(const int& loginAuthority, 
    const std::string& manipulateType)
{
    if (manipulateType=="试剂管理"){
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER||
            loginAuthority==MANIPULATOR)) {
            return false;
        }
    } else if (manipulateType=="设备信息"){
        if (loginAuthority!=SYSTEM_MANAGER) {
            return false;
        }
    } else if (manipulateType=="通道配置"){
        if (!(loginAuthority==SYSTEM_MANAGER)) {
            return false;
        }
    } else if (manipulateType=="方法"){
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER||
            loginAuthority==MANIPULATOR)){
            return false;
        }
    } else if (manipulateType=="分析记录"){
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER)) {
            return false;
        }
    } else if (manipulateType=="流程"){
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER||
            loginAuthority==MANIPULATOR)) {
            return false;
        }
    } else if (manipulateType == "任务序列") {
        if (!(loginAuthority == SYSTEM_MANAGER ||
              loginAuthority == SOFTWARE_MANAGER ||
              loginAuthority == MANIPULATOR)) {
            return false;
        }
    } else if (manipulateType == "运行状态") {
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER||
            loginAuthority==MANIPULATOR)) {
            return false;
        }
    } else if (manipulateType == "日志") {
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER)) {
            return false;
        }
    } else if (manipulateType == "服务信息"){
        if (!(loginAuthority==SYSTEM_MANAGER||
            loginAuthority==SOFTWARE_MANAGER)) {
            return false;
        }
    }
    return true;
}
}