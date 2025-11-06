#ifndef SYSTEMUSAGE_H
#define SYSTEMUSAGE_H 

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>   // Windows API
#include <winioctl.h>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#endif

// 网络统计结构体
    struct NetworkStats {
        long long bytesReceived;
        long long bytesSent;
    };

    // 磁盘IO统计结构体
    struct DiskStats {
        long long readBytes;
        long long writeBytes;
    };

class SystemUsage {
public:
    // 获取当前CPU使用率
    static double getCpuUsage();

    // 获取当前内存使用量（单位MB）
    static long getMemoryUsage();
    // 获取当前网络使用率（KB/s）
    static double getNetworkUsage();
    // 获取当前磁盘IO读写率（KB/s）
    static double getDiskIO();
private:
    
    // 静态变量，用于保存上次的CPU和内存使用数据，以便增量计算
    static inline long prevBytesReceived = 0, prevBytesSent = 0;
    static inline long prevReadBytes = 0, prevWriteBytes = 0;

    // 获取当前网络统计数据
    static NetworkStats getNetworkStats();

    // 获取当前磁盘IO统计数据
    static DiskStats getDiskStats();

};

#endif