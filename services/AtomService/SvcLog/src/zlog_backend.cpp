#include "zlog_backend.h"
#include "hgcommonutility.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <ctime>

namespace HGMACHINE {

ZlogBackend::ZlogBackend() : m_zlogCategory(nullptr), m_initialized(false) {
}

ZlogBackend::~ZlogBackend() {
    cleanupZlog();
}

void ZlogBackend::logInit(const std::string& logDir, LogOutput output) {
    if (m_logDir != logDir || m_output != output || !m_initialized) {
        m_logDir = logDir;
        m_output = output;
        initZlog(logDir, output);
    }
}

void ZlogBackend::logDeInit() {
    cleanupZlog();
}

void ZlogBackend::logInfo(const std::string& content) {
    if (m_initialized && m_zlogCategory) {
        // zlog_info would be used here if zlog was available
        // dzlog_info(content.c_str());
        printf("[INFO] %s\n", content.c_str());
    } else {
        printf("[INFO] %s\n", content.c_str());
    }
}

void ZlogBackend::logDebug(const std::string& content) {
    if (m_initialized && m_zlogCategory) {
        // zlog_debug would be used here if zlog was available
        // dzlog_debug(content.c_str());
        printf("[DEBUG] %s\n", content.c_str());
    } else {
        printf("[DEBUG] %s\n", content.c_str());
    }
}

void ZlogBackend::logError(const std::string& content) {
    if (m_initialized && m_zlogCategory) {
        // zlog_error would be used here if zlog was available
        // dzlog_error(content.c_str());
        printf("[ERROR] %s\n", content.c_str());
    } else {
        printf("[ERROR] %s\n", content.c_str());
    }
}

void ZlogBackend::logWarn(const std::string& content) {
    if (m_initialized && m_zlogCategory) {
        // zlog_warn would be used here if zlog was available
        // dzlog_warn(content.c_str());
        printf("[WARN] %s\n", content.c_str());
    } else {
        printf("[WARN] %s\n", content.c_str());
    }
}

void ZlogBackend::log(const std::string& content, LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            logInfo(content);
            break;
        case LogLevel::DEBUG:
            logDebug(content);
            break;
        case LogLevel::LOG_ERROR:
            logError(content);
            break;
        case LogLevel::WARN:
            logWarn(content);
            break;
    }
}

void ZlogBackend::initZlog(const std::string& logDir, LogOutput output) {
    // Create log directory if it doesn't exist
    int ret = HGMkDir(logDir);
    if (ret == 0) {
        std::ostringstream name;
        name << logDir << "mkdir success\n";
        printf(name.str().c_str());
    } else {
        std::ostringstream name;
        name << logDir << "mkdir failed\n";
        printf(name.str().c_str());
    }

    // Create a simple zlog configuration file
    std::string configFile = logDir + "/zlog.conf";
    std::ofstream config(configFile);
    if (config.is_open()) {
        config << "[formats]\n";
        config << "simple = \"%d %m%n\"\n";
        config << "\n";
        config << "[rules]\n";
        config << "default.* \"stdout\",simple\n";
        
        if (output == LogOutput::FILE || output == LogOutput::ALL) {
            std::string logFile = logDir + "/" + HGGetTimeSlice() + ".log";
            config << "default.* \"" << logFile << "\",simple\n";
        }
        
        config.close();
    }

    // Initialize zlog
    // In a real implementation, we would call:
    // if (dzlog_init(configFile.c_str(), "default") == 0) {
    //     m_zlogCategory = dzlog_get_category();
    //     m_initialized = true;
    // }
    
    // For now, we'll simulate initialization
    m_initialized = true;
    printf("Zlog backend initialized with config: %s\n", configFile.c_str());
}

void ZlogBackend::cleanupZlog() {
    if (m_initialized) {
        // In a real implementation, we would call:
        // zlog_fini();
        m_initialized = false;
        m_zlogCategory = nullptr;
        printf("Zlog backend cleaned up\n");
    }
}

}
