#include "hgcomwithsocket.h"
#include <iostream>
#include <iomanip>
#include <thread>

namespace HGMACHINE
{
    // 平台初始化
#ifdef _WIN32
    static bool wsaInit = false;
    void ensureWinsockInit()
    {
        if (!wsaInit)
        {
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
            wsaInit = true;
        }
    }
#else
    inline void ensureWinsockInit() {}
#endif

    HgComWithSocket::HgComWithSocket(SOCKET_Mode mode, const std::string &ip, int port)
        : mode(mode), ip(ip), port(port), sock(INVALID_SOCKET)
    {
        ensureWinsockInit();
    }

    HgComWithSocket::~HgComWithSocket()
    {
        stop();
    }
    // 设置非阻塞 + 超时连接
    bool HgComWithSocket::connectWithTimeout(int timeoutSec)
    {
        if (mode != CLIENT)
        {
            std::cerr << "connectWithTimeout can only be used in CLIENT mode" << std::endl;
            return false;
        }

        if (sock == INVALID_SOCKET)
        {
            std::cerr << "Socket not initialized! Call start() first." << std::endl;
            return false;
        }

        // 设置非阻塞模式
#ifdef _WIN32
        u_long nonBlocking = 1;
        if (ioctlsocket(sock, FIONBIO, &nonBlocking) != 0)
        {
            std::cerr << "Failed to set non-blocking mode: " << WSAGetLastError() << std::endl;
            return false;
        }
#else
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags == -1)
        {
            perror("fcntl get");
            return false;
        }
        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            perror("fcntl set non-blocking");
            return false;
        }
#endif

        int ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
#ifdef _WIN32
        int err = WSAGetLastError();
        if (ret < 0 && err != WSAEWOULDBLOCK && err != WSAEINPROGRESS)
        {
            std::cerr << "Connect failed immediately: " << err << std::endl;
            return false;
        }
#else
        if (ret < 0 && errno != EINPROGRESS)
        {
            perror("connect");
            return false;
        }
#endif

        // 等待连接完成
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        timeval tv{};
        tv.tv_sec = timeoutSec;
        tv.tv_usec = 0;

        ret = select(sock + 1, nullptr, &fdset, nullptr, &tv);
        if (ret <= 0)
        {
            std::cerr << "Connect timeout or error" << std::endl;
            return false;
        }

        // 检查 socket 错误
        int so_error = 0;
        socklen_t len = sizeof(so_error);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&so_error, &len) < 0)
        {
            perror("getsockopt");
            return false;
        }
        if (so_error != 0)
        {
            std::cerr << "Connect failed, SO_ERROR=" << so_error << std::endl;
            return false;
        }

