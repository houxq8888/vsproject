#ifndef HGCOMWITHSOCKET_H
#define HGCOMWITHSOCKET_H

#include "hgbasecom.h"

// 平台差异封装
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketHandle;
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    typedef int SocketHandle;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif


namespace HGMACHINE
{
    enum SOCKET_Mode
    {
        SERVER,
        CLIENT
    };
    class HgComWithSocket
    {
    public:
        HgComWithSocket(SOCKET_Mode mode, const std::string &ip = "", int port = 0);
        ~HgComWithSocket();

        bool start();                // Server: bind+listen | Client: connect
        bool startServer();          // 新增：多线程服务器
        bool startServerSimple();    // 新增：单线程服务器（测试用）
        bool connectWithTimeout(int timeoutSec=5);

        SocketHandle acceptClient(); // only valid for SERVER
        // ---- 通用收发 ----
        int recvData(SocketHandle sock, std::string &outData,bool readUntilClose=false); // Server: 传 clientSock
        int sendData(SocketHandle sock, const std::string &data);

        // ---- Client 模式下的便捷收发 ----
        int recvData(std::vector<uint8_t> &vecdata, std::string &outData); // Client: 内部 sock
        int sendData(const std::string &data);
        void closeSocket(SocketHandle sock);
        void stop();
        // 新增：处理客户端连接
        void handleClient(SocketHandle clientSock);

    private:
        SOCKET_Mode mode;
        std::string ip;
        int port;
        SocketHandle sock;
        sockaddr_in addr{};
        std::string m_receivedData="";
    };
} // namespace HGMACHINE

#endif