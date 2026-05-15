#ifndef HGINTERFACETYPES_H
#define HGINTERFACETYPES_H

#include <string>
#include "HGErrorAdapter.h"

namespace HGMACHINE {

struct HGErrorDetail {
    int code;
    std::string category;
    std::string message;
    std::string timestamp;
    
    HGErrorDetail() : code(0) {}
    
    bool hasError() const {
        return code != 0;
    }
    
    std::string toString() const {
        return "[" + category + ":" + std::to_string(code) + "] " + message;
    }
};

}

#endif 
