#ifndef ZLOG_BACKEND_H
#define ZLOG_BACKEND_H

#include "log_backend_interface.h"
#include <string>

namespace HGMACHINE {

class ZlogBackend : public LogBackendInterface {
public:
    ZlogBackend();
    ~ZlogBackend() override;

    void logInit(const std::string& logDir, LogOutput output) override;
    void logDeInit() override;

    void logInfo(const std::string& content) override;
    void logDebug(const std::string& content) override;
    void logError(const std::string& content) override;
    void logWarn(const std::string& content) override;

    void log(const std::string& content, LogLevel level) override;

private:
    void* m_zlogCategory;
    std::string m_logDir;
    LogOutput m_output;
    bool m_initialized;

    void initZlog(const std::string& logDir, LogOutput output);
    void cleanupZlog();
};

}

#endif // ZLOG_BACKEND_H
