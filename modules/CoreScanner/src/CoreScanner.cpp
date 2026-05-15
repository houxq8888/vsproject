#include "CoreScanner.h"
#include "rwDb.h"

namespace HGMACHINE {

class CoreScanner::Impl {
public:
    std::string lastError;
};

CoreScanner::CoreScanner() : m_impl(new Impl()) {
}

CoreScanner::~CoreScanner() {
    delete m_impl;
}

bool CoreScanner::initialize() {
    return true;
}

void CoreScanner::shutdown() {
}

std::map<std::string, std::string> CoreScanner::readScannerInfo() {
    return RWDb::readScannerInfo();
}

void CoreScanner::writeScannerInfo(const std::map<std::string, std::string>& info) {
    RWDb::writeScannerInfo(info);
}

std::string CoreScanner::getLastError() const {
    return m_impl->lastError;
}

void CoreScanner::clearError() {
    m_impl->lastError.clear();
}

} // namespace HGMACHINE
