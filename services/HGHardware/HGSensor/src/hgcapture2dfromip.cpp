#include "hgcapture2dfromip.h"
#include <chrono>
#include <thread>

namespace HGMACHINE {
HGCapture2DFromIP::HGCapture2DFromIP()
{
}
HGCapture2DFromIP::~HGCapture2DFromIP()
{
    close();
}
bool HGCapture2DFromIP::open()
{
    return true;
}
bool HGCapture2DFromIP::open(const int &index)
{
    try {
        m_index = index;
        std::cout << "正在尝试打开摄像头，索引: " << index << std::endl;
        
        // 尝试打开摄像头
        if (!m_cap.open("/dev/video" + std::to_string(index),cv::CAP_V4L2)) {
            std::cerr << "无法打开摄像头，索引: " << index << std::endl;
            return false;
        }
        // 等待设备准备好
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // 等待 500 毫秒

        // 手动设置分辨率为 1920x1080
        m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 5000);
        m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 5000);
        m_cap.set(cv::CAP_PROP_FPS, 30);  // 设置帧率为30fps

        // 获取视频帧宽度和高度
        double width = m_cap.get(cv::CAP_PROP_FRAME_WIDTH);
        double height = m_cap.get(cv::CAP_PROP_FRAME_HEIGHT);

        std::cout << "视频帧尺寸: " << width << " x " << height << std::endl;

        // 如果没有正确设置分辨率，则输出警告
        if (width == 0 || height == 0) {
            std::cerr << "警告: 摄像头分辨率设置失败！" << std::endl;
        }

        return true;
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV 异常: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "标准库异常: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "未知错误！" << std::endl;
        return false;
    }
}


void HGCapture2DFromIP::close()
{
    m_cap.release();
}
int HGCapture2DFromIP::getFrameOne(cv::Mat &frame)
{
    std::cout << "Attempting to grab a frame..." << std::endl;

    if (!m_cap.isOpened()) {
        std::cerr << "Error: Camera not opened properly!" << std::endl;
        return -1;
    }

    int fail_count = 0;
    while (fail_count < 5)
    {
        bool grabbed = m_cap.grab(); // 尝试抓取一帧
        if (!grabbed) {
            std::cerr << "Failed to grab frame (" << fail_count + 1 << "/5)" << std::endl;
            fail_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 等待一段时间再试
            continue;
        }

        bool success = m_cap.retrieve(frame);  // 从缓冲区获取帧
        if (!frame.empty()) {
            std::cout << "Successfully retrieved a frame." << std::endl;
            return 0;
        }

        fail_count++;
    }

    std::cerr << "Failed to grab and retrieve frame after multiple attempts." << std::endl;
    return -1;  // 返回失败
}

}
