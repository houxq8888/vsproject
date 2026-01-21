#include "PrinterManager.h"
#include <sstream>
#include <fstream>
#include "config.h"


#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

    std::string wstringToUtf8(const std::wstring &wstr)
    {
        if (wstr.empty())
            return {};
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string result(size_needed - 1, 0); // -1 是为了去掉 null terminator
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
        return result;
    }
    // UTF-8 → std::wstring（宽字符）
    std::wstring utf8ToWstring(const std::string &str)
    {
        if (str.empty())
            return {};

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        std::wstring result(size_needed - 1, 0); // -1 去掉 null terminator
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size_needed);
        return result;
    }
    // 获取打印机列表
    std::vector<std::string> PrinterManager::listPrinters() {
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

    // 打印PDF文件（默认打印机或指定打印机）
    bool PrinterManager::printPdfWithPDFtoPrinter(const std::string& filePathUtf8, const std::string& printerNameUtf8) {
        std::wstring filePath = utf8ToWstring(filePathUtf8);
        std::wstring printerName = utf8ToWstring(printerNameUtf8);
        std::ostringstream ss;
        ss<<HGMACHINE::FileConfig::getDirPath()<<"/resources/PDFtoPrinter.exe";
        std::string exePathS=ss.str();
        std::wstring exePath = utf8ToWstring(exePathS);
        std::wstring command = L"\"" + exePath + L"\" \"" + filePath + L"\"";

        if (!printerName.empty()) {
            command += L" \"" + printerName + L"\"";
        }
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };

        if (CreateProcessW(
                NULL,
                &command[0],  // 注意必须是可修改的字符串
                NULL, NULL, FALSE,
                CREATE_NO_WINDOW,
                NULL, NULL,
                &si, &pi
                )) {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return true;
        } else {
            MessageBoxW(NULL, L"PDFtoPrinter 调用失败", L"错误", MB_ICONERROR);
            return false;
        }
    }

    bool PrinterManager::setDefaultPrinter(const std::string& printerNameUtf8) {
        std::wstring printerName = utf8ToWstring(printerNameUtf8);
        if (SetDefaultPrinterW(printerName.c_str())) {
            std::wcout << L"默认打印机已设置为: " << printerName << std::endl;
            return true;
        } else {
            std::wcerr << L"设置默认打印机失败，错误代码: " << GetLastError() << std::endl;
            return false;
        }
    }

    bool PrinterManager::printExcelFile(const std::string& filePathUtf8, int copies, int paperSize) {
        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            std::cerr << "CoInitialize failed\n";
            return false;
        }

        CLSID clsid;
        hr = CLSIDFromProgID(L"Excel.Application", &clsid);
        if (FAILED(hr)) {
            std::cerr << "CLSIDFromProgID Excel failed\n";
            CoUninitialize();
            return false;
        }

        IDispatch* pExcelApp = nullptr;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pExcelApp);
        if (FAILED(hr) || !pExcelApp) {
            std::cerr << "Excel not registered or cannot create instance\n";
            CoUninitialize();
            return false;
        }

        // Excel.Visible = false
        {
            VARIANT varVisible;
            varVisible.vt = VT_BOOL;
            varVisible.boolVal = VARIANT_FALSE;
            DISPID dispidVisible;
            OLECHAR* propVisible = L"Visible";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &propVisible, 1, LOCALE_USER_DEFAULT, &dispidVisible);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Visible failed\n";
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
            DISPPARAMS params = { &varVisible, NULL, 1, 0 };
            hr = pExcelApp->Invoke(dispidVisible, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
            if (FAILED(hr)) {
                std::cerr << "Invoke Visible failed\n";
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
        }

        // Get Workbooks collection
        IDispatch* pWorkbooks = nullptr;
        {
            DISPID dispidWorkbooks;
            OLECHAR* propWorkbooks = L"Workbooks";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &propWorkbooks, 1, LOCALE_USER_DEFAULT, &dispidWorkbooks);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Workbooks failed\n";
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }

            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pExcelApp->Invoke(dispidWorkbooks, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                std::cerr << "Invoke Workbooks failed\n";
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
            pWorkbooks = result.pdispVal;
        }

        // Open workbook
        IDispatch* pWorkbook = nullptr;
        {
            DISPID dispidOpen;
            OLECHAR* methodOpen = L"Open";
            hr = pWorkbooks->GetIDsOfNames(IID_NULL, &methodOpen, 1, LOCALE_USER_DEFAULT, &dispidOpen);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Open failed\n";
                pWorkbooks->Release();
                pExcelApp->Release();
                CoUninitialize();
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
                std::cerr << "Open workbook failed\n";
                pWorkbooks->Release();
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
            pWorkbook = result.pdispVal;
        }

        // Get ActiveSheet
        IDispatch* pActiveSheet = nullptr;
        {
            DISPID dispidActiveSheet;
            OLECHAR* propActiveSheet = L"ActiveSheet";
            hr = pExcelApp->GetIDsOfNames(IID_NULL, &propActiveSheet, 1, LOCALE_USER_DEFAULT, &dispidActiveSheet);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames ActiveSheet failed\n";
                pWorkbook->Release();
                pWorkbooks->Release();
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pExcelApp->Invoke(dispidActiveSheet, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                std::cerr << "Invoke ActiveSheet failed\n";
                pWorkbook->Release();
                pWorkbooks->Release();
                pExcelApp->Release();
                CoUninitialize();
                return false;
            }
            pActiveSheet = result.pdispVal;
        }

        // Set PageSetup.PaperSize
        {
            DISPID dispidPageSetup;
            OLECHAR* propPageSetup = L"PageSetup";
            hr = pActiveSheet->GetIDsOfNames(IID_NULL, &propPageSetup, 1, LOCALE_USER_DEFAULT, &dispidPageSetup);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames PageSetup failed\n";
            } else {
                VARIANT result;
                VariantInit(&result);
                DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
                hr = pActiveSheet->Invoke(dispidPageSetup, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
                if (SUCCEEDED(hr) && result.vt == VT_DISPATCH && result.pdispVal) {
                    IDispatch* pPageSetup = result.pdispVal;

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

                    pPageSetup->Release();
                }
                VariantClear(&result);
            }
        }

        // PrintOut Copies = copies
        {
            DISPID dispidPrintOut;
            OLECHAR* methodPrintOut = L"PrintOut";
            hr = pActiveSheet->GetIDsOfNames(IID_NULL, &methodPrintOut, 1, LOCALE_USER_DEFAULT, &dispidPrintOut);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames PrintOut failed\n";
            } else {
                VARIANT argCopies;
                VariantInit(&argCopies);
                argCopies.vt = VT_INT;
                argCopies.intVal = copies;

                DISPPARAMS params = { &argCopies, nullptr, 1, 0 };
                hr = pActiveSheet->Invoke(dispidPrintOut, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        // Close Workbook without saving
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

        // Quit Excel
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
        pExcelApp->Release();

        CoUninitialize();
        return true;
    }

    bool PrinterManager::printWordFile(const std::string& filePathUtf8, int copies) {
        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            std::cerr << "CoInitialize failed\n";
            return false;
        }

        CLSID clsid;
        hr = CLSIDFromProgID(L"Word.Application", &clsid);
        if (FAILED(hr)) {
            std::cerr << "CLSIDFromProgID Word failed\n";
            CoUninitialize();
            return false;
        }

        IDispatch* pWordApp = nullptr;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pWordApp);
        if (FAILED(hr) || !pWordApp) {
            std::cerr << "Word not registered or cannot create instance\n";
            CoUninitialize();
            return false;
        }

        // Word.Visible = false
        {
            VARIANT varVisible;
            varVisible.vt = VT_BOOL;
            varVisible.boolVal = VARIANT_FALSE;
            DISPID dispidVisible;
            OLECHAR* propVisible = L"Visible";
            hr = pWordApp->GetIDsOfNames(IID_NULL, &propVisible, 1, LOCALE_USER_DEFAULT, &dispidVisible);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Visible failed\n";
                pWordApp->Release();
                CoUninitialize();
                return false;
            }
            DISPPARAMS params = { &varVisible, NULL, 1, 0 };
            hr = pWordApp->Invoke(dispidVisible, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
            if (FAILED(hr)) {
                std::cerr << "Invoke Visible failed\n";
                pWordApp->Release();
                CoUninitialize();
                return false;
            }
        }

        // Get Documents collection
        IDispatch* pDocuments = nullptr;
        {
            DISPID dispidDocuments;
            OLECHAR* propDocuments = L"Documents";
            hr = pWordApp->GetIDsOfNames(IID_NULL, &propDocuments, 1, LOCALE_USER_DEFAULT, &dispidDocuments);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Documents failed\n";
                pWordApp->Release();
                CoUninitialize();
                return false;
            }

            VARIANT result;
            VariantInit(&result);
            DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
            hr = pWordApp->Invoke(dispidDocuments, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
            if (FAILED(hr) || result.vt != VT_DISPATCH || !result.pdispVal) {
                std::cerr << "Invoke Documents failed\n";
                pWordApp->Release();
                CoUninitialize();
                return false;
            }
            pDocuments = result.pdispVal;
        }

        // Open document
        IDispatch* pDocument = nullptr;
        {
            DISPID dispidOpen;
            OLECHAR* methodOpen = L"Open";
            hr = pDocuments->GetIDsOfNames(IID_NULL, &methodOpen, 1, LOCALE_USER_DEFAULT, &dispidOpen);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames Open failed\n";
                pDocuments->Release();
                pWordApp->Release();
                CoUninitialize();
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
                std::cerr << "Open document failed\n";
                pDocuments->Release();
                pWordApp->Release();
                CoUninitialize();
                return false;
            }
            pDocument = result.pdispVal;
        }

        // PrintOut Copies = copies
        {
            DISPID dispidPrintOut;
            OLECHAR* methodPrintOut = L"PrintOut";
            hr = pDocument->GetIDsOfNames(IID_NULL, &methodPrintOut, 1, LOCALE_USER_DEFAULT, &dispidPrintOut);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames PrintOut failed\n";
            } else {
                VARIANT argCopies;
                VariantInit(&argCopies);
                argCopies.vt = VT_INT;
                argCopies.intVal = copies;

                DISPPARAMS params = { &argCopies, nullptr, 1, 0 };
                hr = pDocument->Invoke(dispidPrintOut, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
            }
        }

        // Close document without saving
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

        // Quit Word
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
        pWordApp->Release();

        CoUninitialize();
        return true;
    }

    bool PrinterManager::printOfficeFile(const std::string& filePathUtf8, int copies, int paperSize) {
        if (filePathUtf8.empty()) return false;

        std::string fileLower = filePathUtf8;
        std::transform(fileLower.begin(), fileLower.end(), fileLower.begin(), ::tolower);

        if (fileLower.find(".doc") != std::string::npos) {
            return printWordFile(filePathUtf8, copies);
        } else if (fileLower.find(".xls") != std::string::npos || fileLower.find(".xlsx") != std::string::npos) {
            return printExcelFile(filePathUtf8, copies, paperSize);
        } else {
            std::cerr << "Unsupported file format\n";
            return false;
        }
    }


#else

#endif