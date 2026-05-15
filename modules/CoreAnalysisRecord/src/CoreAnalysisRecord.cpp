#include "CoreAnalysisRecord.h"
#include "rwDb.h"

using namespace HGMACHINE;

class CoreAnalysisRecord::Impl {
public:
    Impl() {
    }
    
    ~Impl() {
    }
};

CoreAnalysisRecord::CoreAnalysisRecord() : m_impl(new Impl()) {
}

CoreAnalysisRecord::~CoreAnalysisRecord() {
    delete m_impl;
}

bool CoreAnalysisRecord::initialize() {
    return true;
}

void CoreAnalysisRecord::shutdown() {
}

std::string CoreAnalysisRecord::getTaskRunFlag() {
    return ""; //RWDb::getTaskRunFlag();
}

std::vector<std::map<std::string, std::string>> CoreAnalysisRecord::getTaskRunInfo() {
    return std::vector<std::map<std::string,std::string>>{}; //RWDb::getTaskRunInfo();
}
