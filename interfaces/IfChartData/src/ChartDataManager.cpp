#include "ChartDataManager.h"
#include "ChartDataAdapter.h"

namespace HGMACHINE {

ChartDataManager::ChartDataManager() {
    m_chartData = std::make_unique<ChartDataAdapter>();
    m_chartData->initialize();
}

ChartDataManager::~ChartDataManager() {
    if (m_chartData) {
        m_chartData->shutdown();
    }
}

ChartDataManager& ChartDataManager::instance() {
    static ChartDataManager instance;
    return instance;
}

IChartData& ChartDataManager::get() {
    return *m_chartData;
}

}
