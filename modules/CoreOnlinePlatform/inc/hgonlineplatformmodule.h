#ifndef HGONLINEPLATFORM_MODULE_H
#define HGONLINEPLATFORM_MODULE_H

#include <map>
#include "hgxml.h"
#include "config.h"
#include "rwDb.h"

namespace HGMACHINE{  
    // #define ENABLE_CAS6

    
    
    #define ResultDBName "Result"
    

    enum {
        CREATE,
        LOAD
    };
    enum {
        TASK_WASH,
        TASK_INJECTREAGENT,
        TASK_BLANK,
        TASK_GETSAMPLE,
        TASK_DETECT,
        TASK_HOLDSAMPLE
    };
    enum {
        METHOD_COLORDD,
        METHOD_DIANWEIDD,
        METHOD_LIGTH,
        METHOD_RONGLIANG,
        METHOD_KULUN
    };

    extern std::map<int,std::string> taskWorkingMode;
    extern std::map<int,std::string> taskInterval;
    extern std::map<int,std::string> getSampleMode;
    extern std::map<int,std::string> washPaiYeMode;
    extern std::map<int,std::string> injectReagentMode;
    extern std::map<int,std::string> kulunMethodShift;
    extern std::map<int,std::string> targetType;

    extern std::vector<Task> taskSequence;

    class HgOnlinePlatformModule : public FileConfig
    {
    public:
        HgOnlinePlatformModule();
        ~HgOnlinePlatformModule();



    };
}
#endif