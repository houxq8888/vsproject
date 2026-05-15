#ifndef REAGENTADAPTER_H
#define REAGENTADAPTER_H

#include "IReagent.h"
#include "IfReagent_global.h"

namespace HGMACHINE {

class IFREAGENT_EXPORT ReagentAdapter : public IReagent {
public:
    ReagentAdapter();
    virtual ~ReagentAdapter();
    
    bool initialize() override;
    void shutdown() override;
    
    std::vector<std::map<std::string, std::string>> readReagentInfo(const std::string& dbName) override;
    std::map<std::string, std::string> getMapFromReagent(int index,
                                                           const std::string& type,
                                                           const std::string& serialNumber,
                                                           const std::string& name,
                                                           const std::string& concertraction,
                                                           const std::string& calibrationDate,
                                                           const std::string& calibrationCircle,
                                                           const std::string& reagentRemainAmount,
                                                           const std::string& currentState,
                                                           const std::string& dbName,
                                                           const std::map<int, std::map<std::string, std::string>>& linkDevices) override;
    std::map<std::string, std::string> getMapFromDevices(const std::map<std::string, std::string>& device) override;
    
    std::vector<std::string> getAllTables(const std::string& dbName) override;
    std::vector<std::string> getReagentNoEditName() override;
    std::vector<std::string> getReagentLinkDeviceNoEditName() override;
    
    void writeReagentRecord(bool coverFlag, const std::string& dbName, 
                           const std::vector<std::map<std::string, std::string>>& reagentS) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // REAGENTADAPTER_H
