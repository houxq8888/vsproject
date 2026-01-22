#ifndef TIME_ADAPTER_H
#define TIME_ADAPTER_H

#include "ITime.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT TimeAdapter : public ITime {
public:
    TimeAdapter();
    virtual ~TimeAdapter() = default;

    TimeInfo GetCurrentTime() override;
    std::string GetCurrentTimeAsString() override;
    std::string GetCurrentTimeAsStandardString() override;
    std::string GetCurrentTimeAsFilenameString() override;
    std::string GetCurrentTimeFromYearToDay() override;
    std::string GetCurrentTimeFromYearToSec() override;
    std::string GetCurrentTimeFromYearToSecAndZone() override;
    void SetSystemTime(int year, int month, int day, int hour, int minute, int second) override;
    int GetTimeDifferenceInSeconds(const std::string& timeStr) override;
};

#endif // TIME_ADAPTER_H
