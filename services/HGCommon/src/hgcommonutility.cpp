#include "hgcommonutility.h"
#include "HGExactTime.h"
#ifdef __WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <iphlpapi.h>
#include <winsock2.h>
#include <tchar.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#include <alsa/asoundlib.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif
#include <memory>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <cstring>

namespace HGMACHINE {

int HGMkDir(std::string path)
{
    int ret=-1;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    if (_access(path.c_str(), 0) == -1) {
        ret = _mkdir(path.c_str());
    } else ret = 0;
#else
    if (access(path.c_str(), 0) == -1) {
        ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else ret = 0;
#endif
    return ret;
}

enum PasswordStrength{
    WEAK,
    MEDIUM,
    STRONG
};
std::vector<std::string> splitStr(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    size_t start = 0, end;

    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + 1; // 跳过分隔符
    }
    result.push_back(str.substr(start)); // 最后一个子串
    return result;
}
PasswordStrength checkPasswordStrength(const std::string& password){
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : password){
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }
    int typeCount = hasUpper + hasLower + hasDigit + hasSpecial;
    int length = password.length();

    if (length >= 12 && typeCount == 4)
        return STRONG;
    else if (length >= 8 && typeCount >= 2)
        return MEDIUM;
    else 
        return WEAK;
}

std::string checkPasswordStrengthToString(const std::string& password) {
    PasswordStrength level = checkPasswordStrength(password);
    switch (level){
        case STRONG:
            return "strong";
        case MEDIUM:
            return "medium";
        case WEAK:
            return "weak";
        default: return "unknown";
    }
}
std::string getStandardCurTime(){
    std::string slice;
    HGExactTime exact_time = HGExactTime::currentTime();
    slice = exact_time.toStandardString();
    return slice;
}
void decodeStandardTime(const std::string &time, TIME_STRUECT &timeS)
{
    std::string createTimeStr = time;
    size_t lastSemicolon = createTimeStr.find_last_of(';');
    if (lastSemicolon != std::string::npos)
    {
        createTimeStr = createTimeStr.substr(lastSemicolon + 1);
    }
    size_t slashPos = createTimeStr.find('_');
    if (slashPos == std::string::npos)
    {
        printf("Invalid format: missing '_'");
        return;
    }
    std::string datePart = createTimeStr.substr(0, slashPos);
    std::string timePart = createTimeStr.substr(slashPos + 1);

    std::vector<int> dateParts;
    std::istringstream dateStream(datePart);
    std::string token;

    while (std::getline(dateStream, token, '-'))
    {
        dateParts.push_back(std::stoi(token));
    }
    if (dateParts.size() != 3)
    {
        printf("Invalid date format");
        return;
    }
    int year = dateParts[0];
    int month = dateParts[1];
    int day = dateParts[2];
    timeS.year = year;
    timeS.month = month;
    timeS.day = day;
}
std::string HGGetTimeSlice()
{
    std::string slice;
    HGExactTime exact_time = HGExactTime::currentTime();
    slice = exact_time.toString();
    return slice;
}
void HGGetFilesNoBytes(const std::string &dirname, const std::string &prefix, std::vector<FileInfo> &filelist) {
    if (dirname.empty()) return;

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::string searchPath = dirname + "\\*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "无法打开目录: " << dirname << std::endl;
        return;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        std::string fileName = findData.cFileName;
        std::string filePath = dirname + "\\" + fileName;

        struct __stat64 fileStat;
        if (_stat64(filePath.c_str(), &fileStat) == 0) {
            if (fileStat.st_size > 0 && filePath.find(prefix) != std::string::npos) {
                filelist.push_back({filePath, fileStat.st_ctime});
            }
        }

    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);

#else
    DIR* dir = opendir(dirname.c_str());
    if (!dir) {
        perror("无法打开目录");
        return;
    }

    struct dirent* entry;
    struct stat fileStat;

    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        std::string filePath = dirname + "/" + fileName;

        if (stat(filePath.c_str(), &fileStat) == 0) {
            if (S_ISREG(fileStat.st_mode) && fileStat.st_size > 0) {
                if (filePath.find(prefix) != std::string::npos) {
                    filelist.push_back({filePath, fileStat.st_ctime});
                }
            }
        }
    }

    closedir(dir);
