#ifndef HGPRINTER_H
#define HGPRINTER_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#include <winspool.h>
#include <shellapi.h>
#include <algorithm>
#else
#include <cups/cups.h>
#endif

#include <vector>
#include <string>
#include <iostream>

namespace HGMACHINE{

typedef struct tagPrintInfo{
    std::string printerName;
    std::string filePath;
    int copyNum = 1;
    std::string pageSize;
    double dmPaperWidth; // in mm
    double dmPaperHeight; // in mm
} PrintInfo;

std::vector<std::string> getPrinerList();
int printFile(const PrintInfo &printInfo);
}

#endif // HGPRINTER_H