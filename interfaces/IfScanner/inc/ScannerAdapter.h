#ifndef SCANNERADAPTER_H
#define SCANNERADAPTER_H

#include "IScanner.h"
#include <memory>

namespace HGMACHINE {

class ScannerAdapter : public IScanner {
public:
    ScannerAdapter();
    ~ScannerAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    std::map<std::string, std::string> readScannerInfo() override;
    void writeScannerInfo(const std::map<std::string, std::string>& info) override;
    
    std::string getLastError() const override;
    void clearError() override;
    
private:
    class Impl;
    Impl* m_impl;
};

} // namespace HGMACHINE

#endif // SCANNERADAPTER_H
