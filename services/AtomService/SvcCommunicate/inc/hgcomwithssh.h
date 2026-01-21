#ifndef HGCOMWITHSSH_H
#define HGCOMWITHSSH_H

#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE{

class SshUploader {
public:
    SshUploader(const std::string& host, int port,
                const std::string& username, const std::string& password);
    ~SshUploader();

    bool connect();
    bool disconnect();
    bool isConnected();
    bool uploadFile(const std::string& localPath, const std::string& remotePath);

private:
    std::string host_;
    int port_;
    std::string username_;
    std::string password_;

    ssh_session session_;
    sftp_session sftp_;

    bool initSFTP();
    void cleanup();
};
}

#ifdef __cplusplus
}
#endif
#endif // HGCOMWITHSSH_H