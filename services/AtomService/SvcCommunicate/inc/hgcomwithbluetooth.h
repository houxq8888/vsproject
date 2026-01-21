#ifndef HG_COM_WITH_BLUETOOTH_H
#define HG_COM_WITH_BLUETOOTH_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <string>

class HgComWithBluetooth {
public:
    explicit HgComWithBluetooth(const std::string& deviceMac);

    // 连接蓝牙设备
    bool connect();

    // 播放 WAV 文件
    bool playWavFile(const std::string& wavFilePath);

private:
    std::string macAddress_;
};
#endif
#endif // HG_COM_WITH_BLUETOOTH_H
