#include "hgcomwithssh.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

namespace HGMACHINE{
SshUploader::SshUploader(const std::string& host, int port,
                         const std::string& username, const std::string& password)
    : host_(host), port_(port), username_(username), password_(password),
      session_(nullptr), sftp_(nullptr) {}

SshUploader::~SshUploader() {
    cleanup();
}

bool SshUploader::connect() {
    session_ = ssh_new();
    if (!session_) return false;

    ssh_options_set(session_, SSH_OPTIONS_HOST, host_.c_str());
    ssh_options_set(session_, SSH_OPTIONS_PORT, &port_);
    ssh_options_set(session_, SSH_OPTIONS_USER, username_.c_str());

    if (ssh_connect(session_) != SSH_OK) {
        std::cerr << "SSH connection failed: " << ssh_get_error(session_) << "\n";
        return false;
    }

    if (ssh_userauth_password(session_, nullptr, password_.c_str()) != SSH_AUTH_SUCCESS) {
        std::cerr << "Authentication failed: " << ssh_get_error(session_) << "\n";
        return false;
    }

    return initSFTP();
}

bool SshUploader::initSFTP() {
    sftp_ = sftp_new(session_);
    if (!sftp_) {
        std::cerr << "Error allocating SFTP session: " << ssh_get_error(session_) << "\n";
        return false;
    }

    if (sftp_init(sftp_) != SSH_OK) {
        std::cerr << "SFTP initialization failed: " << sftp_get_error(sftp_) << "\n";
        sftp_free(sftp_);
        sftp_ = nullptr;
        return false;
    }

    return true;
}

bool SshUploader::uploadFile(const std::string& localPath, const std::string& remotePath) {
    std::ifstream inFile(localPath, std::ios::binary);
    // std::cout<<"localPath = "<<localPath<<std::endl;
    if (!inFile) {
        std::cerr << "Failed to open local file: " << localPath << "\n";
        return false;
    }

    sftp_file remoteFile = sftp_open(sftp_, remotePath.c_str(),
        O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);  // chmod 0644

    if (!remoteFile) {
        std::cerr << "Failed to open remote file: " << remotePath
                  << ", error: " << sftp_get_error(sftp_) << "\n";
        return false;
    }

    char buffer[16384];  // 增大 buffer size 以减少写入次数
    size_t totalBytes = 0;
    while (inFile) {
        inFile.read(buffer, sizeof(buffer));
        std::streamsize bytesRead = inFile.gcount();
        // std::cout<<"bytesRead = "<<bytesRead<<std::endl;
        if (bytesRead <= 0) break;

        ssize_t bytesWritten = sftp_write(remoteFile, buffer, bytesRead);
        // std::cout<<"bytesWritten = "<<bytesWritten<<std::endl;
        if (bytesWritten != bytesRead) {
            std::cerr << "Error writing to remote file: " << sftp_get_error(sftp_)
                      << ", bytesRead = " << bytesRead
                      << ", bytesWritten = " << bytesWritten << "\n";
            sftp_close(remoteFile);
            return false;
        }

        totalBytes += bytesWritten;
    }

    if (sftp_close(remoteFile) != SSH_OK) {
        std::cerr << "Error closing remote file: " << sftp_get_error(sftp_) << "\n";
        return false;
    }

    std::cout << "Upload success. Total bytes written: " << totalBytes << "\n";
    return true;
}


void SshUploader::cleanup() {
    if (sftp_) {
        sftp_free(sftp_);
        sftp_ = nullptr;
    }

    if (session_) {
        ssh_disconnect(session_);
        ssh_free(session_);
        session_ = nullptr;
    }
}
bool SshUploader::disconnect(){
    cleanup();
    return true;
}
bool SshUploader::isConnected(){
    return session_!=nullptr;
}
}