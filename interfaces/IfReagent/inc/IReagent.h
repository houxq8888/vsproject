#ifndef IREAGENT_H
#define IREAGENT_H

#include "IfReagent_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFREAGENT_EXPORT IReagent {
public:
    virtual ~IReagent() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::map<std::string, std::string>> readReagentInfo(const std::string& dbName) = 0;
    virtual std::map<std::string, std::string> getMapFromReagent(int index,
                                                                  const std::string& type,
                                                                  const std::string& serialNumber,
                                                                  const std::string& name,
                                                                  const std::string& concertraction,
                                                                  const std::string& calibrationDate,
                                                                  const std::string& calibrationCircle,
                                                                  const std::string& reagentRemainAmount,
                                                                  const std::string& currentState,
                                                                  const std::string& dbName,
                                                                  const std::map<int, std::map<std::string, std::string>>& linkDevices) = 0;
    virtual std::map<std::string, std::string> getMapFromDevices(const std::map<std::string, std::string>& device) = 0;
    
    virtual std::vector<std::string> getAllTables(const std::string& dbName) = 0;
    virtual std::vector<std::string> getReagentNoEditName() = 0;
    virtual std::vector<std::string> getReagentLinkDeviceNoEditName() = 0;
    
    virtual void writeReagentRecord(bool coverFlag, const std::string& dbName, 
                                   const std::vector<std::map<std::string, std::string>>& reagentS) = 0;
};

}

#endif // IREAGENT_H
