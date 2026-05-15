#ifndef COREREAGENT_H
#define COREREAGENT_H

#include "CoreReagent_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class COREREAGENT_EXPORT CoreReagent {
public:
    CoreReagent();
    ~CoreReagent();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> readReagentInfo(const std::string& dbName);
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
                                                          const std::map<int, std::map<std::string, std::string>>& linkDevices);
    std::map<std::string, std::string> getMapFromDevices(const std::map<std::string, std::string>& device);
    
    std::vector<std::string> getAllTables(const std::string& dbName);
    std::vector<std::string> getReagentNoEditName();
    std::vector<std::string> getReagentLinkDeviceNoEditName();
    
    void writeReagentRecord(bool coverFlag, const std::string& dbName, 
                           const std::vector<std::map<std::string, std::string>>& reagentS);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // COREREAGENT_H
