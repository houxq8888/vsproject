#include "TimeAdapter.h"
#include "HGExactTime.h"
#include <chrono>

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

double TimeAdapter::GetElapsedMilliseconds(const TimeInfo& start, const TimeInfo& end) {
    int minToSec = 60;
    int hourToMin = 60;
    int dayToHour = 24;
    int monthToDay = 30;
    int yearToMonth = 12;
    double startTime = 0.0;
    startTime = (start.year * yearToMonth * monthToDay * dayToHour * hourToMin * minToSec + 
            start.month * monthToDay * dayToHour * hourToMin * minToSec +
            start.day * dayToHour * hourToMin * minToSec + 
            start.hour * hourToMin * minToSec + 
            start.minute * minToSec + 
            start.second + 
            start.millisecond / 1000.0 + start.microsecond / 1000000.0);

    double endTime = 0.0;
    endTime = (end.year * yearToMonth * monthToDay * dayToHour * hourToMin * minToSec + 
            end.month * monthToDay * dayToHour * hourToMin * minToSec +
            end.day * dayToHour * hourToMin * minToSec + 
            end.hour * hourToMin * minToSec + 
            end.minute * minToSec + 
            end.second + 
            end.millisecond / 1000.0 + end.microsecond / 1000000.0);    
    double elapsed = (endTime - startTime) * 1000; // millisec
    return elapsed;
}
