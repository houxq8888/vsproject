#ifndef ANALYSISRECORDMANAGER_H
#define ANALYSISRECORDMANAGER_H

#include "IAnalysisRecord.h"
#include <memory>

namespace HGMACHINE {

class AnalysisRecordManager {
public:
    static AnalysisRecordManager& instance();
    
    IAnalysisRecord& get();
    
private:
    AnalysisRecordManager();
    ~AnalysisRecordManager();
    
    AnalysisRecordManager(const AnalysisRecordManager&) = delete;
    AnalysisRecordManager& operator=(const AnalysisRecordManager&) = delete;
    
    std::unique_ptr<IAnalysisRecord> m_analysisRecord;
};

}

#endif // ANALYSISRECORDMANAGER_H
