#ifndef IPRINTER_H
#define IPRINTER_H

#include "IfPrinter_global.h"
#include <string>
#include <vector>

namespace HGMACHINE {

class IFPRINTER_EXPORT IPrinter {
public:
    virtual ~IPrinter() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual std::vector<std::string> getPrinterList() = 0;
    
    virtual bool printFile(const std::string& filePath, const std::string& printerName, 
                       int copies, const std::string& pageSize, 
                       double pageWidth = 0.0, double pageHeight = 0.0) = 0;
    virtual bool printExcelFile(const std::string& filePath, int copies, 
                             const std::string& pageSize, 
                             double pageWidth = 0.0, 
                             double pageHeight = 0.0) = 0;
    virtual bool printWordFile(const std::string& filePath, int copies, 
                            const std::string& pageSize, 
                            double pageWidth = 0.0, 
                            double pageHeight = 0.0) = 0;
    virtual bool printOfficeFile(const std::string& filePath, int copies, 
                              const std::string& pageSize, 
                              double pageWidth = 0.0, 
                              double pageHeight = 0.0) = 0;
    
    virtual bool setDefaultPrinter(const std::string& printerName) = 0;
    virtual std::string getDefaultPrinter() const = 0;
    
    virtual std::string getLastError() const = 0;
    virtual void clearError() = 0;
};

}

#endif // IPRINTER_H
