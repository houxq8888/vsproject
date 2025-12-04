// 预编译头文件 - HGPro项目
// 包含常用的系统头文件和第三方库头文件

#ifndef STDAFX_H
#define STDAFX_H

// C标准库（使用C风格头文件，兼容性更好）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

// C++标准库（仅在C++模式下包含）
#ifdef __cplusplus
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <regex>
#endif

// 系统相关
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

// 网络相关
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/epoll.h>

// 进程和线程相关
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

// 文件系统相关
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/select.h>

// 数学库（仅在C++模式下包含）
#ifdef __cplusplus
#include <complex>
#include <random>
#include <numeric>
#endif

// 时间和日期
#include <time.h>
#include <sys/time.h>
// C++时间库（仅在C++模式下包含）
#ifdef __cplusplus
#include <ctime>
#endif

// Qt库（如果项目使用Qt）
#ifdef USE_QT
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>
#endif

// OpenCV库（如果项目使用OpenCV）
#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#endif

// 项目通用定义
#include "common_defs.h"

#endif // STDAFX_H