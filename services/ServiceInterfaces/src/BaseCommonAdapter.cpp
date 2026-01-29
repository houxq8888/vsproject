#include "BaseCommonAdapter.h"
#include "hgcommonutility.h"
#include "HGExactTime.h"
#include "systemusage.h"
#include "HGCommonTypes.h"

using namespace HGMACHINE;

BaseCommonAdapter::BaseCommonAdapter() {}

std::string BaseCommonAdapter::GetVersion() const {
    return "1.0.0";
}

std::string BaseCommonAdapter::GetBuildTime() {
    return __DATE__ " " __TIME__;
}

std::string BaseCommonAdapter::GetAppDataPath() const {
    return "";
}

std::string BaseCommonAdapter::GetTempPath() const {
    return "";
}

std::string BaseCommonAdapter::GetConfigPath() const {
    return "";
}

int BaseCommonAdapter::CreateDirectory(const std::string& path) {
    return HGMkDir(path);
}

bool BaseCommonAdapter::DirectoryExists(const std::string& path) const {
    return isFileExist(path);
}
bool BaseCommonAdapter::FileExists(const std::string& path) const {
    return isFileExist(path);
}

std::vector<std::string> BaseCommonAdapter::ListFiles(const std::string& directory) {
    std::vector<std::string> filelist;
    HGGetFiles(directory, "", filelist);
    return filelist;
}

void BaseCommonAdapter::GetFiles(const std::string& dirname, const std::string& prefix, std::vector<std::string>& filelist) {
    HGGetFiles(dirname, prefix, filelist);
}

void BaseCommonAdapter::GetFilesNoBytes(const std::string& dirname, const std::string& prefix, std::vector<ServiceInterfaces::FileInfo>& filelist) {
    std::vector<HGMACHINE::FileInfo> baseFileList;
    HGMACHINE::HGGetFilesNoBytes(dirname, prefix, baseFileList);
    
    filelist.clear();
    for (const auto& file : baseFileList) {
        ServiceInterfaces::FileInfo fileInfo;
        fileInfo.filename = file.filename;
        fileInfo.createtime = file.createtime;
        filelist.push_back(fileInfo);
    }
}

bool BaseCommonAdapter::DeleteFile(const std::string& path) {
    return removeFile(path);
}

bool BaseCommonAdapter::CopyFile(const std::string& source, const std::string& destination) {
    return false;
}

std::vector<std::string> BaseCommonAdapter::ListResolutions(std::string &deviceName) {
    return HGMACHINE::listResolutions(deviceName);
}
void BaseCommonAdapter::SetTimezone(const std::string& timezone) {
    HGMACHINE::setTimezone(timezone);
}
std::string BaseCommonAdapter::GetStandardCurTime()  {
    return HGMACHINE::getStandardCurTime();
}

std::vector<ServiceInterfaces::WifiNetwork> BaseCommonAdapter::ScanWifiNetworks() {
    std::vector<ServiceInterfaces::WifiNetwork> wifiNetWorks;
    std::vector<HGMACHINE::WifiNetwork> hgnetworks= HGMACHINE::scanWifiNetworks();
    for (const auto& hgnetwork : hgnetworks) {
        wifiNetWorks.push_back(ServiceInterfaces::WifiNetwork(hgnetwork.ssid, hgnetwork.signal_strength, hgnetwork.security));
    }
    return wifiNetWorks;
}
void BaseCommonAdapter::SafeDelete(void* ptr) {
    SAFE_DELETE(ptr);
}

std::string BaseCommonAdapter::CheckPasswordStrengthToString(const std::string &password) {
    return HGMACHINE::checkPasswordStrengthToString(password);
}
void BaseCommonAdapter::SetVolume(long volume) {
    HGMACHINE::setVolume(volume);
}
void BaseCommonAdapter::PlaySound(const std::string& filename) {
    HGMACHINE::playSound(filename);
}
std::vector<std::string> BaseCommonAdapter::SplitString(const std::string& str, char delimiter) {
    return splitStr(str, delimiter);
}

std::string BaseCommonAdapter::TrimString(const std::string& str) {
    std::string result = str;
    size_t start = result.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        result = result.substr(start);
    }
    size_t end = result.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) {
        result = result.substr(0, end + 1);
    }
    return result;
}

std::string BaseCommonAdapter::ToLowerString(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string BaseCommonAdapter::ToUpperString(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
std::map<std::string, std::string> BaseCommonAdapter::GetParamMap(const std::string& param) {
    return HGMACHINE::getParamMap(param);
}

bool BaseCommonAdapter::GetUSBDevices(uint16_t vendorID, uint16_t productID) {
    return SystemUsage::getUSBDevices(vendorID, productID);
}
    