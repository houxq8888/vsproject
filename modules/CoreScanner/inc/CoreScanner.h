#ifndef CORESCANNER_H
#define CORESCANNER_H

#include "CoreScanner_global.h"
#include <string>
#include <map>

namespace HGMACHINE {

class CORESCANNER_EXPORT CoreScanner {
public:
    CoreScanner();
    ~CoreScanner();
    
    bool initialize();
    void shutdown();
    
    std::map<std::string, std::string> readScannerInfo();
    void writeScannerInfo(const std::map<std::string, std::string>& info);
    
    std::string getLastError() const;
    void clearError();
    
private:
    class Impl;
    Impl* m_impl;
};

} // namespace HGMACHINE

#endif // CORESCANNER_H
