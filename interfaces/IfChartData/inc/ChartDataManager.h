#ifndef CHARTDATAMANAGER_H
#define CHARTDATAMANAGER_H

#include "IChartData.h"
#include <memory>

namespace HGMACHINE {

class ChartDataManager {
public:
    static ChartDataManager& instance();
    
    IChartData& get();
    
private:
    ChartDataManager();
    ~ChartDataManager();
    ChartDataManager(const ChartDataManager&) = delete;
    ChartDataManager& operator=(const ChartDataManager&) = delete;
    
    std::unique_ptr<IChartData> m_chartData;
};

}

#endif
