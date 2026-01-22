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
