#ifndef COREANALYSISRECORD_H
#define COREANALYSISRECORD_H

#include "CoreAnalysisRecord_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class COREANALYSISRECORD_EXPORT CoreAnalysisRecord {
public:
    CoreAnalysisRecord();
    ~CoreAnalysisRecord();
    
    bool initialize();
    void shutdown();
    
    std::string getTaskRunFlag();
    std::vector<std::map<std::string, std::string>> getTaskRunInfo();
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // COREANALYSISRECORD_H