#endif
}
void HGGetFiles(const std::string dirname, const std::string prefix, std::vector<std::string> &filelist)
{
    if(dirname.empty())
        return;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    __int64 hFile = 0;
    // 文件信息
    struct __finddata64_t fileinfo;
    std::string p;
    if ((hFile = _findfirst64(p.assign(dirname).append("\\*").c_str(), &fileinfo)) != -1) {
        while (_findnext64(hFile, &fileinfo) == 0)
        {
            if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
            {
                continue;
            }
            string innerDir = p.assign(dirname).append("/").append(fileinfo.name);
            // 如果是目录，判断是否为config
            if ((fileinfo.attrib & _A_SUBDIR)) {
                HGGetFiles(innerDir, prefix, filelist);
                continue;
            } else {
                std::string pathname = fileinfo.name;
                std::string curFileExtName = (pathname.substr(pathname.find_last_of('.'))).c_str();
                if (curFileExtName != "" && (strcmp(curFileExtName.c_str(), prefix.c_str()) == 0))
                {
                    filelist.push_back(p.assign(dirname).append("/").append(fileinfo.name));
                }
            }
        } ;
        _findclose(hFile);
    }
#else
    struct stat s;
    stat(dirname.c_str(), &s);
    if(!S_ISDIR(s.st_mode))
        return;
    DIR *dirhand = opendir(dirname.c_str());
    if(NULL == dirhand){
        return;
    }
    dirent *fp = nullptr;
    while((fp = readdir(dirhand)) != nullptr){
        if(fp->d_name[0] != '.'){
            std::string filename = dirname + "/" + std::string(fp->d_name);
            struct stat filemod;
            stat(filename.c_str(), &filemod);
            if(S_ISDIR(filemod.st_mode)){
                HGGetFiles(filename,prefix,filelist);
            } else {
                char* curFileExtName=NULL;
                curFileExtName = rindex(fp->d_name, '.');
                if (curFileExtName != NULL && (strcmp(curFileExtName, prefix.c_str()) == 0))
                {
                    filelist.push_back(filename);
                }
            }
        }
    }
    closedir(dirhand);
#endif
    return;
}
HGExactTime HGGetTime()
{
    HGExactTime exact_time = HGExactTime::currentTime();
    return exact_time;
}
double ChangeTimeUnitToMS(HGExactTime time)
{
    int minToSec = 60;
    int hourToMin = 60;
    int dayToHour = 24;
    int monthToDay = 30;
    int yearToMonth = 12;
    double elapsedDTime = 0.0;
    elapsedDTime = (time.tm_year * yearToMonth * monthToDay * dayToHour * hourToMin * minToSec + time.tm_mon * monthToDay * dayToHour * hourToMin * minToSec +
                          time.tm_mday * dayToHour * hourToMin * minToSec + time.tm_hour * hourToMin * minToSec + time.tm_min * minToSec + time.tm_sec +
                           time.tm_millisec / 1000.0 + time.tm_microsec / 1000000.0);
    return elapsedDTime;
}
double HGCalTimeElapsed(HGExactTime start, HGExactTime end)
{ // ms
    // change to unit sec
    double elapsedEnd = ChangeTimeUnitToMS(end);
    double elapsedStart = ChangeTimeUnitToMS(start);
    double elapsed = (elapsedEnd - elapsedStart) * 1000; // millisec
    return elapsed;
}
int setCpu(int i)
{
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int cpu_nums = si.dwNumberOfProcessors;
    if (i >= cpu_nums) i = 0;
    SetThreadAffinityMask(GetCurrentThread(), i+1);

#else
    int cpu_nums = sysconf(_SC_NPROCESSORS_CONF);
    if (i >= cpu_nums) i = 0;
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(i, &mask);
    if (-1 == pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask))
    {
        return -1;
    }
    return 0;
#endif
    return 0;
}

bool isFileExist(std::string path)
{
    bool valid = false;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    valid = (_access(path.c_str(), 0) != -1) ? true:false;
#else
    valid = (access(path.c_str(), 0) != -1) ? true:false;
#endif
    return valid;
}

std::map<std::string,std::string> getParamMap(const std::string& param){
    char comsic=':';
    char dot=',';
    std::map<char,std::vector<size_t>> positions;
    positions[comsic]=std::vector<size_t>();
    positions[dot]=std::vector<size_t>();
    for (auto pos : positions){
        size_t position=param.find(pos.first);
        while (position != std::string::npos){
            positions[pos.first].push_back(position);
            position=param.find(pos.first,position+1);
        }
    }
    std::map<std::string,std::string> wparam;
    if (!positions.empty()){
        for (int i=0;i<int(positions[comsic].size());i++){
            std::string name,value;
            int namestart=(i<=0 ? 0:positions[dot][i-1]+1);
            int nameend=positions[comsic][i]-1;
            if (nameend<=namestart) name="";
            name=param.substr(namestart,nameend-namestart+1);
            int valuestart=positions[comsic][i]+1;
            int valueend=(i >= int(positions[dot].size()) ? param.length()-1:positions[dot][i]-1);
            if (valueend<valuestart) value="";
            else value=param.substr(valuestart,valueend-valuestart+1);
            wparam[name]=value;
        }
    } else {printf("empty\n");}
    return wparam;
}
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
std::vector<std::string> listResolutions() {
    DEVMODE dm;
    int modeIndex = 0;

    while (EnumDisplaySettings(NULL, modeIndex, &dm)) {
        std::cout << "Width: " << dm.dmPelsWidth
                  << ", Height: " << dm.dmPelsHeight
                  << ", Refresh Rate: " << dm.dmDisplayFrequency << " Hz"
                  << std::endl;
        modeIndex++;
    }
}
#elif __linux__
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
std::vector<std::string> listResolutions(std::string &deviceName) {
    std::vector<std::string> resolutions;
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Cannot open display!" << std::endl;
        return resolutions;
    }

    Window root = DefaultRootWindow(display);
    XRRScreenResources* screenRes = XRRGetScreenResources(display, root);
    if (!screenRes) {
        std::cerr << "Error: Cannot get screen resources!" << std::endl;
        XCloseDisplay(display);
        return resolutions;
    }

    // std::cout << "Number of outputs: " << screenRes->noutput << std::endl;

    for (int i = 0; i < screenRes->noutput; ++i) {
        XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenRes, screenRes->outputs[i]);
        if (!outputInfo) {
            std::cerr << "Error: Cannot get output info for output " << i << std::endl;
            continue;
        }

        // std::cout << "Output: " << outputInfo->name << std::endl;
        if (outputInfo->connection == RR_Connected) {
            deviceName=outputInfo->name;
            // std::cout << "  Connection: Connected" << std::endl;
        } else {
            // std::cout << "  Connection: Disconnected" << std::endl;
        }

        if (outputInfo->crtc != 0) {
            XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(display, screenRes, outputInfo->crtc);
            if (crtcInfo) {
                // std::cout << "  Resolution: " << crtcInfo->width << "x" << crtcInfo->height
                        //   << " at (" << crtcInfo->x << ", " << crtcInfo->y << ")" << std::endl;
                XRRFreeCrtcInfo(crtcInfo);
                resolutions.push_back(std::to_string(crtcInfo->width) + "x" + std::to_string(crtcInfo->height));
            } else {
                std::cout << "  Error: Cannot get CRTC info" << std::endl;
            }
        } else {
            // std::cout << "  No CRTC assigned" << std::endl;
        }

        XRRFreeOutputInfo(outputInfo);
    }

    XRRFreeScreenResources(screenRes);
    XCloseDisplay(display);
    return resolutions;
}
#else
std::vector<std::string> listResolutions() {
    std::cerr << "Unsupported platform!" << std::endl;
}
#endif



std::vector<WifiNetwork> scanWifiNetworks() {
    std::vector<WifiNetwork> networks;

    // 调用 nmcli 扫描无线网络
    std::string command = "nmcli -t -f SSID,SIGNAL,SECURITY device wifi";
    FILE* pipe = popen(command.c_str(), "r");

    if (!pipe) {
        std::cerr << "Error executing nmcli command!" << std::endl;
        return networks;
    }

    // 读取命令输出
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string line(buffer);
        std::string ssid = line.substr(0, line.find(":"));
        line = line.substr(line.find(":") + 1);
        std::string signal_strength = line.substr(0, line.find(":"));
        std::string security = line.substr(line.find(":") + 1);

        // 创建 WifiNetwork 对象并加入网络列表
        networks.push_back(WifiNetwork(ssid, signal_strength, security));
    }

    fclose(pipe);

    return networks;
}

