#include "CommunicateAdapter.h"
#include "CoreCommunicate.h"

namespace HGMACHINE {

class CommunicateAdapter::Impl {
public:
    CoreCommunicate coreCommunicate;
};

CommunicateAdapter::CommunicateAdapter() : m_impl(new Impl()) {
}

CommunicateAdapter::~CommunicateAdapter() {
    delete m_impl;
}

bool CommunicateAdapter::initialize() {
    return m_impl->coreCommunicate.initialize();
}

void CommunicateAdapter::shutdown() {
    m_impl->coreCommunicate.shutdown();
}

bool CommunicateAdapter::connect(const std::string& address, int port) {
    return m_impl->coreCommunicate.connect(address, port);
}

bool CommunicateAdapter::disconnect() {
    return m_impl->coreCommunicate.disconnect();
}

bool CommunicateAdapter::isConnected() const {
    return m_impl->coreCommunicate.isConnected();
}

bool CommunicateAdapter::send(const std::string& data) {
    return m_impl->coreCommunicate.send(data);
}

bool CommunicateAdapter::receive(std::string& data) {
    return m_impl->coreCommunicate.receive(data);
}

bool CommunicateAdapter::sendAndReceive(const std::string& sendData, std::string& receiveData) {
    return m_impl->coreCommunicate.sendAndReceive(sendData, receiveData);
}

void CommunicateAdapter::setTimeout(int timeoutMs) {
    m_impl->coreCommunicate.setTimeout(timeoutMs);
}

int CommunicateAdapter::getTimeout() const {
    return m_impl->coreCommunicate.getTimeout();
}

std::string CommunicateAdapter::getLastError() const {
    return m_impl->coreCommunicate.getLastError();
}

void CommunicateAdapter::clearError() {
    m_impl->coreCommunicate.clearError();
}

bool CommunicateAdapter::sshConnect(const std::string& host, int port, 
                                    const std::string& username, const std::string& password) {
    return m_impl->coreCommunicate.sftpConnect(host, port, username, password);
}

bool CommunicateAdapter::sshDisconnect() {
    return m_impl->coreCommunicate.sftpDisconnect();
}

bool CommunicateAdapter::sshIsConnected() {
    return m_impl->coreCommunicate.sftpIsConnected();
}

bool CommunicateAdapter::sshUploadFile(const std::string& localPath, const std::string& remotePath) {
    return m_impl->coreCommunicate.sftpUploadFile(localPath, remotePath);
}

}
