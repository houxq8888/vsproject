#include "AnalysisRecordAdapter.h"
#include "CoreAnalysisRecord.h"

using namespace HGMACHINE;

class AnalysisRecordAdapter::Impl {
public:
    CoreAnalysisRecord m_coreAnalysisRecord;
};

AnalysisRecordAdapter::AnalysisRecordAdapter() : m_impl(new Impl()) {
}

AnalysisRecordAdapter::~AnalysisRecordAdapter() {
    delete m_impl;
}

bool AnalysisRecordAdapter::initialize() {
    return m_impl->m_coreAnalysisRecord.initialize();
}

void AnalysisRecordAdapter::shutdown() {
    m_impl->m_coreAnalysisRecord.shutdown();
}

std::string AnalysisRecordAdapter::getTaskRunFlag() {
    return m_impl->m_coreAnalysisRecord.getTaskRunFlag();
}

std::vector<std::map<std::string, std::string>> AnalysisRecordAdapter::getTaskRunInfo() {
    return m_impl->m_coreAnalysisRecord.getTaskRunInfo();
}