// 读取环境光强度
int readAmbientLight() {
    std::ifstream sensor_file("/sys/class/ambient_light/light_level");
    if (!sensor_file) {
        std::cerr << "Failed to open ambient light sensor file!" << std::endl;
        return -1;
    }

    int light_level;
    sensor_file >> light_level;
    return light_level;
}

// 获取电池电量
int getBatteryPercentage() {
    std::ifstream battery_file("/sys/class/power_supply/BAT0/capacity");
    if (!battery_file) {
        std::cerr << "Failed to open battery capacity file!" << std::endl;
        return -1;
    }

    int battery_percentage;
    battery_file >> battery_percentage;
    return battery_percentage;
}

// 设置屏幕亮度
bool setBrightness(int level) {
    std::ofstream brightness_file("/sys/class/backlight/intel_backlight/brightness");
    if (!brightness_file) {
        std::cerr << "Failed to open brightness file!" << std::endl;
        return false;
    }

    brightness_file << level;
    return true;
}

// 计算亮度调整级别
int calculateBrightness(int light_level, int battery_percentage) {
    // 基于环境光和电池电量计算亮度
    int brightness = 0;

    // 光强较弱时，减少亮度
    if (light_level < 100) {
        brightness = 30;
    } else if (light_level < 200) {
        brightness = 60;
    } else {
        brightness = 100;
    }

    // 如果电池电量较低，则调低亮度
    if (battery_percentage < 20) {
        brightness = std::max(brightness - 30, 30);  // 电池电量低于20%时，降低亮度
    }

    return brightness;
}
void setTimezone(const std::string& timezone) {
    std::string command = "sudo timedatectl set-timezone " + timezone;
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Timezone successfully changed to " << timezone << std::endl;
    } else {
        std::cerr << "Failed to set timezone." << std::endl;
    }
}

