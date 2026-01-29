#include "AnalysisRecordManager.h"
#include "AnalysisRecordAdapter.h"

using namespace HGMACHINE;

AnalysisRecordManager::AnalysisRecordManager() : m_analysisRecord(new AnalysisRecordAdapter()) {
}

AnalysisRecordManager::~AnalysisRecordManager() {
}

AnalysisRecordManager& AnalysisRecordManager::instance() {
    static AnalysisRecordManager instance;
    return instance;
}

IAnalysisRecord& AnalysisRecordManager::get() {
    return *m_analysisRecord;
}
