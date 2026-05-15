#ifndef HGSECURITY_H
#define HGSECURITY_H

#include "HGSecurityService_global.h"


namespace HGMACHINE {


HGSECURITYSERVICE_EXPORT bool HGEnDecrypt(bool isEncrypt, const std::string &srcString,
                                          std::string &dstString,std::string key);
std::vector<std::string> GetMacs();
std::string GetCPUSerialNumber();
std::string GetHardDiskSerialNumber(const std::string& device="/dev/sda");
// encrypt
bool isRightAuthority(const std::string &in);

}

#endif
