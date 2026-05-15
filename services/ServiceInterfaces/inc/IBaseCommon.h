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
    struct WifiNetwork {
        std::string ssid;
        std::string signal_strength;
        std::string security;

        WifiNetwork(const std::string& s, const std::string& ss, const std::string& sec)
            : ssid(s), signal_strength(ss), security(sec) {}
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
    
    virtual int CreateDirectory(const std::string& path) = 0;
    virtual bool DirectoryExists(const std::string& path) const = 0;
    virtual bool FileExists(const std::string& path) const = 0;
    
    virtual std::vector<std::string> ListFiles(const std::string& directory) = 0;
    virtual void GetFiles(const std::string& dirname, const std::string& prefix, std::vector<std::string>& filelist) = 0;
    virtual void GetFilesNoBytes(const std::string& dirname, const std::string& prefix, std::vector<ServiceInterfaces::FileInfo>& filelist) = 0;
    virtual bool DeleteFile(const std::string& path) = 0;
    virtual bool CopyFile(const std::string& source, const std::string& destination) = 0;
    
    virtual void SafeDelete(void* ptr) = 0;
    virtual std::string GetStandardCurTime() = 0;
    virtual void SetTimezone(const std::string& timezone) = 0; 
    virtual std::vector<std::string> ListResolutions(std::string &deviceName) = 0;
    virtual std::vector<ServiceInterfaces::WifiNetwork> ScanWifiNetworks() = 0;
    virtual void SetVolume(long volume) = 0;
    virtual void PlaySound(const std::string& filename) = 0;
    virtual std::string CheckPasswordStrengthToString(const std::string &password) = 0;
    virtual std::map<std::string, std::string> GetParamMap(const std::string& param) = 0;


    virtual std::vector<std::string> SplitString(const std::string& str, char delimiter) = 0;
    virtual std::string TrimString(const std::string& str) = 0;
    virtual std::string ToLowerString(const std::string& str) = 0;
    virtual std::string ToUpperString(const std::string& str) = 0;
    
    virtual bool GetUSBDevices(uint16_t vendorID, uint16_t productID) = 0;
};

#endif // I_BASE_COMMON_H
