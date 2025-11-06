#include "systemusage.h"

    double SystemUsage::getCpuUsage() {
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        // Windows平台的CPU使用率获取
        FILETIME idleTime, kernelTime, userTime;
        GetSystemTimes(&idleTime, &kernelTime, &userTime);

        static FILETIME prevIdleTime = idleTime, prevKernelTime = kernelTime, prevUserTime = userTime;

        ULARGE_INTEGER prevIdle, prevKernel, prevUser, idle, kernel, user;
        prevIdle.LowPart = prevIdleTime.dwLowDateTime;
        prevIdle.HighPart = prevIdleTime.dwHighDateTime;
        prevKernel.LowPart = prevKernelTime.dwLowDateTime;
        prevKernel.HighPart = prevKernelTime.dwHighDateTime;
        prevUser.LowPart = prevUserTime.dwLowDateTime;
        prevUser.HighPart = prevUserTime.dwHighDateTime;

        idle.LowPart = idleTime.dwLowDateTime;
        idle.HighPart = idleTime.dwHighDateTime;
        kernel.LowPart = kernelTime.dwLowDateTime;
        kernel.HighPart = kernelTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;

        ULONGLONG prevTotal = prevKernel.QuadPart + prevUser.QuadPart;
        ULONGLONG total = kernel.QuadPart + user.QuadPart;

        double usage = 100.0 * (total - prevTotal) / (total + idle.QuadPart - prevIdle.QuadPart);

        prevIdleTime = idleTime;
        prevKernelTime = kernelTime;
        prevUserTime = userTime;

        return usage;
        #else
        // Linux平台的CPU使用率获取
        std::ifstream file("/proc/stat");
        std::string line;
        std::getline(file, line); // Get the first line (cpu info)

        long user, nice, system, idle;
        sscanf(line.c_str(), "cpu  %ld %ld %ld %ld", &user, &nice, &system, &idle);

        static long prevUser = 0, prevNice = 0, prevSystem = 0, prevIdle = 0;

        long total = user + nice + system + idle;
        long prevTotal = prevUser + prevNice + prevSystem + prevIdle;

        // Check if the previous total and current total are the same, avoid division by zero
        if (total == prevTotal) {
            return 0.0;  // If no change in total, return 0% usage (or handle it in any other way)
        }

        // Calculate CPU usage percentage
        double usage = 100.0 * (total - prevTotal - (user - prevUser) - (nice - prevNice)) / (total - prevTotal);

        // Update previous values
        prevUser = user;
        prevNice = nice;
        prevSystem = system;
        prevIdle = idle;

        return usage;
        #endif
    }

    long SystemUsage::getMemoryUsage() {
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        // Windows平台的内存使用量获取
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        return statex.ullTotalPhys - statex.ullAvailPhys; // Used memory in bytes
        #else
        // Linux平台的内存使用量获取
        std::ifstream file("/proc/meminfo");
        std::string line;
        long totalMemory = 0, freeMemory = 0;

        while (std::getline(file, line)) {
            if (line.find("MemTotal") != std::string::npos)
                sscanf(line.c_str(), "MemTotal: %ld kB", &totalMemory);
            if (line.find("MemFree") != std::string::npos)
                sscanf(line.c_str(), "MemFree: %ld kB", &freeMemory);
        }

        return (totalMemory - freeMemory) / 1024;  // Used memory in MB
        #endif
    }
    // 获取当前网络使用率（KB/s）
    double SystemUsage::getNetworkUsage() {
        // 读取当前的网络统计
        NetworkStats currentStats = getNetworkStats();
        
        // 计算网络接收和发送的变化率
        double receivedRate = (currentStats.bytesReceived - prevBytesReceived) / 1024.0;  // KB/s
        double sentRate = (currentStats.bytesSent - prevBytesSent) / 1024.0;  // KB/s
        
        // 更新静态变量以便下次计算
        prevBytesReceived = currentStats.bytesReceived;
        prevBytesSent = currentStats.bytesSent;

        // 返回网络使用率
        return (receivedRate + sentRate) / 2.0;  // 平均网络带宽使用率（KB/s）
    }

    // 获取当前磁盘IO读写率（KB/s）
    double SystemUsage::getDiskIO() {
        // 读取当前的磁盘IO统计
        DiskStats currentStats = getDiskStats();
        
        // 计算磁盘的读写速度
        double readRate = (currentStats.readBytes - prevReadBytes) / 1024.0;  // KB/s
        double writeRate = (currentStats.writeBytes - prevWriteBytes) / 1024.0;  // KB/s
        
        // 更新静态变量以便下次计算
        prevReadBytes = currentStats.readBytes;
        prevWriteBytes = currentStats.writeBytes;

        // 返回磁盘I/O使用率
        return (readRate + writeRate) / 2.0;  // 平均磁盘 I/O 读写率（KB/s）
    }
    NetworkStats SystemUsage::getNetworkStats() {
        NetworkStats stats = {0, 0};

        #ifdef _WIN32
            // Windows系统获取网络流量统计
            MIB_IFTABLE ifTable;
            DWORD dwSize = sizeof(MIB_IFTABLE);

            // 获取接口信息
            if (GetIfTable(&ifTable, &dwSize, FALSE) == NO_ERROR) {
                for (DWORD i = 0; i < ifTable.dwNumEntries; i++) {
                    MIB_IFROW ifRow = ifTable.table[i];

                    // 假设我们关注的是“Ethernet”接口 (名字需要匹配)
                    if (strcmp((char*)ifRow.bDescr, "Ethernet") == 0) {
                        stats.bytesReceived = ifRow.dwInOctets;
                        stats.bytesSent = ifRow.dwOutOctets;
                        break;
                    }
                }
            }
        #else
            // Linux系统获取网络流量统计
            std::ifstream file("/proc/net/dev");
            std::string line;
        
            while (std::getline(file, line)) {
                if (line.find("eth0") != std::string::npos) {  // 根据需要选择接口（如 eth0, wlan0 等）
                    std::istringstream iss(line);
                    std::string iface;
                    long long bytesReceived, bytesSent;
                    iss >> iface >> bytesReceived;  // 接收字节数
                    iss >> bytesSent;  // 发送字节数
                
                    stats.bytesReceived = bytesReceived;
                    stats.bytesSent = bytesSent;
                    break;
                }
            }
        #endif
        
        return stats;
    }
    DiskStats SystemUsage::getDiskStats() {
    DiskStats stats = {0, 0};

#ifdef _WIN32
    // Windows系统获取磁盘统计
    // 获取第一个物理磁盘的信息
    HANDLE hDisk = CreateFile(_T("\\\\.\\PhysicalDrive0"), GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

    if (hDisk != INVALID_HANDLE_VALUE) {
        DISK_PERFORMANCE dp;
        DWORD bytesRead;
        if (DeviceIoControl(hDisk, IOCTL_DISK_PERFORMANCE, nullptr, 0, &dp, sizeof(dp), &bytesRead, nullptr)) {
            stats.readBytes = dp.BytesRead.QuadPart;
            stats.writeBytes = dp.BytesWritten.QuadPart;
        }
        CloseHandle(hDisk);
    }
#else
    // Linux系统获取磁盘统计
    std::ifstream file("/proc/diskstats");
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("sda") != std::string::npos) {  // 根据需要选择磁盘（如 sda, sdb 等）
            std::istringstream iss(line);
            std::string dev;
            long long readIO, writeIO;
            long long readBytes, writeBytes;

            iss >> dev >> readIO >> writeIO;
            iss >> readBytes >> writeBytes;

            stats.readBytes = readBytes;
            stats.writeBytes = writeBytes;
            break;
        }
    }
#endif

    return stats;
}