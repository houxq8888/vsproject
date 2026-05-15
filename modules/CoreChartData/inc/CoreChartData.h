#ifndef CORECHARTDATA_H
#define CORECHARTDATA_H

#include "CoreChartData_global.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace HGMACHINE {

class CORECHARTDATA_EXPORT CoreChartData {
public:
    CoreChartData();
    ~CoreChartData();
    
    bool initialize();
    void shutdown();
    
    void load();
    void save();
    
    void setValue(const std::string& key, const std::string& value);
    void addValue(const std::string& key, const std::string& value);
    std::string getValue(const std::string& key) const;
    
    std::vector<std::string> getRecordNames() const;
    std::vector<std::string> getDisplayNames() const;
    std::vector<std::string> getRecordInTimeNames() const;
    
    void setRecordNames(const std::vector<std::string>& names);
    void setDisplayNames(const std::vector<std::string>& names);
    void setRecordInTimeNames(const std::vector<std::string>& names);
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif // CORECHARTDATA_H
