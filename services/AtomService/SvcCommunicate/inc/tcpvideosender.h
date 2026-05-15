#ifndef TCPVIDEOSENDER_H
#define TCPVIDEOSENDER_H

#include <string>
#include <thread>
#include <atomic>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class TCPVideoSender {
public:
    TCPVideoSender(const std::string& url);
    ~TCPVideoSender();

    bool start(const std::string& videoFile);
    void stop();

private:
    void run(const std::string& videoFile);

private:
    std::string m_url;
    std::atomic<bool> m_running;
    std::thread m_thread;
};

#endif
#endif