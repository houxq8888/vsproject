#ifndef CORECOMMUNICATE_H
#define CORECOMMUNICATE_H

#include "CoreCommunicate_global.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace HGMACHINE {

class SshUploader;

class CORECOMMUNICATE_EXPORT CoreCommunicate {
public:
    CoreCommunicate();
    ~CoreCommunicate();
    
    bool initialize();
    void shutdown();
    
    bool connect(const std::string& address, int port);
    bool disconnect();
    bool isConnected() const;
    
    bool send(const std::string& data);
    bool receive(std::string& data);
    bool sendAndReceive(const std::string& sendData, std::string& receiveData);
    
    void setTimeout(int timeoutMs);
    int getTimeout() const;
    
    std::string getLastError() const;
    void clearError();
    
    bool sftpConnect(const std::string& host, int port, 
                     const std::string& username, const std::string& password);
    bool sftpDisconnect();
    bool sftpIsConnected();
    bool sftpUploadFile(const std::string& localPath, const std::string& remotePath);
    
private:
    bool m_initialized;
    bool m_connected;
    int m_timeoutMs;
    std::string m_lastError;
    std::unique_ptr<SshUploader> m_sshUploader;
};

}

#endif // CORECOMMUNICATE_H
