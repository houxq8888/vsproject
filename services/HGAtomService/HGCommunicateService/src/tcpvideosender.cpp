#include "tcpvideosender.h"
#include <iostream>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <regex>

TCPVideoSender::TCPVideoSender(const std::string& url)
    : m_url(url), m_running(false)
{
    av_register_all();
    avformat_network_init();
}

TCPVideoSender::~TCPVideoSender() {
    stop();
}

bool TCPVideoSender::start(const std::string& videoFile) {
    if (m_running) return false;
    m_running = true;
    m_thread = std::thread(&TCPVideoSender::run, this, videoFile);
    return true;
}

void TCPVideoSender::stop() {
    m_running = false;
    if (m_thread.joinable()) m_thread.join();
}

static int openClientSocket(const std::string& host, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

void TCPVideoSender::run(const std::string& videoFile) {
    std::regex re(R"(tcp://([^:]*):(\d+))");
    std::smatch match;
    std::string host = "127.0.0.1";
    int port = 8885;
    if (std::regex_match(m_url, match, re)) {
        host = match[1];
        port = std::stoi(match[2]);
    }

    int sock_fd = openClientSocket(host, port);
    if (sock_fd < 0) {
        std::cerr << "Failed to connect server" << std::endl;
        return;
    }

    // 打开视频文件或摄像头
    cv::VideoCapture cap;
    try
    {
        int camIndex = std::stoi(videoFile);
        cap.open(camIndex); //,cv::CAP_V4L2);
        if (!cap.isOpened())
        {
            printf("camera open failed\n");
            return;
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 5000);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 5000);
        // 获取视频帧宽度
        double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        // 获取视频帧高度
        double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

        std::cout << "视频帧尺寸: " << width << " x " << height << std::endl;
    }
    catch (...)
    {
        cap.open(videoFile);
    }
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video source" << std::endl;
        close(sock_fd);
        return;
    }

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = static_cast<int>(cap.get(cv::CAP_PROP_FPS));
    if (fps <= 0) fps = 25;

    // 初始化 FFmpeg 输出
    AVFormatContext* fmt_ctx = nullptr;
    avformat_alloc_output_context2(&fmt_ctx, nullptr, "mpegts", nullptr);
    if (!fmt_ctx) {
        close(sock_fd);
        return;
    }

    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H265); //AV_CODEC_ID_H265
    if (!codec) {
        avformat_free_context(fmt_ctx);
        close(sock_fd);
        return;
    }

    AVStream* stream = avformat_new_stream(fmt_ctx, codec);
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    codec_ctx->width = width;
    codec_ctx->height = height;
    codec_ctx->time_base = {1, fps};
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_ctx->bit_rate = 400000;

    avcodec_open2(codec_ctx, codec, nullptr);
    avcodec_parameters_from_context(stream->codecpar, codec_ctx);

    // 创建自定义 AVIO 写入 TCP
    const int avio_buf_size = 4096;
    unsigned char* avio_buf = (unsigned char*)av_malloc(avio_buf_size);

    auto write_packet = [](void* opaque, uint8_t* buf, int buf_size) -> int {
        int fd = *(int*)opaque;
        int sent = 0;
        while (sent < buf_size) {
            int n = send(fd, buf + sent, buf_size - sent, 0);
            if (n <= 0) return -1;
            sent += n;
        }
        return buf_size;
    };

    AVIOContext* avio_ctx = avio_alloc_context(avio_buf, avio_buf_size, 1, &sock_fd, nullptr, write_packet, nullptr);
    fmt_ctx->pb = avio_ctx;

    avformat_write_header(fmt_ctx, nullptr);

    AVFrame* frame = av_frame_alloc();
    frame->format = codec_ctx->pix_fmt;
    frame->width = width;
    frame->height = height;
    av_frame_get_buffer(frame, 32);

    SwsContext* sws_ctx = sws_getContext(width, height, AV_PIX_FMT_BGR24,
                                         width, height, AV_PIX_FMT_YUV420P,
                                         SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVPacket* pkt = av_packet_alloc();

    while (m_running) {
        cv::Mat img;
        if (!cap.read(img)) break;
        uint8_t* src_slices[3]={img.data,nullptr,nullptr};
        int src_stride[3] = {static_cast<int>(img.step[0]),0,0};
        sws_scale(sws_ctx, src_slices, src_stride, 0, height, frame->data, frame->linesize);

        frame->pts++;

        avcodec_send_frame(codec_ctx, frame);
        while (avcodec_receive_packet(codec_ctx, pkt) == 0) {
            av_interleaved_write_frame(fmt_ctx, pkt);
            av_packet_unref(pkt);
        }
    }

    av_write_trailer(fmt_ctx);
    sws_freeContext(sws_ctx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&codec_ctx);
    avformat_free_context(fmt_ctx);
    avio_context_free(&avio_ctx);
    close(sock_fd);
}
#endif