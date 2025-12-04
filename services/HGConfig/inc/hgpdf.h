#ifndef HGPDF_H
#define HGPDF_H

#include <hpdf.h>
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE{
    int generatePDF(const std::string& filename, const std::string& content);
    int saveTableToPdf(const std::vector<std::map<std::string,std::string>> & content,
        const std::string& savename, const std::string& charset);

    void generateZHPDF(const std::string& filename,
        const std::string &font);
}

#endif // HGPDF_H