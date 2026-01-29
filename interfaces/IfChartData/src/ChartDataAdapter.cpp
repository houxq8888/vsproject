#include "ChartDataAdapter.h"
#include "CoreChartData.h"

namespace HGMACHINE {

class ChartDataAdapter::Impl {
public:
    CoreChartData coreChartData;
};

ChartDataAdapter::ChartDataAdapter() : m_impl(new Impl()) {
}

ChartDataAdapter::~ChartDataAdapter() {
    delete m_impl;
}

bool ChartDataAdapter::initialize() {
    return m_impl->coreChartData.initialize();
}

void ChartDataAdapter::shutdown() {
    m_impl->coreChartData.shutdown();
}

void ChartDataAdapter::load() {
    m_impl->coreChartData.load();
}

void ChartDataAdapter::save() {
    m_impl->coreChartData.save();
}

void ChartDataAdapter::setValue(const std::string& key, const std::string& value) {
    m_impl->coreChartData.setValue(key, value);
}

void ChartDataAdapter::addValue(const std::string& key, const std::string& value) {
    m_impl->coreChartData.addValue(key, value);
}

std::string ChartDataAdapter::getValue(const std::string& key) const {
    return m_impl->coreChartData.getValue(key);
}

std::vector<std::string> ChartDataAdapter::getRecordNames() const {
    return m_impl->coreChartData.getRecordNames();
}

std::vector<std::string> ChartDataAdapter::getDisplayNames() const {
    return m_impl->coreChartData.getDisplayNames();
}

std::vector<std::string> ChartDataAdapter::getRecordInTimeNames() const {
    return m_impl->coreChartData.getRecordInTimeNames();
}

void ChartDataAdapter::setRecordNames(const std::vector<std::string>& names) {
    m_impl->coreChartData.setRecordNames(names);
}

void ChartDataAdapter::setDisplayNames(const std::vector<std::string>& names) {
    m_impl->coreChartData.setDisplayNames(names);
}

void ChartDataAdapter::setRecordInTimeNames(const std::vector<std::string>& names) {
    m_impl->coreChartData.setRecordInTimeNames(names);
}

}
