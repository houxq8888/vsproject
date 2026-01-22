#ifndef BASE_COMMON_ADAPTER_H
#define BASE_COMMON_ADAPTER_H

#include "IBaseCommon.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT BaseCommonAdapter : public IBaseCommon {
public:
    BaseCommonAdapter();
    virtual ~BaseCommonAdapter() = default;

    std::string GetVersion() const override;
    std::string GetBuildTime() override;
    
    std::string GetAppDataPath() const override;
    std::string GetTempPath() const override;
    std::string GetConfigPath() const override;
    
    bool CreateDirectory(const std::string& path) override;
    bool DirectoryExists(const std::string& path) const override;
    bool FileExists(const std::string& path) const override;
    
    std::vector<std::string> ListFiles(const std::string& directory) override;
    void GetFiles(const std::string& dirname, const std::string& prefix, std::vector<std::string>& filelist) override;
    void GetFilesNoBytes(const std::string& dirname, const std::string& prefix, std::vector<ServiceInterfaces::FileInfo>& filelist) override;
    bool DeleteFile(const std::string& path) override;
    bool CopyFile(const std::string& source, const std::string& destination) override;
    
    void SafeDelete(void* ptr) override;
    
    
    std::vector<std::string> SplitString(const std::string& str, char delimiter) override;
    std::string TrimString(const std::string& str) override;
    std::string ToLowerString(const std::string& str) override;
    std::string ToUpperString(const std::string& str) override;
    
};

#endif // BASE_COMMON_ADAPTER_H
