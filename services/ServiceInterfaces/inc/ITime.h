#ifndef I_TIME_H
#define I_TIME_H

#include <string>
#include <ctime>

struct TimeInfo {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    int microsecond;

    TimeInfo() 
        : year(0), month(0), day(0), hour(0), minute(0), second(0), millisecond(0), microsecond(0) {}
    
    TimeInfo(int y, int mo, int d, int h, int mi, int s, int ms = 0, int us = 0)
        : year(y), month(mo), day(d), hour(h), minute(mi), second(s), millisecond(ms), microsecond(us) {}

    bool operator<(const TimeInfo& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        if (day != other.day) return day < other.day;
        if (hour != other.hour) return hour < other.hour;
        if (minute != other.minute) return minute < other.minute;
        if (second != other.second) return second < other.second;
        if (millisecond != other.millisecond) return millisecond < other.millisecond;
        return microsecond < other.microsecond;
    }

    bool operator>(const TimeInfo& other) const {
        return other < *this;
    }

    bool operator<=(const TimeInfo& other) const {
        return !(other < *this);
    }

    bool operator>=(const TimeInfo& other) const {
        return !(*this < other);
    }

    bool operator==(const TimeInfo& other) const {
        return year == other.year && month == other.month && day == other.day &&
               hour == other.hour && minute == other.minute && second == other.second &&
               millisecond == other.millisecond && microsecond == other.microsecond;
    }

    bool operator!=(const TimeInfo& other) const {
        return !(*this == other);
    }

    int operator-(const TimeInfo& other) const {
        std::tm tm1 = {};
        tm1.tm_year = year - 1900;
        tm1.tm_mon = month - 1;
        tm1.tm_mday = day;
        tm1.tm_hour = hour;
        tm1.tm_min = minute;
        tm1.tm_sec = second;

        std::tm tm2 = {};
        tm2.tm_year = other.year - 1900;
        tm2.tm_mon = other.month - 1;
        tm2.tm_mday = other.day;
        tm2.tm_hour = other.hour;
        tm2.tm_min = other.minute;
        tm2.tm_sec = other.second;

        std::time_t time1 = std::mktime(&tm1);
        std::time_t time2 = std::mktime(&tm2);

        if (time1 == -1 || time2 == -1) return 0;
        return static_cast<int>(std::difftime(time1, time2) / (60 * 60 * 24));
    }

    TimeInfo& operator+=(int days) {
        std::tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;

        std::time_t time = std::mktime(&tm);
        if (time != -1) {
            time += days * 24 * 60 * 60;
            std::tm* new_tm = std::localtime(&time);
            if (new_tm) {
                year = new_tm->tm_year + 1900;
                month = new_tm->tm_mon + 1;
                day = new_tm->tm_mday;
                hour = new_tm->tm_hour;
                minute = new_tm->tm_min;
                second = new_tm->tm_sec;
            }
        }
        return *this;
    }
};

class ITime {
public:
    virtual ~ITime() = default;

    virtual TimeInfo GetCurrentTime() = 0;
    virtual std::string GetCurrentTimeAsString() = 0;
    virtual std::string GetCurrentTimeAsStandardString() = 0;
    virtual std::string GetCurrentTimeAsFilenameString() = 0;
    virtual std::string GetCurrentTimeFromYearToDay() = 0;
    virtual std::string GetCurrentTimeFromYearToSec() = 0;
    virtual std::string GetCurrentTimeFromYearToSecAndZone() = 0;
    virtual void SetSystemTime(int year, int month, int day, int hour, int minute, int second) = 0;
    virtual int GetTimeDifferenceInSeconds(const std::string& timeStr) = 0;
};

#endif // I_TIME_H
