#include "ChannelManager.h"
#include "ChannelAdapter.h"

using namespace HGMACHINE;

ChannelManager::ChannelManager() : m_channel(new ChannelAdapter()) {
}

ChannelManager::~ChannelManager() {
}

ChannelManager& ChannelManager::instance() {
    static ChannelManager instance;
    return instance;
}

IChannel& ChannelManager::get() {
    return *m_channel;
}
