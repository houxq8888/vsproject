#ifndef SCANNERMANAGER_H
#define SCANNERMANAGER_H

#include "IScanner.h"
#include <memory>

namespace HGMACHINE {

class ScannerManager {
public:
    static ScannerManager& instance();
    
    void initialize();
    void shutdown();
    
    IScanner* get();
    
private:
    ScannerManager();
    ~ScannerManager();
    
    ScannerManager(const ScannerManager&) = delete;
    ScannerManager& operator=(const ScannerManager&) = delete;
    
    std::unique_ptr<IScanner> m_scanner;
};

} // namespace HGMACHINE

#endif // SCANNERMANAGER_H
