#include "hglog4cplus.h"
#include "hgcommonutility.h"

namespace HGMACHINE {

    #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else

    HGLog4Cplus::HGLog4Cplus(const std::string &logDir)
    {
        std::ostringstream logtext;
        //  创建log
        int ret=HGMkDir(logDir);
        if (ret == 0) {
            std::ostringstream name;
            name << logDir << "mkdir success\n";
            printf(name.str().c_str());
        } else {
            std::ostringstream name;
            name << logDir << "mkdir failed\n";
            printf(name.str().c_str());
        }
        std::ostringstream logpath;
        logpath<<logDir<<"/"<<HGGetTimeSlice()<<".log";

        /* step1: Instantiate an appender object */
       // SharedObjectPtr<Appender> appendPtr(new ConsoleAppender());// 输出到控制台
        //SharedAppenderPtr appendPtr(new FileAppender(logpath.str())); // 输出到文件

        SharedAppenderPtr appendPtr(new DailyRollingFileAppender(LOG4CPLUS_STRING_TO_TSTRING(logpath.str()), HOURLY, true));
       // SharedAppenderPtr appendPtr(new RollingFileAppender(logpath.str(), 10*1024*1024));
        appendPtr->setName(LOG4CPLUS_TEXT("filelogtest"));

        /* step2: Instantiate a layout object */
        std::string pattern = "%D{%Y/%m/%d %X-%Q}->%m%n";
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        // Create a layout
        std::auto_ptr<Layout> layout(new PatternLayout(LOG4CPLUS_STRING_TO_TSTRING(pattern)));

        /* step3: Attach the layout object to the appender*/
        appendPtr->setLayout(layout);

        #else
        /* step3: Attach the layout object to the appender*/
        appendPtr->setLayout(std::unique_ptr<Layout>(new PatternLayout(LOG4CPLUS_STRING_TO_TSTRING(pattern))));
        #endif

        /* step4: Instantiate a logger object*/
        m_logger = Logger::getInstance(LOG4CPLUS_TEXT("test.subtestof_filelog"));

        /* step5: Attach the appender object to the logger */
        m_logger.addAppender(appendPtr);

        /* step6: Set a priority for the logger */
        m_logger.setLogLevel(ALL_LOG_LEVEL);
    }
    HGLog4Cplus::~HGLog4Cplus()
    {
        printf("release log\n");
    }
    HGLog4Cplus* HGLog4Cplus::getLogInstance(const std::string &logDir)
    {
        static HGLog4Cplus obj(logDir);
        return (&obj);
    }
    void HGLog4Cplus::logout(std::string content,int type)
    {
        switch (type) {
        case LOGINFO:{
            LOG4CPLUS_INFO(m_logger, content.c_str());
            break;
        }
        case LOGDEBUG:{
            LOG4CPLUS_DEBUG(m_logger,content.c_str());
            break;
        }
        case LOGERROR:{
            LOG4CPLUS_ERROR(m_logger,content.c_str());
            break;
        }
        case LOGWARN:{
            LOG4CPLUS_WARN(m_logger,content.c_str());
            break;
        } default:break;
        }
    }
#endif
}