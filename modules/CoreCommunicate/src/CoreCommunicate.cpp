#include "CoreCommunicate.h"
#include "hgcomwithssh.h"

namespace HGMACHINE {

CoreCommunicate::CoreCommunicate()
    : m_initialized(false)
    , m_connected(false)
    , m_timeoutMs(5000)
    , m_sshUploader(nullptr)
{
}

CoreCommunicate::~CoreCommunicate() {
    shutdown();
}

bool CoreCommunicate::initialize() {
    if (!m_initialized) {
        m_initialized = true;
    }
    return m_initialized;
}

void CoreCommunicate::shutdown() {
    if (m_connected) {
        disconnect();
    }
    if (m_sshUploader) {
        sftpDisconnect();
    }
    m_initialized = false;
}

bool CoreCommunicate::connect(const std::string& address, int port) {
    if (!m_initialized) {
        m_lastError = "Not initialized";
        return false;
    }
    
    m_connected = true;
    return m_connected;
}

bool CoreCommunicate::disconnect() {
    if (!m_initialized) {
        m_lastError = "Not initialized";
        return false;
    }
    
    m_connected = false;
    return true;
}

bool CoreCommunicate::isConnected() const {
    return m_connected;
}

bool CoreCommunicate::send(const std::string& data) {
    if (!m_initialized) {
        m_lastError = "Not initialized";
        return false;
    }
    
    if (!m_connected) {
        m_lastError = "Not connected";
        return false;
    }
    
    return true;
}

bool CoreCommunicate::receive(std::string& data) {
    if (!m_initialized) {
        m_lastError = "Not initialized";
        return false;
    }
    
    if (!m_connected) {
        m_lastError = "Not connected";
        return false;
    }
    
    return true;
}

bool CoreCommunicate::sendAndReceive(const std::string& sendData, std::string& receiveData) {
    if (!send(sendData)) {
        return false;
    }
    
    return receive(receiveData);
}

void CoreCommunicate::setTimeout(int timeoutMs) {
    m_timeoutMs = timeoutMs;
}

int CoreCommunicate::getTimeout() const {
    return m_timeoutMs;
}

std::string CoreCommunicate::getLastError() const {
    return m_lastError;
}

void CoreCommunicate::clearError() {
    m_lastError.clear();
}

bool CoreCommunicate::sftpConnect(const std::string& host, int port, 
                                  const std::string& username, const std::string& password) {
    if (!m_initialized) {
        m_lastError = "Not initialized";
        return false;
    }
    
    m_sshUploader = std::make_unique<SshUploader>(host, port, username, password);
    if (!m_sshUploader->connect()) {
        m_lastError = "Failed to connect to SSH server";
        m_sshUploader.reset();
        return false;
    }
    
    return true;
}

bool CoreCommunicate::sftpDisconnect() {
    if (!m_sshUploader) {
        return true;
    }
    
    if (!m_sshUploader->disconnect()) {
        m_lastError = "Failed to disconnect from SSH server";
        return false;
    }
    
    m_sshUploader.reset();
    return true;
}

bool CoreCommunicate::sftpIsConnected() {
    if (!m_sshUploader) {
        return false;
    }
    
    return m_sshUploader->isConnected();
}

bool CoreCommunicate::sftpUploadFile(const std::string& localPath, const std::string& remotePath) {
    if (!m_sshUploader) {
        m_lastError = "SSH uploader not connected";
        return false;
    }
    
    if (!m_sshUploader->uploadFile(localPath, remotePath)) {
        m_lastError = "Failed to upload file";
        return false;
    }
    
    return true;
}

}
