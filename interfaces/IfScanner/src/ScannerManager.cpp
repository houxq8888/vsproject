#include "ScannerManager.h"
#include "ScannerAdapter.h"

namespace HGMACHINE {

ScannerManager::ScannerManager() {
}

ScannerManager::~ScannerManager() {
}

ScannerManager& ScannerManager::instance() {
    static ScannerManager instance;
    return instance;
}

void ScannerManager::initialize() {
    if (!m_scanner) {
        m_scanner = std::make_unique<ScannerAdapter>();
        m_scanner->initialize();
    }
}

void ScannerManager::shutdown() {
    if (m_scanner) {
        m_scanner->shutdown();
        m_scanner.reset();
    }
}

IScanner* ScannerManager::get() {
    if (!m_scanner) {
        initialize();
    }
    return m_scanner.get();
}

} // namespace HGMACHINE
