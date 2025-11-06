#ifndef HGAUTHORITYSERVICE_H
#define HGAUTHORITYSERVICE_H


#include <vector>
#include <string>
#include <map>

using namespace std;

#define HGAuthoritySeviceNAME "[HGAuthoritySevice]"

namespace HGMACHINE
{
enum
{
    SYSTEM_MANAGER,
    SOFTWARE_MANAGER,
    MANIPULATOR,
    MAINTENANCE_MANAGER,
    SELF_DEFINE
};


bool isPermitted(const std::map<std::string,std::string> &authorityInfo,
    const std::string &userNo,
    const std::string& moduleName);
}

#endif // HGAUTHORITYSERVICE_H