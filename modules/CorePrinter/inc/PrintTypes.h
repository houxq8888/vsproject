#ifndef PRINTTYPES_H
#define PRINTTYPES_H

#include <string>

namespace HGMACHINE {

struct PrintInfo {
    std::string printerName;
    std::string filePath;
    int copyNum = 1;
    std::string pageSize;
    double dmPaperWidth = 0.0;
    double dmPaperHeight = 0.0;
};

}

#endif // PRINTTYPES_H
