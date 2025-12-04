#ifndef PACKFUNINTERFACE_H
#define PACKFUNINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE{

    // database
void openDB(const std::string&dbName);
void closeDB();
void changeUserLoginAndQuitTime(const std::string &userNo, const std::string &key);

// config
bool loadConfig(std::string curPath);
void saveConfig();
void setAppVersion(const std::string &version);
std::string getAppVersion();
}

#ifdef __cplusplus
}
#endif
#endif // PACKFUNINTERFACE_H