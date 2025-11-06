#ifndef HGCSV_H
#define HGCSV_H

#include <string>
#include <vector>
#include <map>

namespace HGMACHINE{
    int saveTableToCsv(const std::vector<std::map<std::string,std::string>> & content,
        const std::string& savename);
}

#endif // HGCSV_H