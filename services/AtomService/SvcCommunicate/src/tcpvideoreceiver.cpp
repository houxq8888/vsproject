#include "tcpvideoreceiver.h"
#include <iostream>
#include <unistd.h>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <arpa/inet.h>
#include <regex>
#include <fcntl.h>      // fcntl(), O_NONBLOCK
#include <unistd.h>     // close()
#include <sys/types.h>  // socket 类型
#include <sys/socket.h> // socket 函数
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet_addr

TCPVideoReceiver::TCPVideoReceiver(const std::string& url,Mode mode)
    : m_url(url), m_mode(mode), m_running(false)
{
    av_register_all();
    avformat_network_init();
}

TCPVideoReceiver::~TCPVideoReceiver()
{
    stop();
}

void TCPVideoReceiver::start(FrameCallback callback)
{
    if (m_running) return;
    m_callback = callback;
    m_running = true;
    m_thread = std::thread(&TCPVideoReceiver::run, this);
}

void TCPVideoReceiver::stop()
{
    m_running = false;
    for (int fd:m_clientFds){
        close(fd);
    }
    if (m_serverFd>=0){
        close(m_serverFd);
        m_serverFd=-1;
    }
    if (m_thread.joinable()) m_thread.join();
}

int TCPVideoReceiver::openClientSocket(const std::string &host, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
}

int TCPVideoReceiver::openServerSocket(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        return -1;
    }

    fcntl(server_fd, F_SETFL, O_NONBLOCK); // 非阻塞 accept
    m_serverFd = server_fd;
    std::cout << "Server listening on port " << port << "..." << std::endl;
    return server_fd;
}

void TCPVideoReceiver::run()
{
    // 判断 URL 类型
    if (m_url.find("tcp://") == 0)
    {
        runTCP();
    }
    else
    {
        runFFmpegURL();
    }
}
void TCPVideoReceiver::runTCP()
{
    std::regex re(R"(tcp://([^:]*):(\d+))");
    std::smatch match;
    std::string host = "0.0.0.0";
    int port = 8885;
    if (std::regex_match(m_url, match, re)) {
        host = match[1];
        port = std::stoi(match[2]);
    }

    if (m_mode == Mode::Client) {
        int sock_fd = openClientSocket(host, port);
        if (sock_fd < 0) {
            std::cerr << "Failed to connect TCP server" << std::endl;
            return;
        }
        handleTCPConnection(sock_fd);
        close(sock_fd);
        return;
    }

    // Server 模式
    int server_fd = openServerSocket(port);
    if (server_fd < 0) {
        std::cerr << "Failed to open TCP server socket" << std::endl;
        return;
    }

    std::vector<std::thread> clientThreads;

    while (m_running) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        m_clientFds.push_back(client_fd);

        clientThreads.emplace_back([this, client_fd]() {
            handleTCPConnection(client_fd);
            close(client_fd);
        });
    }

    // 等待客户端线程结束
    for (auto &t : clientThreads) {
        if (t.joinable()) t.join();
    }
}

