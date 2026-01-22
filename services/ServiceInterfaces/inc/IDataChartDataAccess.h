#ifndef I_DATA_CHART_DATA_ACCESS_H
#define I_DATA_CHART_DATA_ACCESS_H

#include <string>
#include <vector>
#include "ServiceInterfaces_global.h"

class SERVICEINTERFACES_EXPORT IDataChartDataAccess {
public:
    virtual ~IDataChartDataAccess() = default;

    virtual void Load() = 0;
    virtual void Save() = 0;
    
    virtual void SetValue(const std::string& key, const std::string& value) = 0;
    virtual void AddValue(const std::string& key, const std::string& value) = 0;
    virtual const std::string& GetValue(const std::string& key) const = 0;
};

#endif // I_DATA_CHART_DATA_ACCESS_H
