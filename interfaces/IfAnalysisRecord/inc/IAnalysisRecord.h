#ifndef IANALYSISRECORD_H
#define IANALYSISRECORD_H

#include "IfAnalysisRecord_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFANALYSISRECORD_EXPORT IAnalysisRecord {
public:
    virtual ~IAnalysisRecord() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::string getTaskRunFlag() = 0;
    virtual std::vector<std::map<std::string, std::string>> getTaskRunInfo() = 0;
};

}

#endif // IANALYSISRECORD_H
