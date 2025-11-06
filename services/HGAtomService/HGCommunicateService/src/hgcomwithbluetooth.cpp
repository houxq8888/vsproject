#include "hgcomwithbluetooth.h"
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <pulse/simple.h>
#include <pulse/error.h>

HgComWithBluetooth::HgComWithBluetooth(const std::string& deviceMac)
    : macAddress_(deviceMac) { }

bool HgComWithBluetooth::connect() {
    std::string cmd = "echo -e 'connect " + macAddress_ + "' | bluetoothctl";
    int result = std::system(cmd.c_str());
    if (result != 0) {
        std::cerr << "[Bluetooth] Connection failed for " << macAddress_ << std::endl;
        return false;
    }
    std::cout << "[Bluetooth] Connected to " << macAddress_ << std::endl;
    return true;
}

bool HgComWithBluetooth::playWavFile(const std::string& wavFilePath) {
    pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };

    int error;
    pa_simple *s = pa_simple_new(NULL, "HgBluetoothAudio", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error);
    if (!s) {
        std::cerr << "[PulseAudio] pa_simple_new() failed: " << pa_strerror(error) << std::endl;
        return false;
    }

    std::ifstream file(wavFilePath, std::ios::binary);
    if (!file) {
        std::cerr << "[File] Cannot open WAV file: " << wavFilePath << std::endl;
        pa_simple_free(s);
        return false;
    }

    file.seekg(44); // Skip WAV header
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), {});
    file.close();

    if (pa_simple_write(s, buffer.data(), buffer.size(), &error) < 0) {
        std::cerr << "[PulseAudio] Write failed: " << pa_strerror(error) << std::endl;
        pa_simple_free(s);
        return false;
    }

    pa_simple_drain(s, &error);
    pa_simple_free(s);
    return true;
}


// 使用前确保已经 trust 了蓝牙设备。
// WAV 文件必须是标准格式（44.1kHz, 16bit, 2ch PCM）。
// #include "hgcomwithbluetooth.h"
// #include <iostream>

// int main() {
//     HgComWithBluetooth player("XX:XX:XX:XX:XX:XX");  // 替换为你的蓝牙地址

//     if (!player.connect()) {
//         return 1;
//     }

//     if (!player.playWavFile("example.wav")) {
//         return 1;
//     }

//     return 0;
// }
#endif