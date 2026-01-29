#ifndef PRINTERADAPTER_H
#define PRINTERADAPTER_H

#include "IPrinter.h"
#include <memory>

namespace HGMACHINE {

class PrinterAdapter : public IPrinter {
public:
    PrinterAdapter();
    ~PrinterAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    std::vector<std::string> getPrinterList() override;
    
    bool printFile(const std::string& filePath, const std::string& printerName, 
                  int copies, const std::string& pageSize, 
                  double pageWidth = 0.0, double pageHeight = 0.0) override;
    bool printExcelFile(const std::string& filePath, int copies, 
                       const std::string& pageSize, 
                       double pageWidth = 0.0, 
                       double pageHeight = 0.0) override;
    bool printWordFile(const std::string& filePath, int copies, 
                      const std::string& pageSize, 
                      double pageWidth = 0.0, 
                      double pageHeight = 0.0) override;
    bool printOfficeFile(const std::string& filePath, int copies, 
                        const std::string& pageSize, 
                        double pageWidth = 0.0, 
                        double pageHeight = 0.0) override;
    
    bool setDefaultPrinter(const std::string& printerName) override;
    std::string getDefaultPrinter() const override;
    
    std::string getLastError() const override;
    void clearError() override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif
