#include "PrinterAdapter.h"
#include "CorePrinter.h"

namespace HGMACHINE {

class PrinterAdapter::Impl {
public:
    CorePrinter corePrinter;
};

PrinterAdapter::PrinterAdapter() : m_impl(new Impl()) {
}

PrinterAdapter::~PrinterAdapter() {
    delete m_impl;
}

bool PrinterAdapter::initialize() {
    return m_impl->corePrinter.initialize();
}

void PrinterAdapter::shutdown() {
    m_impl->corePrinter.shutdown();
}

std::vector<std::string> PrinterAdapter::getPrinterList() {
    return m_impl->corePrinter.getPrinterList();
}

bool PrinterAdapter::printFile(const std::string& filePath, const std::string& printerName, 
                              int copies, const std::string& pageSize, 
                              double pageWidth, double pageHeight) {
    PrintInfo printInfo;
    printInfo.filePath = filePath;
    printInfo.printerName = printerName;
    printInfo.copyNum = copies;
    printInfo.pageSize = pageSize;
    printInfo.dmPaperWidth = pageWidth;
    printInfo.dmPaperHeight = pageHeight;
    return m_impl->corePrinter.printFile(printInfo);
}

bool PrinterAdapter::printExcelFile(const std::string& filePath, int copies, 
                                   const std::string& pageSize, 
                                   double pageWidth, 
                                   double pageHeight) {
    return m_impl->corePrinter.printExcelFile(filePath, copies, pageSize, pageWidth, pageHeight);
}

bool PrinterAdapter::printWordFile(const std::string& filePath, int copies, 
                                  const std::string& pageSize, 
                                  double pageWidth, 
                                  double pageHeight) {
    return m_impl->corePrinter.printWordFile(filePath, copies, pageSize, pageWidth, pageHeight);
}

bool PrinterAdapter::printOfficeFile(const std::string& filePath, int copies, 
                                    const std::string& pageSize, 
                                    double pageWidth, 
                                    double pageHeight) {
    return m_impl->corePrinter.printOfficeFile(filePath, copies, pageSize, pageWidth, pageHeight);
}

bool PrinterAdapter::setDefaultPrinter(const std::string& printerName) {
    return m_impl->corePrinter.setDefaultPrinter(printerName);
}

std::string PrinterAdapter::getDefaultPrinter() const {
    return m_impl->corePrinter.getDefaultPrinter();
}

std::string PrinterAdapter::getLastError() const {
    return m_impl->corePrinter.getLastError();
}

void PrinterAdapter::clearError() {
    m_impl->corePrinter.clearError();
}

}
