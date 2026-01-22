#ifndef I_BASE_COMMON_H
#define I_BASE_COMMON_H

#include <string>
#include <vector>
#include <ctime>
#include <memory>

namespace ServiceInterfaces {
    struct FileInfo {
        std::string filename;
        std::time_t createtime;
    };
}

class IBaseCommon {
public:
    virtual ~IBaseCommon() = default;

    virtual std::string GetVersion() const = 0;
    virtual std::string GetBuildTime() = 0;
    
    virtual std::string GetAppDataPath() const = 0;
    virtual std::string GetTempPath() const = 0;
    virtual std::string GetConfigPath() const = 0;
    
    virtual bool CreateDirectory(const std::string& path) = 0;
    virtual bool DirectoryExists(const std::string& path) const = 0;
    virtual bool FileExists(const std::string& path) const = 0;
    
    virtual std::vector<std::string> ListFiles(const std::string& directory) = 0;
    virtual void GetFiles(const std::string& dirname, const std::string& prefix, std::vector<std::string>& filelist) = 0;
    virtual void GetFilesNoBytes(const std::string& dirname, const std::string& prefix, std::vector<ServiceInterfaces::FileInfo>& filelist) = 0;
    virtual bool DeleteFile(const std::string& path) = 0;
    virtual bool CopyFile(const std::string& source, const std::string& destination) = 0;
    
    virtual void SafeDelete(void* ptr) = 0;
    
    virtual std::vector<std::string> SplitString(const std::string& str, char delimiter) = 0;
    virtual std::string TrimString(const std::string& str) = 0;
    virtual std::string ToLowerString(const std::string& str) = 0;
    virtual std::string ToUpperString(const std::string& str) = 0;
};

#endif // I_BASE_COMMON_H
