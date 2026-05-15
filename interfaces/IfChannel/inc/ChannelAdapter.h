#ifndef CHANNELADAPTER_H
#define CHANNELADAPTER_H

#include "IChannel.h"
#include "IfChannel_global.h"

namespace HGMACHINE {

class IFCHANNEL_EXPORT ChannelAdapter : public IChannel {
public:
    ChannelAdapter();
    virtual ~ChannelAdapter();
    
    bool initialize() override;
    void shutdown() override;
    
    std::vector<std::map<std::string, std::string>> readChannelInfo() override;
    std::vector<std::map<std::string, std::string>> readModulesParam(const std::string& dbName) override;
    
    void deleteRecord(const std::string& tableName, const std::string& key, 
                     const std::string& value) override;
    void deleteDB(const std::string& dbName) override;
    void clearChannelManageRecord() override;
    void deleteAllChannelModuleDB() override;
    
    void writeChannelManageRecord(const std::map<std::string, std::string>& info) override;
    void writeModulesRecord(const std::string& dbName, bool coverFlag, 
                            const std::vector<std::map<std::string, std::string>>& info) override;
    
    std::map<std::string, std::string> getChannelMap(int index,
                                                          int channel,
                                                          const std::string& dbName,
                                                          const std::vector<std::map<std::string, std::string>>& modules) override;
    std::vector<std::map<std::string, std::string>> getModulesMap(int channel,
                                                                      const std::string& dbName,
                                                                      const std::vector<std::map<std::string, std::string>>& modules) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CHANNELADAPTER_H
