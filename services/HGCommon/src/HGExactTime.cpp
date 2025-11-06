/*
    @file   exacttime.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "HGExactTime.h"
#include <iostream>
#include <chrono>
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <ctime>
using namespace std::chrono;

HGExactTime::HGExactTime()
    : tm_millisec(0),
    tm_microsec(0)
{
}

HGExactTime::HGExactTime(const std::tm& _tm, int _tm_millisec, int _tm_microsec)
    : std::tm(_tm),
    tm_millisec(_tm_millisec),
    tm_microsec(_tm_microsec)
{
}

std::string HGExactTime::toString() const
{
    char temp[27]{ 0 };
    snprintf(temp,
             27,
             "%04d%02d%02d_%02d%02d%02d_%03d%03d",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec,
             tm_millisec,
             tm_microsec);
    return std::string(temp);
}
std::string HGExactTime::toStandardString() const
{
    char temp[27]{ 0 };
    snprintf(temp,
             27,
             "%04d-%02d-%02d_%02d:%02d:%02d",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec);
    return std::string(temp);
}
std::string HGExactTime::toStringFromYearToDay() const 
{
    char temp[27]{0};
    snprintf(temp,
             27,
             "%04d%02d%02d",
             tm_year,
             tm_mon,
             tm_mday);
    return std::string(temp);
}
std::string HGExactTime::toStringFromYearToSec() const
{
    char temp[27]{ 0 };
    snprintf(temp,
             27,
             "%04d%02d%02d%02d%02d%02d",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec/*,
             tm_millisec*/);
    return std::string(temp);
}
std::string HGExactTime::toStringFromYearToSecAndZone() const
{
    char temp[27]{ 0 };
    #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
    TIME_ZONE_INFORMATION tzi;
    GetTimeZoneInformation(&tzi);

    char tzName[64] = "Unknown";
    WideCharToMultiByte(CP_UTF8, 0, tzi.StandardName, -1, tzName, sizeof(tzName), nullptr, nullptr);

    snprintf(temp,
             27,
             "%04d-%02d-%02d_%02d:%02d:%02d %s",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec,
             tzName);
    #else
    snprintf(temp,
             27,
             "%04d-%02d-%02d_%02d:%02d:%02d %s",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec,
             tm_zone);
    #endif
    
    return std::string(temp);
}
std::string HGExactTime::toStringForFilename() const
{
    char temp[27]{ 0 };
    snprintf(temp,
             27,
             "%04d%02d%02d_%02d%02d%02d_%03d%03d",
             tm_year,
             tm_mon,
             tm_mday,
             tm_hour,
             tm_min,
             tm_sec,
             tm_millisec,
             tm_microsec);
    return std::string(temp);
}