void setVolume(long volume) {
    // snd_mixer_t* handle;
    // snd_mixer_selem_id_t* sid;
    // const char* card = "default"; // 声卡名称，通常是 "default"
    // const char* selem_name = "Master"; // 通道名称，通常是 "Master"

    // // 打开混音器
    // if (snd_mixer_open(&handle, 0) < 0) {
    //     std::cerr << "Failed to open mixer" << std::endl;
    //     return;
    // }

    // // 加载混音器控制
    // if (snd_mixer_attach(handle, card) < 0 ||
    //     snd_mixer_selem_register(handle, nullptr, nullptr) < 0 ||
    //     snd_mixer_load(handle) < 0) {
    //     std::cerr << "Failed to attach mixer" << std::endl;
    //     snd_mixer_close(handle);
    //     return;
    // }

    // // 分配简单元素 ID
    // snd_mixer_selem_id_malloc(&sid);
    // snd_mixer_selem_id_set_index(sid, 0);
    // snd_mixer_selem_id_set_name(sid, selem_name);

    // // 获取简单元素
    // snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);
    // if (!elem) {
    //     std::cerr << "Failed to find mixer element" << std::endl;
    //     snd_mixer_close(handle);
    //     snd_mixer_selem_id_free(sid);
    //     return;
    // }

    // // 设置音量
    // if (snd_mixer_selem_set_playback_volume_all(elem, volume) < 0) {
    //     std::cerr << "Failed to set volume" << std::endl;
    // }

    // // 释放资源
    // snd_mixer_close(handle);
    // snd_mixer_selem_id_free(sid);
}
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
// 播放 PCM 数据
void playSound(const std::string& filename) {
    // std::ifstream file(filename, std::ios::binary);
    // if (!file) {
    //     std::cerr << "Failed to open sound file" << std::endl;
    //     return;
    // }

    // // 设置 ALSA PCM
    // snd_pcm_t* pcm_handle=nullptr;
    // snd_pcm_hw_params_t* params=nullptr;
    // unsigned int rate = 44100;  // 采样率
    // int channels = 2;  // 声道数
    // int buffer_size = 4096;  // 缓冲区大小
    // snd_pcm_uframes_t frames = buffer_size / (sizeof(int16_t) * channels);

    // // 打开 PCM 设备
    // if (snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
    //     std::cerr << "Failed to open PCM device" << std::endl;
    //     return;
    // }

    // // 设置硬件参数
    // if (snd_pcm_hw_params_malloc(&params) < 0)
    // {
    //     std::cerr << "Failed to allocate hardware parameters" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params_any(pcm_handle, params) < 0)
    // {
    //     std::cerr << "Failed to initialize hardware parameters" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
    // {
    //     std::cerr << "Failed to set access type" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE) < 0)
    // {
    //     std::cerr << "Failed to set sample format" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, nullptr) < 0)
    // {
    //     std::cerr << "Failed to set sample rate" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0)
    // {
    //     std::cerr << "Failed to set channel count" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }

    // if (snd_pcm_hw_params(pcm_handle, params) < 0)
    // {
    //     std::cerr << "Failed to apply hardware parameters" << std::endl;
    //     if (params)
    //     {
    //         snd_pcm_hw_params_free(params);
    //     }
    //     if (pcm_handle)
    //     {
    //         snd_pcm_close(pcm_handle);
    //     }
    // }
    // snd_pcm_hw_params_free(params);
    // params=nullptr;

    // // 使用智能指针管理缓冲区
    // char* buffer = new char[buffer_size];

    // while (true) {
    //     file.read(buffer, buffer_size);
    //     ssize_t bytes_read = file.gcount();
    //     if (bytes_read <= 0) break;

    //     frames = bytes_read / (sizeof(int16_t) * channels);
    //     int err = snd_pcm_writei(pcm_handle, buffer, frames);
    //     if (err < 0)
    //     {
    //         std::cerr << "Error in snd_pcm_writei: " << snd_strerror(err) << std::endl;
    //         if (err == -EPIPE)
    //         {
    //             snd_pcm_prepare(pcm_handle); // 处理缓冲区溢出
    //         }
    //     }
    //     // if (snd_pcm_writei(pcm_handle, buffer, frames) < 0) {
    //     //     std::cerr << "Failed to write to PCM device" << std::endl;
    //     //     break;
    //     // }
    // }

    // if (params) {
    //     snd_pcm_hw_params_free(params);
    // }
    // if (pcm_handle) {
    //     snd_pcm_close(pcm_handle);
    // }
}
int countInstances(const std::string& appName) {
    int count = 0;

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot." << std::endl;
        return 0;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            // 将 ANSI 字符串转换为 std::wstring
            std::wstring exeW;
            int length = MultiByteToWideChar(CP_ACP, 0, pe.szExeFile, -1, nullptr, 0);
            if (length > 0)
            {
                exeW.resize(length - 1);  // 为了去掉结尾的 null 字符
                MultiByteToWideChar(CP_ACP, 0, pe.szExeFile, -1, &exeW[0], length);
            }

            std::string exe(exeW.begin(), exeW.end());  // 转换回 std::string

            std::string exeName(exe.begin(), exe.end());
            if (exeName.find(appName) != std::string::npos) {
                count++;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);

#else
    DIR* procDir = opendir("/proc");
    if (!procDir) {
        std::cerr << "Failed to open /proc: " << strerror(errno) << std::endl;
        return 0;
    }

    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;

        std::string cmdlinePath = std::string("/proc/") + entry->d_name + "/cmdline";
        std::ifstream cmdFile(cmdlinePath);
        if (!cmdFile.is_open()) continue;

        std::string cmdline;
        std::getline(cmdFile, cmdline, '\0');
        if (cmdline.find(appName) != std::string::npos) {
            count++;
        }
    }

    closedir(procDir);
#endif

    return count;
}
// 检查是否已有父进程在运行
bool isParentRunning()
{
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // 读取父进程的 PID
    std::ifstream pid_file(PARENT_PID_FILE);
    if (pid_file)
    {
        DWORD existing_pid = GetCurrentProcessId();
        std::cout<<"existing_pid:"<<existing_pid<<std::endl;
        pid_file >> existing_pid;
        pid_file.close();

        // 打开进程并检查它是否存在
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, existing_pid);
        if (hProcess != NULL)
        {
            DWORD exitCode;
            if (GetExitCodeProcess(hProcess, &exitCode))
            {
                if (exitCode == STILL_ACTIVE)
                {
                    std::cout << "已有父进程在运行，PID: " << existing_pid << std::endl;
                    CloseHandle(hProcess);
                    return true;
                }
                else
                {
                    std::cout << "发现过期的父进程 PID 文件，删除后重新启动" << std::endl;
                    CloseHandle(hProcess);
                    remove(PARENT_PID_FILE);
                }
            }
        }
        else
        {
            std::cout << "无法打开父进程，删除 PID 文件" << std::endl;
            remove(PARENT_PID_FILE);
        }
    }
    return false;

