#ifndef COREPRINTER_H
#define COREPRINTER_H

#include "CorePrinter_global.h"
#include "PrintTypes.h"
#include <string>
#include <vector>

namespace HGMACHINE {


class COREPRINTER_EXPORT CorePrinter {
public:
    CorePrinter();
    ~CorePrinter();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::string> getPrinterList();
    
    bool printFile(const PrintInfo& printInfo);
    bool printExcelFile(const std::string& filePath, int copies, 
                       const std::string& pageSize, 
                       double pageWidth = 0.0, 
                       double pageHeight = 0.0);
    bool printWordFile(const std::string& filePath, int copies, 
                      const std::string& pageSize, 
                      double pageWidth = 0.0, 
                      double pageHeight = 0.0);
    bool printOfficeFile(const std::string& filePath, int copies, 
                        const std::string& pageSize, 
                        double pageWidth = 0.0, 
                        double pageHeight = 0.0);
    
    bool setDefaultPrinter(const std::string& printerName);
    std::string getDefaultPrinter() const;
    
    std::string getLastError() const;
    void clearError();
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // COREPRINTER_H