HGExactTime HGExactTime::currentTime()
{
    system_clock::time_point time_point_now = system_clock::now(); // 获取当前时间点
    system_clock::duration duration_since_epoch
        = time_point_now.time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
    time_t microseconds_since_epoch
        = duration_cast<microseconds>(duration_since_epoch).count(); // 将时长转换为微秒数
    time_t seconds_since_epoch = microseconds_since_epoch / 1000000; // 将时长转换为秒数

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HGExactTime exact_time;
    localtime_s(&exact_time, &seconds_since_epoch);
    exact_time.tm_microsec = microseconds_since_epoch % 1000;
    exact_time.tm_millisec = microseconds_since_epoch / 1000 % 1000;
#else
    HGExactTime exact_time;
    localtime_r(&seconds_since_epoch, &exact_time);
    exact_time.tm_microsec = microseconds_since_epoch % 1000;
    exact_time.tm_millisec = microseconds_since_epoch / 1000 % 1000;
#endif

    exact_time.tm_year += 1900;
    exact_time.tm_mon +=1 ;
    return exact_time;
}
int HGExactTime::fasterThanThirtyMimutes(const std::string &timeStr){
    if (timeStr.size() != 14) return -1;
    int year=std::atoi(timeStr.substr(0, 4).c_str());
    int month=std::atoi(timeStr.substr(4, 2).c_str());
    int mday=std::atoi(timeStr.substr(6, 2).c_str());
    int hour=std::atoi(timeStr.substr(8, 2).c_str());
    int minute=std::atoi(timeStr.substr(10, 2).c_str());
    int second=std::atoi(timeStr.substr(12, 2).c_str());
    // 构造 tm 结构
    std::tm t = {};
    t.tm_year = year - 1900;  // tm_year 是从1900年开始
    t.tm_mon  = month - 1;    // tm_mon 从0开始
    t.tm_mday = mday;
    t.tm_hour = hour;
    t.tm_min  = minute;
    t.tm_sec  = second;

    // 转换为 time_t
    std::time_t inputTime = std::mktime(&t);
    if (inputTime == -1) return -1;  // 转换失败

    // 获取当前时间
    std::time_t now = std::time(nullptr);

    // 计算时间差（秒）
    double diff = std::difftime(now, inputTime);
    return int(diff); // /60 + 0.5);
}
HGExactTime& HGExactTime::operator+=(const int &value){
    this->tm_mday += value;
    while (tm_mday > 28)
    {
        if (tm_mon == 1 || tm_mon == 3 ||
            tm_mon == 5 || tm_mon == 7 ||
            tm_mon == 8 || tm_mon == 10)
        {
            if (tm_mday > 31)
            {
                tm_mon += 1;
                tm_mday -= 31;
            } else {
                break;
            }
        }
        else if (tm_mon == 4 || tm_mon == 6 ||
                 tm_mon == 9 || tm_mon == 11)
        {
            if (tm_mday > 30)
            {
                tm_mon += 1;
                tm_mday -= 30;
            } else {
                break;
            }
        }
        else if (tm_mon == 2)
        {
            if ((tm_year % 4 == 0 && tm_year % 100 != 0) || tm_year % 400 == 0)
            {
                if (tm_mday > 29)
                {
                    tm_mon += 1;
                    tm_mday -= 29;
                }
            }
            else
            {
                if (tm_mday > 28)
                {
                    tm_mon += 1;
                    tm_mday -= 28;
                }
            }
        } else if (tm_mon == 12){
            if (tm_mday > 31)
            {
                tm_year+=1;
                tm_mon=1;
                tm_mday -= 31;
            } else {
                break;
            }
        }
    }
    return *this;
}
bool HGExactTime::operator<(const HGExactTime &obj) const
{
    std::tm timeInfo = {};
    timeInfo.tm_year = obj.tm_year-1900; // 年
    timeInfo.tm_mon = obj.tm_mon-1;            // 月（0-11）
    timeInfo.tm_mday = obj.tm_mday;           // 日
    timeInfo.tm_hour = obj.tm_hour;          // 时
    timeInfo.tm_min = obj.tm_min;           // 分
    timeInfo.tm_sec = obj.tm_sec;            // 秒
    // 将 tm* 类型指针传递给 mktime
    std::time_t t2 = std::mktime(&timeInfo);
    std::tm timeInfoThis={};
    timeInfoThis.tm_year = this->tm_year-1900; // 年
    timeInfoThis.tm_mon = this->tm_mon-1;            // 月（0-11）
    timeInfoThis.tm_mday = this->tm_mday;           // 日
    timeInfoThis.tm_hour = this->tm_hour;          // 时
    timeInfoThis.tm_min = this->tm_min;           // 分
    timeInfoThis.tm_sec = this->tm_sec;            // 秒
    std::time_t t1 = mktime(&timeInfoThis); 
    printf("from %d-%d-%d %d:%d:%d to %d-%d-%d %d:%d:%d",
        timeInfoThis.tm_year,timeInfoThis.tm_mon,timeInfoThis.tm_mday,timeInfoThis.tm_hour,timeInfoThis.tm_min,timeInfoThis.tm_sec,
        timeInfo.tm_year,timeInfo.tm_mon,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec);
    return t1<t2;
}
bool HGExactTime::operator>(const HGExactTime &obj) const
{
    std::tm timeInfo = {};
    timeInfo.tm_year = obj.tm_year-1900; // 年
    timeInfo.tm_mon = obj.tm_mon-1;            // 月（0-11）
    timeInfo.tm_mday = obj.tm_mday;           // 日
    timeInfo.tm_hour = obj.tm_hour;          // 时
    timeInfo.tm_min = obj.tm_min;           // 分
    timeInfo.tm_sec = obj.tm_sec;            // 秒
    // 将 tm* 类型指针传递给 mktime
    std::time_t t2 = std::mktime(&timeInfo);
    std::tm timeInfoThis={};
    timeInfoThis.tm_year = this->tm_year-1900; // 年
    timeInfoThis.tm_mon = this->tm_mon-1;            // 月（0-11）
    timeInfoThis.tm_mday = this->tm_mday;           // 日
    timeInfoThis.tm_hour = this->tm_hour;          // 时
    timeInfoThis.tm_min = this->tm_min;           // 分
    timeInfoThis.tm_sec = this->tm_sec;            // 秒
    std::time_t t1 = mktime(&timeInfoThis); 
    return t1>t2;
}
// 判断是否为闰年
static bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
// 每个月的天数（平年）
static const int daysInMonth[12] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};
int HGExactTime::toDays() const {
    int days = 0;

    // 加上所有完整年份的天数
    for (int y = 1970; y < this->tm_year; ++y) {
        days += isLeapYear(y) ? 366 : 365;
    }

    // 加上当前年份的月份天数
    for (int m = 1; m < this->tm_mon; ++m) {
        if (m == 2 && isLeapYear(this->tm_year))
            days += 29;
        else
            days += daysInMonth[m - 1];
    }

    // 加上当前月的天数（不包括今天）
    days += (this->tm_mday - 1);

    return days;
}

