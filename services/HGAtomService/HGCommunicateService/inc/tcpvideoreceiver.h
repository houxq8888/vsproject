#ifndef TCPVIDEORECEIVER_H
#define TCPVIDEORECEIVER_H 

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else

#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <mutex>  // 保护客户端列表

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
}

class TCPVideoReceiver {
public:
    enum class Mode {Client, Server};
    using FrameCallback = std::function<void(uint8_t* data, int width, int height)>;

    explicit TCPVideoReceiver(const std::string& url, Mode mode);
    ~TCPVideoReceiver();

    void start(FrameCallback callback);
    void stop();

private:
    int openClientSocket(const std::string& host, int port);
    int openServerSocket(int port);
    void run();
    void runTCP();
    void runFFmpegURL();
    void handleTCPConnection(int sock_fd);             // TCP-MPEGTS
    void handleFFmpegStream(AVFormatContext* fmt_ctx); // RTMP或解码后的FFmpeg上下文

    std::string m_url;
    Mode m_mode;
    std::atomic<bool> m_running;
    FrameCallback m_callback;
    std::thread m_thread;

    // TCP Server 相关
    int m_serverFd = -1;                         // 当前 Server socket
    std::vector<int> m_clientFds;                // 当前连接的 Client sockets
    std::mutex m_clientMutex;                    // 保护 m_clientFds 多线程访问
};

#endif

#endif
