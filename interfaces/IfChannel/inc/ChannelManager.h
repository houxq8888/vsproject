#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include "IChannel.h"
#include <memory>

namespace HGMACHINE {

class ChannelManager {
public:
    static ChannelManager& instance();
    
    IChannel& get();
    
private:
    ChannelManager();
    ~ChannelManager();
    
    ChannelManager(const ChannelManager&) = delete;
    ChannelManager& operator=(const ChannelManager&) = delete;
    
    std::unique_ptr<IChannel> m_channel;
};

}

#endif // CHANNELMANAGER_H