//         // 恢复阻塞模式
// #ifdef _WIN32
//         nonBlocking = 0;
//         ioctlsocket(sock, FIONBIO, &nonBlocking);
// #else
//         fcntl(sock, F_SETFL, flags);
// #endif

        std::cout << "Connected to " << ip << ":" << port << std::endl;
        return true;
    }
    // 处理单个客户端连接的函数
    void HgComWithSocket::handleClient(SocketHandle clientSock)
    {
        std::cout << "Starting to handle client connection..." << std::endl;
        
        std::string receivedData;
        while (true) {
            // 读取客户端数据
            int ret = recvData(clientSock, receivedData, false);
            
            if (ret > 0) {
                // 处理接收到的数据
                m_receivedData=receivedData;

                // std::cout << "Processing data: " << receivedData << std::endl;
                
                // 示例：回声服务器，将数据发回客户端
                // sendData(clientSock, "Server received: " + receivedData);
                
                receivedData.clear(); // 清空以备下次接收
            } 
            else if (ret == 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            } 
            else {
                std::cerr << "Error receiving data from client." << std::endl;
                break;
            }
#ifdef _WIN32
#else            
            // 短暂休眠，避免CPU占用过高
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
        }
        
        closeSocket(clientSock);
    }
    // 启动服务器主循环
    bool HgComWithSocket::startServer()
    {
        if (mode != SERVER) {
            std::cerr << "startServer can only be used in SERVER mode" << std::endl;
            return false;
        }

        if (!start()) {
            return false;
        }

        std::cout << "Server started successfully. Waiting for connections..." << std::endl;

        // 主接受循环
        while (true) {
            SocketHandle clientSock = acceptClient();
            if (clientSock == INVALID_SOCKET) {
                continue; // 继续等待下一个连接
            }
#ifdef _WIN32
#else
            // 为每个客户端创建一个新线程处理
            std::thread clientThread(&HgComWithSocket::handleClient, this, clientSock);
            clientThread.detach(); // 分离线程，让其独立运行
#endif       
            std::cout << "New client connected. Spawned handler thread." << std::endl;
        }

        return true;
    }

    // 简单的单线程服务器版本（适用于测试）
    bool HgComWithSocket::startServerSimple()
    {
        if (mode != SERVER) {
            std::cerr << "startServerSimple can only be used in SERVER mode" << std::endl;
            return false;
        }

        if (!start()) {
            return false;
        }

        std::cout << "Simple server started. Waiting for one connection..." << std::endl;

        SocketHandle clientSock = acceptClient();
        if (clientSock == INVALID_SOCKET) {
            return false;
        }

        // 处理这个客户端
        handleClient(clientSock);

        return true;
    }
    bool HgComWithSocket::start()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
        {
            perror("socket");
            return false;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (mode == SERVER)
        {
            addr.sin_addr.s_addr = INADDR_ANY;
            int opt = 1;
#ifdef _WIN32
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
#else
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

            if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
            {
                perror("bind");
                return false;
            }
            if (listen(sock, 5) == SOCKET_ERROR)
            {
                perror("listen");
                return false;
            }
            std::cout << "Server listening on port " << port << std::endl;
        }
        else
        { // CLIENT
            if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
            {
                perror("inet_pton");
                return false;
            }
            // if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
            // {
            //     perror("connect");
            //     return false;
            // }
            std::cout << "Client socket created for " << ip << ":" << port << std::endl;
        }
        return true;
    }

    SocketHandle HgComWithSocket::acceptClient()
    {
        if (mode != SERVER)
            return INVALID_SOCKET;

        sockaddr_in clientAddr{};
#ifdef _WIN32
        int len = sizeof(clientAddr);
#else
        socklen_t len = sizeof(clientAddr);
#endif
        SocketHandle clientSock = accept(sock, (struct sockaddr *)&clientAddr, &len);
        if (clientSock == INVALID_SOCKET)
        {
            perror("accept");
            return INVALID_SOCKET;
        }
        std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        return clientSock;
    }

    // ---- 通用收发 ----
    int HgComWithSocket::recvData(SocketHandle s, std::string &outData,bool readUntilClose)
    {
        char buffer[1024];
        outData.clear();
        int total_len=0;
        int len=0;
        // 循环读取直到连接关闭或缓冲区无数据
        while ((len = recv(s, buffer, sizeof(buffer), 0)) > 0) {
            outData.append(buffer, len);
            total_len += len;
            
            // 如果不要求读取到连接关闭，且已经读到了一些数据，可以提前返回
            if (!readUntilClose && total_len > 0) {
                break;
            }
            
            // 非阻塞模式检查（如果设置了非阻塞）
            #ifdef _WIN32
            if (len == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
                break;
            }
            #else
            if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                break;
            }
            #endif
        }

        std::cout << "recvData total_len: " << total_len << std::endl;
        if (total_len > 0) {
            std::cout << "Recv: " << outData << std::endl;
        }
        
        if (len == 0) {
            std::cout << "Connection closed by peer." << std::endl;
            return 0;
        } else if (len < 0) {
            perror("recv failed");
            return len;
        }
        
        return total_len;
    }

    int HgComWithSocket::sendData(SocketHandle s, const std::string &data)
    {
        return send(s, data.c_str(), (int)data.size(), 0);
    }

    // ---- Client 模式的便捷收发 ----
    int HgComWithSocket::recvData(std::vector<uint8_t> &vecdata, std::string &outData)
    {
        int len = 0;
        // std::string outData;
        if (mode == CLIENT){
            len = recvData(sock, outData);
        } else if (mode == SERVER){
            // printf("Server mode, use m_receivedData:%s\n",m_receivedData.c_str());
            outData = m_receivedData;
            len = outData.length();
        }
        std::vector<uint8_t> vec(outData.data(), outData.data() + outData.size());
        vecdata = vec;
        return len;
    }

    int HgComWithSocket::sendData(const std::string &data)
    {
        return sendData(sock, data);
    }

    void HgComWithSocket::closeSocket(SocketHandle s)
    {
#ifdef _WIN32
        closesocket(s);
#else
        close(s);
#endif
    }

    void HgComWithSocket::stop()
    {
        if (sock != INVALID_SOCKET)
        {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            sock = INVALID_SOCKET;
        }
    }
}