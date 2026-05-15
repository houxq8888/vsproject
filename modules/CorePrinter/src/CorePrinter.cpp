#include "CorePrinter.h"
#include <algorithm>
#include <iostream>
#include <sstream>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#include <winspool.h>
#include <shellapi.h>
#else
#include <cups/cups.h>
#endif

namespace HGMACHINE {

class CorePrinter::Impl {
public:
    Impl() : m_initialized(false) {
    }
    
    ~Impl() {
        shutdown();
    }
    
    bool initialize() {
        m_initialized = true;
        return true;
    }
    
    void shutdown() {
        m_initialized = false;
    }
    
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    
    double mmToPoints(double mm) {
        return mm * 72.0 / 25.4;
    }
    
    std::string wstringToUtf8(const std::wstring& wstr) {
        if (wstr.empty()) return {};
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string result(size_needed - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
        return result;
    }
    
    std::wstring utf8ToWstring(const std::string& str) {
        if (str.empty()) return {};
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        std::wstring result(size_needed - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size_needed);
        return result;
    }
    
    std::vector<std::string> getPrinterList() {
        DWORD needed = 0, returned = 0;
        EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &needed, &returned);

        std::vector<std::string> result;
        if (needed == 0) return result;

        std::vector<BYTE> buffer(needed);
        if (EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, buffer.data(), needed, &needed, &returned)) {
            PRINTER_INFO_2W* info = reinterpret_cast<PRINTER_INFO_2W*>(buffer.data());
            for (DWORD i = 0; i < returned; ++i) {
                std::wstring wname = info[i].pPrinterName;
                result.push_back(wstringToUtf8(wname));
            }
        }
        return result;
    }
    
    bool printExcelFile(const std::string& filePathUtf8, int copies, const std::string& pageSize, double pageWidth, double pageHeight) {
        int paperSize = getPaperSize(pageSize);
        
        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            m_lastError = "CoInitialize failed";
            return false;
        }

        CLSID clsid;
        hr = CLSIDFromProgID(L"Excel.Application", &clsid);
        if (FAILED(hr)) {
            m_lastError = "CLSIDFromProgID Excel failed";
            CoUninitialize();
            return false;
        }

