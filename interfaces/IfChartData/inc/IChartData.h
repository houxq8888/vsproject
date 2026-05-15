#ifndef ICHARTDATA_H
#define ICHARTDATA_H

#include "IfChartData_global.h"
#include <string>
#include <vector>
#include <map>

namespace HGMACHINE {

class IFCHARTDATA_EXPORT IChartData {
public:
    virtual ~IChartData() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual void load() = 0;
    virtual void save() = 0;
    
    virtual void setValue(const std::string& key, const std::string& value) = 0;
    virtual void addValue(const std::string& key, const std::string& value) = 0;
    virtual std::string getValue(const std::string& key) const = 0;
    
    virtual std::vector<std::string> getRecordNames() const = 0;
    virtual std::vector<std::string> getDisplayNames() const = 0;
    virtual std::vector<std::string> getRecordInTimeNames() const = 0;
    
    virtual void setRecordNames(const std::vector<std::string>& names) = 0;
    virtual void setDisplayNames(const std::vector<std::string>& names) = 0;
    virtual void setRecordInTimeNames(const std::vector<std::string>& names) = 0;
};

}

#endif // ICHARTDATA_H
