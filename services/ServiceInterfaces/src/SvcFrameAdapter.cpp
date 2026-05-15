#include "SvcFrameAdapter.h"
#include "rwDb.h"
#include "config.h"

SvcFrameAdapter::SvcFrameAdapter() {
}

SvcFrameAdapter::~SvcFrameAdapter() {
}

std::string SvcFrameAdapter::ReadCurDirPath() {
    return HGMACHINE::RWDb::readCurDirPath();
}

bool SvcFrameAdapter::loadConfig(const std::string &curPath) {
    return HGMACHINE::loadConfig(curPath);
}

void SvcFrameAdapter::saveConfig() {
    HGMACHINE::saveConfig();
}
std::string SvcFrameAdapter::getAppName(){
    return HGMACHINE::FileConfig::getAppName();
}

std::string SvcFrameAdapter::GetDirPath() const {
    return HGMACHINE::FileConfig::getDirPath();
}
uint16_t SvcFrameAdapter::getScannerProductID() {
    return HGMACHINE::FileConfig::getScannerProductID();
}
uint16_t SvcFrameAdapter::getScannerVendorID() {
    return HGMACHINE::FileConfig::getScannerVendorID();
}