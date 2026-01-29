#ifndef CORECHANNEL_H
#define CORECHANNEL_H

#include "CoreChannel_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

struct ModuleOfChannel {
    int index;
    std::string name;
    std::string param;
    std::string nameOfSameModule;
    int indexOfSameModule;
    std::string typeName;
};

struct Channel {
    int channel;
    std::string dbName;
    std::vector<ModuleOfChannel> modules;
};

class CORECHANNEL_EXPORT CoreChannel {
public:
    CoreChannel();
    ~CoreChannel();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> readChannelInfo();
    std::string readModulesParam(const std::string& dbName);
    
    void deleteRecord(const std::string& tableName, const std::string& key, 
                     const std::string& value);
    void deleteDB(const std::string& dbName);
    void clearChannelManageRecord();
    void deleteAllChannelModuleDB();
    
    void writeChannelManageRecord(const std::map<std::string, std::string>& info);
    void writeModulesRecord(const std::string& dbName, bool coverFlag, 
                           const std::vector<std::map<std::string, std::string>>& info);
    
    std::map<std::string, std::string> getChannelMap(int index,
                                                          int channel,
                                                          const std::string& dbName,
                                                          const std::vector<std::map<std::string, std::string>>& modules);
    std::vector<std::map<std::string, std::string>> getModulesMap(int channel,
                                                                      const std::string& dbName,
                                                                      const std::vector<std::map<std::string, std::string>>& modules);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CORECHANNEL_H
