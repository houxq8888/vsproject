#ifndef FILE_BACKEND_H
#define FILE_BACKEND_H

#include "log_backend_interface.h"
#include <string>
#include <fstream>
#include <mutex>

namespace HGMACHINE {

class FileBackend : public LogBackendInterface {
public:
    FileBackend();
    ~FileBackend() override;

    void logInit(const std::string& logDir, LogOutput output) override;
    void logDeInit() override;

    void logInfo(const std::string& content) override;
    void logDebug(const std::string& content) override;
    void logError(const std::string& content) override;
    void logWarn(const std::string& content) override;

    void log(const std::string& content, LogLevel level) override;

private:
    std::ofstream m_logFile;
    std::string m_logDir;
    std::string m_logFilePath;
    LogOutput m_output;
    bool m_initialized;
    std::mutex m_mutex;

    void openLogFile();
    void closeLogFile();
    std::string getCurrentTime();
    std::string levelToString(LogLevel level);
};

}

#endif // FILE_BACKEND_H
