#include "TimeAdapter.h"
#include "HGExactTime.h"

TimeAdapter::TimeAdapter() {
}

TimeInfo TimeAdapter::GetCurrentTime() {
    HGExactTime time = HGExactTime::currentTime();
    return TimeInfo(time.tm_year, time.tm_mon, time.tm_mday, 
                   time.tm_hour, time.tm_min, time.tm_sec, 
                   time.tm_millisec, time.tm_microsec);
}

std::string TimeAdapter::GetCurrentTimeAsString() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toString();
}

std::string TimeAdapter::GetCurrentTimeAsStandardString() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toStandardString();
}

std::string TimeAdapter::GetCurrentTimeAsFilenameString() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toStringForFilename();
}

std::string TimeAdapter::GetCurrentTimeFromYearToDay() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toStringFromYearToDay();
}

std::string TimeAdapter::GetCurrentTimeFromYearToSec() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toStringFromYearToSec();
}

std::string TimeAdapter::GetCurrentTimeFromYearToSecAndZone() {
    HGExactTime time = HGExactTime::currentTime();
    return time.toStringFromYearToSecAndZone();
}

void TimeAdapter::SetSystemTime(int year, int month, int day, int hour, int minute, int second) {
    HGExactTime::setSystemTime(year, month, day, hour, minute, second);
}

int TimeAdapter::GetTimeDifferenceInSeconds(const std::string& timeStr) {
    return HGExactTime::fasterThanThirtyMimutes(timeStr);
}
