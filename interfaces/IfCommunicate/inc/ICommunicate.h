#ifndef ICOMMUNICATE_H
#define ICOMMUNICATE_H

#include "IfCommunicate_global.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace HGMACHINE {

class IFCOMMUNICATE_EXPORT ICommunicate {
public:
    virtual ~ICommunicate() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual bool connect(const std::string& address, int port) = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    virtual bool send(const std::string& data) = 0;
    virtual bool receive(std::string& data) = 0;
    virtual bool sendAndReceive(const std::string& sendData, std::string& receiveData) = 0;
    
    virtual void setTimeout(int timeoutMs) = 0;
    virtual int getTimeout() const = 0;
    
    virtual std::string getLastError() const = 0;
    virtual void clearError() = 0;
    
    virtual bool sshConnect(const std::string& host, int port, 
                           const std::string& username, const std::string& password) = 0;
    virtual bool sshDisconnect() = 0;
    virtual bool sshIsConnected() = 0;
    virtual bool sshUploadFile(const std::string& localPath, const std::string& remotePath) = 0;
};

}

#endif // ICOMMUNICATE_H
