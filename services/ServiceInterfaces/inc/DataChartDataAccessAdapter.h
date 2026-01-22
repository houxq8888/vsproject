#ifndef DATA_CHART_DATA_ACCESS_ADAPTER_H
#define DATA_CHART_DATA_ACCESS_ADAPTER_H

#include "IDataChartDataAccess.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT DataChartDataAccessAdapter : public IDataChartDataAccess {
public:
    DataChartDataAccessAdapter();
    virtual ~DataChartDataAccessAdapter() = default;

    void Load() override;
    void Save() override;
    
    void SetValue(const std::string& key, const std::string& value) override;
    void AddValue(const std::string& key, const std::string& value) override;
    const std::string& GetValue(const std::string& key) const override;

private:
    class Impl;
    Impl* m_impl;
};

#endif // DATA_CHART_DATA_ACCESS_ADAPTER_H
