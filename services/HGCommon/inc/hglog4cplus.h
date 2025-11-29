#ifndef HGLOG4CPLUS_H
#define HGLOG4CPLUS_H

#include "HGCommon_global.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

enum {
    LOGINFO,
    LOGDEBUG,
    LOGERROR,
    LOGWARN
};
#define LOG_PATH "/app/log"

namespace HGMACHINE {
class HGCOMMON_EXPORT HGLog4Cplus
{
public:
    HGLog4Cplus(const std::string &logDir);
    ~HGLog4Cplus();
    static HGLog4Cplus* getLogInstance(const std::string &logDir);

    void logout(std::string content,int type);
private:
    Logger m_logger;
};
}


#endif
#endif // HGLOG4CPLUS_H
