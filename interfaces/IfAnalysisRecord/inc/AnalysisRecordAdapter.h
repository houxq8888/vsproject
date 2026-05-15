#ifndef ANALYSISRECORDADAPTER_H
#define ANALYSISRECORDADAPTER_H

#include "IAnalysisRecord.h"
#include "IfAnalysisRecord_global.h"

namespace HGMACHINE {

class IFANALYSISRECORD_EXPORT AnalysisRecordAdapter : public IAnalysisRecord {
public:
    AnalysisRecordAdapter();
    virtual ~AnalysisRecordAdapter();
    
    bool initialize() override;
    void shutdown() override;
    
    std::string getTaskRunFlag() override;
    std::vector<std::map<std::string, std::string>> getTaskRunInfo() override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // ANALYSISRECORDADAPTER_H
