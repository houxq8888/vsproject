#include "hgprinter.h"

namespace HGMACHINE{
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
double mmToPoints(double mm){
    return mm * 72.0 / 25.4;
}
std::string wstringToUtf8(const std::wstring& wstr){
    if (wstr.empty()) return {};
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size_needed - 1, 0); // -1 是为了去掉 null terminator
    WideCharToMultiByte(CP_UTF8,0,wstr.c_str(),-1,&result[0],size_needed,nullptr,nullptr);
    return result;
}
// UTF-8 → std::wstring（宽字符）
std::wstring utf8ToWstring(const std::string& str) {
    if (str.empty()) return {};

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(size_needed - 1, 0);  // -1 去掉 null terminator
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size_needed);
    return result;
}    
// 获取打印机列表
    std::vector<std::string> getPrinerList() {
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
    
    bool printExcelFile(const std::string& filePathUtf8, int copies, int paperSize,int pageW=0,int pageH=0) {
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

                    if (paperSize == 0 && pageW > 0 && pageH > 0)
                    {
                        double widthPt = mmToPoints(pageW);
                        double heightPt = mmToPoints(pageH);
                        // 设置 PaperWidth
                        {
                            DISPID dispidWidth;
                            OLECHAR *name = L"PaperWidth";
                            if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidWidth)))
                            {
                                VARIANT arg;
                                VariantInit(&arg);
                                arg.vt = VT_R8;
                                arg.dblVal = widthPt;
                                DISPPARAMS params = {&arg, nullptr, 1, 0};
                                pPageSetup->Invoke(dispidWidth, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                            }
                        }

                        // 设置 PaperHeight
                        {
                            DISPID dispidHeight;
                            OLECHAR *name = L"PaperHeight";
                            if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidHeight)))
                            {
                                VARIANT arg;
                                VariantInit(&arg);
                                arg.vt = VT_R8;
                                arg.dblVal = heightPt;
                                DISPPARAMS params = {&arg, nullptr, 1, 0};
                                pPageSetup->Invoke(dispidHeight, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                            }
                        }
                    }
                    else
                    {
                        DISPID dispidPaperSize;
                        OLECHAR *propPaperSize = L"PaperSize";
                        hr = pPageSetup->GetIDsOfNames(IID_NULL, &propPaperSize, 1, LOCALE_USER_DEFAULT, &dispidPaperSize);
                        if (SUCCEEDED(hr))
                        {
                            VARIANT arg;
                            VariantInit(&arg);
                            arg.vt = VT_INT;
                            arg.intVal = paperSize;

                            DISPPARAMS params = {&arg, nullptr, 1, 0};
                            pPageSetup->Invoke(dispidPaperSize, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                        }
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

    bool printWordFile(const std::string& filePathUtf8, int copies, int paperSize,int pageW=0,int pageH=0) {
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
        // Set PageSetup.PaperSize
        {
            DISPID dispidPageSetup;
            OLECHAR* propPageSetup = L"PageSetup";
            hr = pDocuments->GetIDsOfNames(IID_NULL, &propPageSetup, 1, LOCALE_USER_DEFAULT, &dispidPageSetup);
            if (FAILED(hr)) {
                std::cerr << "GetIDsOfNames PageSetup failed\n";
            } else {
                VARIANT result;
                VariantInit(&result);
                DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
                hr = pDocuments->Invoke(dispidPageSetup, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, NULL, NULL);
                if (SUCCEEDED(hr) && result.vt == VT_DISPATCH && result.pdispVal) {
                    IDispatch* pPageSetup = result.pdispVal;

                    if (paperSize == 0 && pageW > 0 && pageH > 0)
                    {
                        double widthPt = mmToPoints(pageW);
                        double heightPt = mmToPoints(pageH);
                        // 设置 PaperWidth
                        {
                            DISPID dispidWidth;
                            OLECHAR *name = L"PaperWidth";
                            if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidWidth)))
                            {
                                VARIANT arg;
                                VariantInit(&arg);
                                arg.vt = VT_R8;
                                arg.dblVal = widthPt;
                                DISPPARAMS params = {&arg, nullptr, 1, 0};
                                pPageSetup->Invoke(dispidWidth, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                            }
                        }

                        // 设置 PaperHeight
                        {
                            DISPID dispidHeight;
                            OLECHAR *name = L"PaperHeight";
                            if (SUCCEEDED(pPageSetup->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispidHeight)))
                            {
                                VARIANT arg;
                                VariantInit(&arg);
                                arg.vt = VT_R8;
                                arg.dblVal = heightPt;
                                DISPPARAMS params = {&arg, nullptr, 1, 0};
                                pPageSetup->Invoke(dispidHeight, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                            }
                        }
                    }
                    else
                    {
                        DISPID dispidPaperSize;
                        OLECHAR *propPaperSize = L"PaperSize";
                        hr = pPageSetup->GetIDsOfNames(IID_NULL, &propPaperSize, 1, LOCALE_USER_DEFAULT, &dispidPaperSize);
                        if (SUCCEEDED(hr))
                        {
                            VARIANT arg;
                            VariantInit(&arg);
                            arg.vt = VT_INT;
                            arg.intVal = paperSize;

                            DISPPARAMS params = {&arg, nullptr, 1, 0};
                            pPageSetup->Invoke(dispidPaperSize, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
                        }
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

    bool printOfficeFile(const std::string& filePathUtf8, int copies, int paperSize,int pageW=0,int pageH=0) {
        if (filePathUtf8.empty()) return false;

        std::string fileLower = filePathUtf8;
        std::transform(fileLower.begin(), fileLower.end(), fileLower.begin(), ::tolower);

        if (fileLower.find(".doc") != std::string::npos) {
            return printWordFile(filePathUtf8, copies, paperSize, pageW,pageH);
        } else if (fileLower.find(".xls") != std::string::npos || fileLower.find(".xlsx") != std::string::npos) {
            return printExcelFile(filePathUtf8, copies, paperSize,pageW,pageH);
        } else {
            std::cerr << "Unsupported file format\n";
            return false;
        }
    }
    // "A4","Letter","B5","A5","B6","A6","Executive","16K","自定义尺寸"
    // 打印PDF文件（默认打印机或指定打印机）
    int printFile(const PrintInfo &printInfo) {
        int paperSize = 9;
        if (printInfo.pageSize == "A4")  paperSize = 9;
        else if (printInfo.pageSize == "B5") paperSize = 13;
        else if (printInfo.pageSize == "Letter") paperSize = 1;
        else if (printInfo.pageSize == "A5") paperSize = 11;
        else if (printInfo.pageSize == "B6") paperSize = 86;
        else if (printInfo.pageSize == "A6") paperSize = 70;
        else if (printInfo.pageSize == "Executive") paperSize = 7;
        else if (printInfo.pageSize == "16K") paperSize = 78;
        else if (printInfo.pageSize == "自定义尺寸") paperSize = 0;

        printOfficeFile(printInfo.filePath, printInfo.copyNum, paperSize,printInfo.dmPaperWidth,printInfo.dmPaperHeight);
        return 0;
        std::string filePathUtf8 = printInfo.filePath;
        std::string printerNameUtf8  = printInfo.printerName;
    // bool printPdf(const std::string& filePathUtf8, const std::string& printerNameUtf8  = "") {
        std::wstring filePath = utf8ToWstring(filePathUtf8);
        std::wstring printerName = utf8ToWstring(printerNameUtf8);
        std::wstring verb = L"print";

        std::wstring parameters;
        if (!printerName.empty()) {
            parameters = L"/p /h \"" + printerName + L"\"";
        }

        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(SHELLEXECUTEINFOW);
        sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_FLAG_NO_UI;
        sei.hwnd = NULL;
        sei.lpVerb = verb.c_str();
        sei.lpFile = filePath.c_str();
        sei.lpParameters = parameters.empty() ? NULL : parameters.c_str();
        sei.lpDirectory = NULL;
        sei.nShow = SW_HIDE;


        return ShellExecuteExW(&sei) != 0;
    }

    bool setDefaultPrinter(const std::string& printerNameUtf8) {
        std::wstring printerName = utf8ToWstring(printerNameUtf8);
        if (SetDefaultPrinterW(printerName.c_str())) {
            std::wcout << L"默认打印机已设置为: " << printerName << std::endl;
            return true;
        } else {
            std::wcerr << L"设置默认打印机失败，错误代码: " << GetLastError() << std::endl;
            return false;
        }
    }
#else

std::vector<std::string> getPrinerList(){
    std::vector<std::string> printerLists;
    int numPrinters;
    cups_dest_t *printers;
    numPrinters = cupsGetDests(&printers);
    if (numPrinters == 0){
        return printerLists;
    }
    for (int i = 0; i < numPrinters; i++){
        printerLists.push_back(printers[i].name);
    }
    return printerLists;
}

int printFile(const PrintInfo &printInfo) {
    cups_dest_t *dests;
    int numDests = cupsGetDests(&dests);
    cups_dest_t *dest = cupsGetDest(printInfo.printerName.c_str(), nullptr, numDests, dests);

    if (!dest) {
        // std::cerr << "无法找到打印机: " << printerName << std::endl;
        cupsFreeDests(numDests, dests);
        return -1;
    }
    // const char *printer_name = "MyPrinter";  // 你的打印机名称
    // const char *file_path = "/path/to/document.txt";  // 要打印的文件
    // const char *title = "My Print Job";  // 任务名称
    int numOptions = 0;
    cups_option_t *options = NULL;

    // set print copyies
    numOptions=cupsAddOption("copies", std::to_string(printInfo.copyNum).c_str(), numOptions, &options);
    // 设置自定义纸张尺寸，这里假设 printInfo.widthMm 和 printInfo.heightMm 是纸张宽和高，单位mm
    if (printInfo.pageSize == "自定义尺寸"){
        char mediaOption[64];
        snprintf(mediaOption, sizeof(mediaOption), "Custom.%dx%dmm", int(printInfo.dmPaperWidth), int(printInfo.dmPaperHeight));
        numOptions=cupsAddOption("media", mediaOption, numOptions, &options);
    } else {
        numOptions=cupsAddOption("media", printInfo.pageSize.c_str(), numOptions, &options);
    }
    // numOptions=cupsAddOption("orientation-requested", "3", numOptions, &options); //"3"（纵向）, "4"（横向）
    // numOptions=cupsAddOption("page-ranges", "1-2", numOptions, &options);
    // numOptions=cupsAddOption("sides","one-sided",numOptions,&options); // 	"one-sided", "two-sided-long-edge", "two-sided-short-edge"

    int jobId = cupsPrintFile(printInfo.printerName.c_str(), printInfo.filePath.c_str(), "", numOptions, options);
    if (jobId > 0) {
        // return 0;
        // std::cout << "打印任务提交成功，任务 ID：" << job_id << std::endl;
    } else {
        cupsFreeDests(numDests, dests);
        return -1;
        // std::cerr << "打印失败：" << cupsLastErrorString() << std::endl;
    }
    cupsFreeDests(numDests, dests);
    return 0;
}

#endif

}