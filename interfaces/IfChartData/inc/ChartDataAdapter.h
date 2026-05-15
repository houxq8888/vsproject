#ifndef CHARTDATAADAPTER_H
#define CHARTDATAADAPTER_H

#include "IChartData.h"
#include <memory>

namespace HGMACHINE {

class ChartDataAdapter : public IChartData {
public:
    ChartDataAdapter();
    ~ChartDataAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    void load() override;
    void save() override;
    
    void setValue(const std::string& key, const std::string& value) override;
    void addValue(const std::string& key, const std::string& value) override;
    std::string getValue(const std::string& key) const override;
    
    std::vector<std::string> getRecordNames() const override;
    std::vector<std::string> getDisplayNames() const override;
    std::vector<std::string> getRecordInTimeNames() const override;
    
    void setRecordNames(const std::vector<std::string>& names) override;
    void setDisplayNames(const std::vector<std::string>& names) override;
    void setRecordInTimeNames(const std::vector<std::string>& names) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif
