#include "ScannerAdapter.h"
#include "CoreScanner.h"

namespace HGMACHINE {

class ScannerAdapter::Impl {
public:
    CoreScanner coreScanner;
    std::string lastError;
};

ScannerAdapter::ScannerAdapter() : m_impl(new Impl()) {
}

ScannerAdapter::~ScannerAdapter() {
    delete m_impl;
}

bool ScannerAdapter::initialize() {
    return m_impl->coreScanner.initialize();
}

void ScannerAdapter::shutdown() {
    m_impl->coreScanner.shutdown();
}

std::map<std::string, std::string> ScannerAdapter::readScannerInfo() {
    return m_impl->coreScanner.readScannerInfo();
}

void ScannerAdapter::writeScannerInfo(const std::map<std::string, std::string>& info) {
    m_impl->coreScanner.writeScannerInfo(info);
}

std::string ScannerAdapter::getLastError() const {
    return m_impl->lastError;
}

void ScannerAdapter::clearError() {
    m_impl->lastError.clear();
}

} // namespace HGMACHINE