int HGExactTime::operator-(const HGExactTime& other) const {
    return this->toDays() - other.toDays();
}
void HGExactTime::setSystemTime(int year, int month, int day, int hour, int minute, int second) {
    struct tm newTime;
    newTime.tm_year = year - 1900;  // 年份是从1900开始的
    newTime.tm_mon = month - 1;     // 月份从0开始
    newTime.tm_mday = day;
    newTime.tm_hour = hour;
    newTime.tm_min = minute;
    newTime.tm_sec = second;
    newTime.tm_isdst = -1;  // 自动判断夏令时

    // 将 tm 转换为 time_t
    time_t t = mktime(&newTime);
    if (t == -1) {
        std::cerr << "Failed to convert time" << std::endl;
        return;
    }
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    struct tm *utcTime = gmtime(&t);
    if (!utcTime)
    {
        std::cerr << "Failed to convert to UTC time" << std::endl;
        return;
    }

    SYSTEMTIME st;
    st.wYear = utcTime->tm_year + 1900;
    st.wMonth = utcTime->tm_mon + 1;
    st.wDay = utcTime->tm_mday;
    st.wHour = utcTime->tm_hour;
    st.wMinute = utcTime->tm_min;
    st.wSecond = utcTime->tm_sec;
    st.wMilliseconds = 0;

    if (!SetSystemTime(&st))
    {
        std::cerr << "Failed to set system time. Error: " << GetLastError() << std::endl;
    }
    else
    {
        std::cout << "System time successfully set (Windows UTC)!" << std::endl;
    }

#else
    struct timeval tv;
    tv.tv_sec = t;    // 设置秒
    tv.tv_usec = 0;   // 设置微秒

    // 设置系统时间
    if (settimeofday(&tv, NULL) == -1) {
        perror("Failed to set system time");
    } else {
        std::cout << "System time successfully set!" << std::endl;
    }
#endif
}
