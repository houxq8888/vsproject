#ifndef HGTXT_H
#define HGTXT_H

#include <vector>
#include <map>
#include <string>

namespace HGMACHINE{
    int saveTableToTxt(const std::vector<std::map<std::string,std::string>> & content,
            const std::string& savename);
}

#endif // HGTXT_H