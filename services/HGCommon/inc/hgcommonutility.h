#ifndef HGCOMMONUTILITY_H
#define HGCOMMONUTILITY_H

#include "HGCommon_global.h"
#include "HGExactTime.h"
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE {
#ifdef _WIN32
#define PARENT_PID_FILE "C:/parent.txt"
#define CHILD_PID_FILE "C:/child.txt"
#else
#define PARENT_PID_FILE "/tmp/parent.pid"
#define CHILD_PID_FILE "/tmp/child.pid"
#endif
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


std::vector<std::string> splitStr(const std::string &str, char delimiter);
std::string checkPasswordStrengthToString(const std::string &password);
int HGMkDir(std::string path);
std::string HGGetTimeSlice();
void HGGetFiles(const std::string dirname, const std::string prefix, std::vector<std::string> &filelist);
void HGGetFilesNoBytes(const std::string &dirname,const std::string &prefix,std::vector<FileInfo> &filelist);
HGExactTime HGGetTime();
double HGCalTimeElapsed(HGExactTime start, HGExactTime end);
int setCpu(int i);
bool isFileExist(std::string path);
std::map<std::string,std::string> getParamMap(const std::string& param);
std::vector<std::string> listResolutions(std::string &deviceName);
std::vector<WifiNetwork> scanWifiNetworks();
void setTimezone(const std::string& timezone);
void setVolume(long volume);
void playSound(const std::string& filename);
int countInstances(const std::string& appName);
bool isProcessRunning(const std::string& processName);
bool isParentRunning();
std::vector<std::map<std::string,std::string>> getWirelessIP();

std::string getStandardCurTime();
typedef struct tagTIME_STRUECT {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    tagTIME_STRUECT() {
        year = month = day = hour = minute = second = 0;
    }
}TIME_STRUECT;

void decodeStandardTime(const std::string& time, TIME_STRUECT& timeS);


}
#ifdef __cplusplus
}
#endif

#endif // HGCOMMONUTILITY_H
