#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <windows.h>
#include <winspool.h>
#include <string>
#include <vector>
#include <iostream>
#include <shellapi.h>
#include <locale>
#include <codecvt>
#include <comdef.h>
#include <algorithm>

class PrinterManager
{
public:
    // 获取打印机列表
    static std::vector<std::string> listPrinters() ;

    // 打印PDF文件（默认打印机或指定打印机）
    static bool printPdfWithPDFtoPrinter(const std::string& filePathUtf8, const std::string& printerNameUtf8  = "") ;

    static bool setDefaultPrinter(const std::string& printerNameUtf8);

    static bool printExcelFile(const std::string& filePathUtf8, int copies, int paperSize);

    static bool printWordFile(const std::string& filePathUtf8, int copies);

    static bool printOfficeFile(const std::string& filePathUtf8, int copies, int paperSize);
};
#else
#endif

#endif // PRINTERMANAGER_H