        IDispatch* pExcelApp = nullptr;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp);
        if (FAILED(hr) || !pExcelApp) {
            m_lastError = "Excel not registered or cannot create instance";
            CoUninitialize();
            return false;
        }

        bool result = printExcelFileImpl(pExcelApp, filePathUtf8, copies, paperSize, pageWidth, pageHeight);
        
        pExcelApp->Release();
        CoUninitialize();
        return result;
    }
    
    bool printExcelFileImpl(IDispatch* pExcelApp, const std::string& filePathUtf8, int copies, int paperSize, double pageWidth, double pageHeight) {
        HRESULT hr;
        
        VARIANT varVisible;
        varVisible.vt = VT_BOOL;
        varVisible.boolVal = VARIANT_FALSE;
        DISPID dispidVisible;
        OLECHAR* propVisible = L"Visible";
        hr = pExcelApp->GetIDsOfNames(IID_NULL, &propVisible, 1, LOCALE_USER_DEFAULT, &dispidVisible);
        if (FAILED(hr)) {
            m_lastError = "GetIDsOfNames Visible failed";
            return false;
        }
        DISPPARAMS params = { &varVisible, NULL, 1, 0 };
        hr = pExcelApp->Invoke(dispidVisible, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
        if (FAILED(hr)) {
            m_lastError = "Invoke Visible failed";
            return false;
        }

        IDispatch* pWorkbooks = nullptr;
        {
            DISPID dispidWorkbooks;
            OLECHAR* propWorkbooks = L"Workbooks";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &propWorkbooks, 1, LOCALE_USER_DEFAULT, &dispidWorkbooks);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames Workbooks failed";
                return false;
            }

            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pExcelApp->Invoke(dispidWorkbooks, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                m_lastError = "Invoke Workbooks failed";
                return false;
            }
            pWorkbooks = result.pdispVal;
        }

        IDispatch* pWorkbook = nullptr;
        {
            DISPID dispidOpen;
            OLECHAR* methodOpen = L"Open";
            hr = pWorkbooks->GetIDsOfNames(IID_NULL, &methodOpen, 1, LOCALE_USER_DEFAULT, &dispidOpen);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames Open failed";
                pWorkbooks->Release();
                return false;
            }

            std::wstring wFilePath = utf8ToWstring(filePathUtf8);
            VARIANT argFileName;
            VariantInit(&argFileName);
            argFileName.vt = VT_BSTR;
            argFileName.bstrVal = SysAllocString(wFilePath.c_str());

            DISPPARAMS params = { &argFileName, nullptr, 1, 0 };
            VARIANT result;
            VariantInit(&result);

            hr = pWorkbooks->Invoke(dispidOpen, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &result, NULL, NULL);

            SysFreeString(argFileName.bstrVal);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                m_lastError = "Open workbook failed";
                pWorkbooks->Release();
                return false;
            }
            pWorkbook = result.pdispVal;
        }

        IDispatch* pActiveSheet = nullptr;
        {
            DISPID dispidActiveSheet;
            OLECHAR* propActiveSheet = L"ActiveSheet";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &propActiveSheet, 1, LOCALE_USER_DEFAULT, &dispidActiveSheet);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames ActiveSheet failed";
                pWorkbook->Release();
                pWorkbooks->Release();
                return false;
            }
            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pExcelApp->Invoke(dispidActiveSheet, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                m_lastError = "Invoke ActiveSheet failed";
                pWorkbook->Release();
                pWorkbooks->Release();
                return false;
            }
            pActiveSheet = result.pdispVal;
        }

        setPageSetup(pActiveSheet, paperSize, pageWidth, pageHeight);

        {
            DISPID dispidPrintOut;
            OLECHAR* methodPrintOut = L"PrintOut";
            hr = pActiveSheet->GetIDsOfNames(IID_NULL, &methodPrintOut, 1, LOCALE_USER_DEFAULT, &dispidPrintOut);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames PrintOut failed";
            } else {
                VARIANT argCopies;
                VariantInit(&argCopies);
                argCopies.vt = VT_INT;
                argCopies.intVal = copies;

                DISPPARAMS params = { &argCopies, nullptr, 1, 0 };
                hr = pActiveSheet->Invoke(dispidPrintOut, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        {
            DISPID dispidClose;
            OLECHAR* methodClose = L"Close";
            hr = pWorkbook->GetIDsOfNames(IID_NULL, &methodClose, 1, LOCALE_USER_DEFAULT, &dispidClose);
            if (SUCCEEDED(hr)) {
                VARIANT argSaveChanges;
                VariantInit(&argSaveChanges);
                argSaveChanges.vt = VT_BOOL;
                argSaveChanges.boolVal = VARIANT_FALSE;
                DISPPARAMS params = { &argSaveChanges, nullptr, 1, 0 };
                pWorkbook->Invoke(dispidClose, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        {
            DISPID dispidQuit;
            OLECHAR* methodQuit = L"Quit";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &methodQuit, 1, LOCALE_USER_DEFAULT, &dispidQuit);
            if (SUCCEEDED(hr)) {
                DISPPARAMS params = { nullptr, nullptr, 0, 0 };
                pExcelApp->Invoke(dispidQuit, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        pActiveSheet->Release();
        pWorkbook->Release();
        pWorkbooks->Release();

        return true;
    }
    
    bool printWordFile(const std::string& filePathUtf8, int copies, const std::string& pageSize, double pageWidth, double pageHeight) {
        int paperSize = getPaperSize(pageSize);
        
        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            m_lastError = "CoInitialize failed";
            return false;
        }

        CLSID clsid;
        hr = CLSIDFromProgID(L"Word.Application", &clsid);
        if (FAILED(hr)) {
            m_lastError = "CLSIDFromProgID Word failed";
            CoUninitialize();
            return false;
        }

        IDispatch* pWordApp = nullptr;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pWordApp);
        if (FAILED(hr) || !pWordApp) {
            m_lastError = "Word not registered or cannot create instance";
            CoUninitialize();
            return false;
        }

        bool result = printWordFileImpl(pWordApp, filePathUtf8, copies, paperSize, pageWidth, pageHeight);
        
        pWordApp->Release();
        CoUninitialize();
        return result;
    }
    
    bool printWordFileImpl(IDispatch* pWordApp, const std::string& filePathUtf8, int copies, int paperSize, double pageWidth, double pageHeight) {
        HRESULT hr;
        
        VARIANT varVisible;
        varVisible.vt = VT_BOOL;
        varVisible.boolVal = VARIANT_FALSE;
        DISPID dispidVisible;
        OLECHAR* propVisible = L"Visible";
        hr = pWordApp->GetIDsOfNames(IID_NULL, &propVisible, 1, LOCALE_USER_DEFAULT, &dispidVisible);
        if (FAILED(hr)) {
            m_lastError = "GetIDsOfNames Visible failed";
            return false;
        }
        DISPPARAMS params = { &varVisible, NULL, 1, 0 };
        hr = pWordApp->Invoke(dispidVisible, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
        if (FAILED(hr)) {
            m_lastError = "Invoke Visible failed";
            return false;
        }

        IDispatch* pDocuments = nullptr;
        {
            DISPID dispidDocuments;
            OLECHAR* propDocuments = L"Documents";
            hr = pWordApp->GetIDsOfNames(IID_NULL, &propDocuments, 1, LOCALE_USER_DEFAULT, &dispidDocuments);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames Documents failed";
                return false;
            }

            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pWordApp->Invoke(dispidDocuments, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                m_lastError = "Invoke Documents failed";
                return false;
            }
            pDocuments = result.pdispVal;
        }

        IDispatch* pDocument = nullptr;
        {
            DISPID dispidOpen;
            OLECHAR* methodOpen = L"Open";
            hr = pDocuments->GetIDsOfNames(IID_NULL, &methodOpen, 1, LOCALE_USER_DEFAULT, &dispidOpen);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames Open failed";
                pDocuments->Release();
                return false;
            }

            std::wstring wFilePath = utf8ToWstring(filePathUtf8);
            VARIANT argFileName;
            VariantInit(&argFileName);
            argFileName.vt = VT_BSTR;
            argFileName.bstrVal = SysAllocString(wFilePath.c_str());

            DISPPARAMS params = { &argFileName, nullptr, 1, 0 };
            VARIANT result;
            VariantInit(&result);

            hr = pDocuments->Invoke(dispidOpen, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &result, NULL, NULL);

            SysFreeString(argFileName.bstrVal);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                m_lastError = "Open document failed";
                pDocuments->Release();
                return false;
            }
            pDocument = result.pdispVal;
        }

        setPageSetup(pDocument, paperSize, pageWidth, pageHeight);

        {
            DISPID dispidPrintOut;
            OLECHAR* methodPrintOut = L"PrintOut";
            hr = pDocument->GetIDsOfNames(IID_NULL, &methodPrintOut, 1, LOCALE_USER_DEFAULT, &dispidPrintOut);
            if (FAILED(hr)) {
                m_lastError = "GetIDsOfNames PrintOut failed";
            } else {
                VARIANT argCopies;
                VariantInit(&argCopies);
                argCopies.vt = VT_INT;
                argCopies.intVal = copies;

                DISPPARAMS params = { &argCopies, nullptr, 1, 0 };
                hr = pDocument->Invoke(dispidPrintOut, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        {
            DISPID dispidClose;
            OLECHAR* methodClose = L"Close";
            hr = pDocument->GetIDsOfNames(IID_NULL, &methodClose, 1, LOCALE_USER_DEFAULT, &dispidClose);
            if (SUCCEEDED(hr)) {
                VARIANT argSaveChanges;
                VariantInit(&argSaveChanges);
                argSaveChanges.vt = VT_BOOL;
                argSaveChanges.boolVal = VARIANT_FALSE;
                DISPPARAMS params = { &argSaveChanges, nullptr, 1, 0 };
                pDocument->Invoke(dispidClose, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        {
            DISPID dispidQuit;
            OLECHAR* methodQuit = L"Quit";
            hr = pWordApp->GetIDsOfNames(IID_NULL, &methodQuit, 1, LOCALE_USER_DEFAULT, &dispidQuit);
            if (SUCCEEDED(hr)) {
                DISPPARAMS params = { nullptr, nullptr, 0, 0 };
                pWordApp->Invoke(dispidQuit, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        pDocument->Release();
        pDocuments->Release();

        return true;
    }
    
    void setPageSetup(IDispatch* pObject, int paperSize, double pageWidth, double pageHeight) {
        HRESULT hr;
        DISPID dispidPageSetup;
        OLECHAR* propPageSetup = L"PageSetup";
        hr = pObject->GetIDsOfNames(IID_NULL, &propPageSetup, 1, LOCALE_USER_DEFAULT, &dispidPageSetup);
        if (FAILED(hr)) {
            m_lastError = "GetIDsOfNames PageSetup failed";
            return;
        }
        
        VARIANT result;
        VariantInit(&result);
        DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
        hr = pObject->Invoke(dispidPageSetup, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
        if (SUCCEEDED(hr) && result.vt == VT_DISPATCH && result.pdispVal) {
            IDispatch* pPageSetup = result.pdispVal;

            if (paperSize == 0 && pageWidth > 0 && pageHeight > 0) {
                double widthPt = mmToPoints(pageWidth);
                double heightPt = mmToPoints(pageHeight);
                
                {
                    DISPID dispidWidth;
                    OLECHAR* name = L"PaperWidth";
                    if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidWidth))) {
                        VARIANT arg;
                        VariantInit(&arg);
                        arg.vt = VT_R8;
                        arg.dblVal = widthPt;
                        DISPPARAMS params = { &arg, nullptr, 1, 0 };
                        pPageSetup->Invoke(dispidWidth, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                    }
                }

                {
                    DISPID dispidHeight;
                    OLECHAR* name = L"PaperHeight";
                    if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidHeight))) {
                        VARIANT arg;
                        VariantInit(&arg);
                        arg.vt = VT_R8;
                        arg.dblVal = heightPt;
                        DISPPARAMS params = { &arg, nullptr, 1, 0 };
                        pPageSetup->Invoke(dispidHeight, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                    }
                }
            } else {
                DISPID dispidPaperSize;
                OLECHAR* propPaperSize = L"PaperSize";
                hr = pPageSetup->GetIDsOfNames(IID_NULL, &propPaperSize, 1, LOCALE_USER_DEFAULT, &dispidPaperSize);
                if (SUCCEEDED(hr)) {
                    VARIANT arg;
                    VariantInit(&arg);
                    arg.vt = VT_INT;
                    arg.intVal = paperSize;

                    DISPPARAMS params = { &arg, nullptr, 1, 0 };
                    pPageSetup->Invoke(dispidPaperSize, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                }
            }

            pPageSetup->Release();
        }
        VariantClear(&result);
    }
    
    bool setDefaultPrinter(const std::string& printerNameUtf8) {
        std::wstring printerName = utf8ToWstring(printerNameUtf8);
        if (SetDefaultPrinterW(printerName.c_str())) {
            return true;
        } else {
            m_lastError = "SetDefaultPrinter failed, error code: " + std::to_string(GetLastError());
            return false;
        }
    }
    
    std::string getDefaultPrinter() const {
        return "";
    }
    
#else // Linux platform
    
    std::vector<std::string> getPrinterList() {
        std::vector<std::string> printerLists;
        int numPrinters;
        cups_dest_t *printers;
        numPrinters = cupsGetDests(&printers);
        if (numPrinters == 0) {
            return printerLists;
        }
        for (int i = 0; i < numPrinters; i++) {
            printerLists.push_back(printers[i].name);
        }
        cupsFreeDests(numPrinters, printers);
        return printerLists;
    }
    
    bool printFile(const PrintInfo& printInfo) {
        cups_dest_t *dests;
        int numDests = cupsGetDests(&dests);
        cups_dest_t *dest = cupsGetDest(printInfo.printerName.c_str(), nullptr, numDests, dests);

        if (!dest) {
            m_lastError = "Cannot find printer: " + printInfo.printerName;
            cupsFreeDests(numDests, dests);
            return false;
        }

        int numOptions = 0;
        cups_option_t *options = NULL;

        numOptions = cupsAddOption("copies", std::to_string(printInfo.copyNum).c_str(), numOptions, &options);
        
        if (printInfo.pageSize == "自定义尺寸") {
            char mediaOption[64];
            snprintf(mediaOption, sizeof(mediaOption), "Custom.%dx%dmm", int(printInfo.dmPaperWidth), int(printInfo.dmPaperHeight));
            numOptions = cupsAddOption("media", mediaOption, numOptions, &options);
        } else {
            numOptions = cupsAddOption("media", printInfo.pageSize.c_str(), numOptions, &options);
        }

        int jobId = cupsPrintFile(printInfo.printerName.c_str(), printInfo.filePath.c_str(), "", numOptions, options);
        if (jobId > 0) {
            cupsFreeDests(numDests, dests);
            cupsFreeOptions(numOptions, options);
            return true;
        } else {
            m_lastError = "Print failed: " + std::string(cupsLastErrorString());
            cupsFreeDests(numDests, dests);
            cupsFreeOptions(numOptions, options);
            return false;
        }
    }
    
    bool printExcelFile(const std::string&, int, const std::string&, double, double) {
        m_lastError = "Excel printing not supported on Linux";
        return false;
    }
    
    bool printWordFile(const std::string&, int, const std::string&, double, double) {
        m_lastError = "Word printing not supported on Linux";
        return false;
    }
    
    bool setDefaultPrinter(const std::string&) {
        m_lastError = "SetDefaultPrinter not supported on Linux";
        return false;
    }
    
    std::string getDefaultPrinter() const {
        return "";
    }
    
#endif // Platform check
    
    int getPaperSize(const std::string& pageSize) {
        if (pageSize == "A4") return 9;
        else if (pageSize == "B5") return 13;
        else if (pageSize == "Letter") return 1;
        else if (pageSize == "A5") return 11;
        else if (pageSize == "B6") return 86;
        else if (pageSize == "A6") return 70;
        else if (pageSize == "Executive") return 7;
        else if (pageSize == "16K") return 78;
        else if (pageSize == "自定义尺寸") return 0;
        return 9;
    }
    
    bool m_initialized;
    std::string m_lastError;
};

CorePrinter::CorePrinter() : m_impl(new Impl()) {
}

CorePrinter::~CorePrinter() {
    delete m_impl;
}

bool CorePrinter::initialize() {
    return m_impl->initialize();
}

void CorePrinter::shutdown() {
    m_impl->shutdown();
}

std::vector<std::string> CorePrinter::getPrinterList() {
    return m_impl->getPrinterList();
}

bool CorePrinter::printFile(const PrintInfo& printInfo) {
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return printOfficeFile(printInfo.filePath, printInfo.copyNum, printInfo.pageSize, 
                        printInfo.dmPaperWidth, printInfo.dmPaperHeight);
#else
    return m_impl->printFile(printInfo);
#endif
}

bool CorePrinter::printExcelFile(const std::string& filePath, int copies, 
                                 const std::string& pageSize, 
                                 double pageWidth, 
                                 double pageHeight) {
    return m_impl->printExcelFile(filePath, copies, pageSize, pageWidth, pageHeight);
}

bool CorePrinter::printWordFile(const std::string& filePath, int copies, 
                                const std::string& pageSize, 
                                double pageWidth, 
                                double pageHeight) {
    return m_impl->printWordFile(filePath, copies, pageSize, pageWidth, pageHeight);
}

bool CorePrinter::printOfficeFile(const std::string& filePath, int copies, 
                                  const std::string& pageSize, 
                                  double pageWidth, 
                                  double pageHeight) {
    if (filePath.empty()) {
        m_impl->m_lastError = "File path is empty";
        return false;
    }

    std::string fileLower = filePath;
    std::transform(fileLower.begin(), fileLower.end(), fileLower.begin(), ::tolower);

    if (fileLower.find(".doc") != std::string::npos) {
        return printWordFile(filePath, copies, pageSize, pageWidth, pageHeight);
    } else if (fileLower.find(".xls") != std::string::npos || fileLower.find(".xlsx") != std::string::npos) {
        return printExcelFile(filePath, copies, pageSize, pageWidth, pageHeight);
    } else {
        m_impl->m_lastError = "Unsupported file format";
        return false;
    }
}

bool CorePrinter::setDefaultPrinter(const std::string& printerName) {
    return m_impl->setDefaultPrinter(printerName);
}

std::string CorePrinter::getDefaultPrinter() const {
    return m_impl->getDefaultPrinter();
}

std::string CorePrinter::getLastError() const {
    return m_impl->m_lastError;
}

void CorePrinter::clearError() {
    m_impl->m_lastError.clear();
}

}