void TCPVideoReceiver::handleTCPConnection(int sock_fd)
{
    const int buffer_size = 4096;
    unsigned char* avio_buffer = (unsigned char*)av_malloc(buffer_size);
    if (!avio_buffer) return;

    auto read_packet = [](void* opaque, uint8_t* buf, int buf_size) -> int {
        int fd = *(int*)opaque;
        ssize_t len = read(fd, buf, buf_size);
        if (len == 0) return AVERROR_EOF;
        if (len < 0) return -1;
        return static_cast<int>(len);
    };

    AVIOContext* avio_ctx = avio_alloc_context(
        avio_buffer, buffer_size, 0, &sock_fd, read_packet, nullptr, nullptr);

    AVFormatContext* fmt_ctx = avformat_alloc_context();
    fmt_ctx->pb = avio_ctx;
    fmt_ctx->iformat = av_find_input_format("mpegts");

    if (avformat_open_input(&fmt_ctx, nullptr, nullptr, nullptr) < 0) {
        avio_context_free(&avio_ctx);
        return;
    }
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        avformat_close_input(&fmt_ctx);
        avio_context_free(&avio_ctx);
        return;
    }

    handleFFmpegStream(fmt_ctx);

    avformat_close_input(&fmt_ctx);
    avio_context_free(&avio_ctx);
}
////////////////////// RTMP / FFmpeg URL //////////////////////
static int interrupt_cb(void* opaque) {
    std::atomic<bool>* running = (std::atomic<bool>*)opaque;
    return !running->load();
}
void TCPVideoReceiver::runFFmpegURL()
{
    // add reconnect
    while (m_running)
    {
        AVFormatContext *fmt_ctx = nullptr;
        AVDictionary *opts = nullptr;
        // 强制使用 TCP 传输
        av_dict_set(&opts, "rtmp_transport", "tcp", 0);
        av_dict_set(&opts, "reconnect", "1", 0);
        av_dict_set(&opts, "reconnect_streamed", "1", 0);
        av_dict_set(&opts, "reconnect_delay_max", "5", 0);
        // 设置超时，避免阻塞太久 (单位: 微秒)
        av_dict_set(&opts, "stimeout", "5000000", 0); // 5s timeout

        av_dict_set(&opts, "rtmp_live", "live", 0);
        AVFormatContext *tmp_ctx = avformat_alloc_context();
        tmp_ctx->interrupt_callback.callback = interrupt_cb;
        tmp_ctx->interrupt_callback.opaque = &m_running;

        fmt_ctx = tmp_ctx;

        if (avformat_open_input(&fmt_ctx, m_url.c_str(), nullptr, &opts) < 0)
        {
            std::cerr << "[TCPVideoReceiver] avformat_open_input failed (no stream or server not ready): "
              << m_url << ", retrying in 2s..." << std::endl;
            avformat_free_context(fmt_ctx);
            av_dict_free(&opts);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        if (avformat_find_stream_info(fmt_ctx, nullptr) < 0)
        {
            std::cerr << "[TCPVideoReceiver] failed to get stream info, retrying in 2s..." << std::endl;
            avformat_close_input(&fmt_ctx);
            av_dict_free(&opts);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        std::cout << "[TCPVideoReceiver] Stream started!" << std::endl;
        handleFFmpegStream(fmt_ctx);
        avformat_close_input(&fmt_ctx);
        av_dict_free(&opts);
        std::cerr << "[TCPVideoReceiver] Stream ended or error, retrying in 2s..." << std::endl;
        // handleFFmpegStream 跳出说明流断了，休眠后再重试
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
// void TCPVideoReceiver::runFFmpegURL()
// {
//     while (m_running)
//     {
//         int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock_fd < 0) {
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             continue;
//         }

//         sockaddr_in addr{};
//         addr.sin_family = AF_INET;
//         addr.sin_port = htons(8885);  // RTMP 端口
//         inet_pton(AF_INET, "192.168.1.131", &addr.sin_addr);

//         if (connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
//             close(sock_fd);
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             continue;
//         }

//         const int buffer_size = 4096;
//         uint8_t* avio_buffer = (uint8_t*)av_malloc(buffer_size);
//         if (!avio_buffer) {
//             close(sock_fd);
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             continue;
//         }

//         // 自定义 read_packet 回调
//         auto read_packet = [](void* opaque, uint8_t* buf, int buf_size) -> int {
//             int fd = *(int*)opaque;
//             ssize_t ret = recv(fd, buf, buf_size, 0);
//             if (ret == 0) return AVERROR_EOF;    // 对端断开
//             if (ret < 0) return AVERROR(EAGAIN); // 暂时无数据
//             return static_cast<int>(ret);
//         };

//         AVIOContext* avio_ctx = avio_alloc_context(
//             avio_buffer, buffer_size, 0, &sock_fd, read_packet, nullptr, nullptr);

//         AVFormatContext* fmt_ctx = avformat_alloc_context();
//         fmt_ctx->pb = avio_ctx;
//         fmt_ctx->iformat = av_find_input_format("flv"); // RTMP 使用 FLV 封装

//         if (avformat_open_input(&fmt_ctx, nullptr, nullptr, nullptr) < 0) {
//             std::cerr << "[TCPVideoReceiver] waiting for stream..." << std::endl;
//             avio_context_free(&avio_ctx);
//             avformat_free_context(fmt_ctx);
//             close(sock_fd);
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             continue;
//         }

//         if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
//             std::cerr << "[TCPVideoReceiver] failed to get stream info, retrying..." << std::endl;
//             avformat_close_input(&fmt_ctx);
//             avio_context_free(&avio_ctx);
//             close(sock_fd);
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             continue;
//         }

//         std::cout << "[TCPVideoReceiver] Stream started!" << std::endl;
//         handleFFmpegStream(fmt_ctx);

//         avformat_close_input(&fmt_ctx);
//         avio_context_free(&avio_ctx);
//         close(sock_fd);

//         std::cerr << "[TCPVideoReceiver] Stream ended or error, retrying in 2s..." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//     }
// }

void TCPVideoReceiver::handleFFmpegStream(AVFormatContext *fmt_ctx)
{
    int video_index = -1;
    for (unsigned i = 0; i < fmt_ctx->nb_streams; ++i) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }
    if (video_index < 0) return;

    AVCodecParameters* codecpar = fmt_ctx->streams[video_index]->codecpar;
    AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) return;

    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codecpar);
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        avcodec_free_context(&codec_ctx);
        return;
    }

    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    AVFrame* rgb_frame = av_frame_alloc();

    int width = codec_ctx->width;
    int height = codec_ctx->height;
    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(num_bytes);
    av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, buffer,
                         AV_PIX_FMT_RGB24, width, height, 1);

    SwsContext* sws_ctx = sws_getContext(width, height, codec_ctx->pix_fmt,
                                         width, height, AV_PIX_FMT_RGB24,
                                         SWS_BILINEAR, nullptr, nullptr, nullptr);

    while (m_running) {
        int ret = av_read_frame(fmt_ctx, pkt);
        if (ret < 0) break;

        if (pkt->stream_index == video_index) {
            if (avcodec_send_packet(codec_ctx, pkt) == 0) {
                while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                    sws_scale(sws_ctx, frame->data, frame->linesize, 0, height,
                              rgb_frame->data, rgb_frame->linesize);

                    if (m_callback) {
                        std::vector<uint8_t> rgb_copy(rgb_frame->data[0],
                                                      rgb_frame->data[0] + width * height * 3);
                        m_callback(rgb_copy.data(), width, height);
                    }
                }
            }
        }
        av_packet_unref(pkt);
    }

    sws_freeContext(sws_ctx);
    av_free(buffer);
    av_frame_free(&rgb_frame);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&codec_ctx);
}
#endif