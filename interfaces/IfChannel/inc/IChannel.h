#ifndef ICHANNEL_H
#define ICHANNEL_H

#include "IfChannel_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFCHANNEL_EXPORT IChannel {
public:
    virtual ~IChannel() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> readChannelInfo() = 0;
    virtual std::string readModulesParam(const std::string& dbName) = 0;
    
    virtual void deleteRecord(const std::string& tableName, const std::string& key, 
                            const std::string& value) = 0;
    virtual void deleteDB(const std::string& dbName) = 0;
    virtual void clearChannelManageRecord() = 0;
    virtual void deleteAllChannelModuleDB() = 0;
    
    virtual void writeChannelManageRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void writeModulesRecord(const std::string& dbName, bool coverFlag, 
                                  const std::vector<std::map<std::string, std::string>>& info) = 0;
    
    virtual std::map<std::string, std::string> getChannelMap(int index,
                                                              int channel,
                                                              const std::string& dbName,
                                                              const std::vector<std::map<std::string, std::string>>& modules) = 0;
    virtual std::vector<std::map<std::string, std::string>> getModulesMap(int channel,
                                                                          const std::string& dbName,
                                                                          const std::vector<std::map<std::string, std::string>>& modules) = 0;
};

}

#endif // ICHANNEL_H
