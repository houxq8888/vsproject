#ifndef COMMUNICATEADAPTER_H
#define COMMUNICATEADAPTER_H

#include "ICommunicate.h"
#include <memory>

namespace HGMACHINE {

class CommunicateAdapter : public ICommunicate {
public:
    CommunicateAdapter();
    ~CommunicateAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    bool connect(const std::string& address, int port) override;
    bool disconnect() override;
    bool isConnected() const override;
    
    bool send(const std::string& data) override;
    bool receive(std::string& data) override;
    bool sendAndReceive(const std::string& sendData, std::string& receiveData) override;
    
    void setTimeout(int timeoutMs) override;
    int getTimeout() const override;
    
    std::string getLastError() const override;
    void clearError() override;
    
    bool sshConnect(const std::string& host, int port, 
                   const std::string& username, const std::string& password) override;
    bool sshDisconnect() override;
    bool sshIsConnected() override;
    bool sshUploadFile(const std::string& localPath, const std::string& remotePath) override;
    
private:
    class Impl;
    Impl* m_impl;
};

}

#endif
