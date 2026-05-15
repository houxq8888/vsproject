#ifndef IFSCANNER_H
#define IFSCANNER_H

#include "IfScanner_global.h"
#include <string>
#include <map>

namespace HGMACHINE {

class IFSCANNER_EXPORT IScanner {
public:
    virtual ~IScanner() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::map<std::string, std::string> readScannerInfo() = 0;
    virtual void writeScannerInfo(const std::map<std::string, std::string>& info) = 0;
    
    virtual std::string getLastError() const = 0;
    virtual void clearError() = 0;
};

} // namespace HGMACHINE

#endif // IFSCANNER_H