#else
    // 读取父进程的 PID
    std::ifstream pid_file(PARENT_PID_FILE);
    if (pid_file)
    {
        pid_t existing_pid;
        pid_file >> existing_pid;
        pid_file.close();

        // 检查该进程是否仍然存活
        if (kill(existing_pid, 0) == 0)
        {
            std::cout << "已有父进程在运行，PID: " << existing_pid << std::endl;
            return true;
        }
        else
        {
            std::cout << "发现过期的父进程 PID 文件，删除后重新启动" << std::endl;
            remove(PARENT_PID_FILE);
        }
    }
    return false;
#endif
}
bool isProcessRunning(const std::string &processName)
{
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateToolhelp32Snapshot failed." << std::endl;
        return false;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe))
    {
        do
        {
            // 将 ANSI 字符串转换为 std::wstring
            std::wstring exeW;
            int length = MultiByteToWideChar(CP_ACP, 0, pe.szExeFile, -1, nullptr, 0);
            if (length > 0)
            {
                exeW.resize(length - 1);  // 为了去掉结尾的 null 字符
                MultiByteToWideChar(CP_ACP, 0, pe.szExeFile, -1, &exeW[0], length);
            }

            std::string exe(exeW.begin(), exeW.end());  // 转换回 std::string

            if (exe == processName)
            {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return false;

#else
    // Linux 代码部分保持不变
    DIR *dir = opendir("/proc");
    if (!dir)
        return false;

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (!isdigit(entry->d_name[0]))
            continue;

        std::string path = std::string("/proc/") + entry->d_name + "/comm";
        FILE *file = fopen(path.c_str(), "r");
        if (file)
        {
            char comm[256];
            if (fgets(comm, sizeof(comm), file))
            {
                comm[strcspn(comm, "\n")] = 0;
                if (processName == comm)
                {
                    fclose(file);
                    closedir(dir);
                    return true;
                }
            }
            fclose(file);
        }
    }

    closedir(dir);
    return false;
#endif
}

std::vector<std::map<std::string, std::string>> getWirelessIP() {
    std::vector<std::map<std::string, std::string>> ip_list;

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return ip_list;
    }

    // Get adapter information
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwSize = sizeof(adapterInfo);
    DWORD dwResult = GetAdaptersInfo(adapterInfo, &dwSize);
    if (dwResult != ERROR_SUCCESS) {
        std::cerr << "GetAdaptersInfo failed." << std::endl;
        WSACleanup();
        return ip_list;
    }

    // Iterate through adapter information
    for (PIP_ADAPTER_INFO pAdapterInfo = adapterInfo; pAdapterInfo != nullptr; pAdapterInfo = pAdapterInfo->Next) {
        std::string ifaceName = pAdapterInfo->AdapterName;
        std::string ip = pAdapterInfo->IpAddressList.IpAddress.String;

        // Check if the adapter is Wi-Fi based on its description
        if (pAdapterInfo->Description != nullptr) {
            std::string description(pAdapterInfo->Description);
            
            // Check if the description contains "Wi-Fi" (assuming Wi-Fi adapters have "Wi-Fi" in their description)
            if (description.find("Wi-Fi") != std::string::npos) {
                std::map<std::string, std::string> ipinfo;
                ipinfo[ifaceName] = ip;
                ip_list.push_back(ipinfo);
            }
        }
    }

    WSACleanup();
#else
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];
    
    // Get interface address information
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return ip_list;
    }

    // Iterate through interface information
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        // Only check IPv4 addresses
        if (ifa->ifa_addr->sa_family == AF_INET) {
            std::string ifaceName = ifa->ifa_name;

            // Filter interfaces starting with "wl" or "en" for wireless interfaces
            std::map<std::string, std::string> ipinfo;
            if (ifaceName.find("wl") == 0 || ifaceName.find("en") == 0) {
                struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
                inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);
                ipinfo[ifaceName] = ip;
                ip_list.push_back(ipinfo);
            }
        }
    }

    freeifaddrs(ifaddr);
#endif

    return ip_list;
}


}
