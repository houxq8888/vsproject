// HGPro项目通用定义文件

#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

// 平台检测
#if defined(_WIN32) || defined(_WIN64)
    #define HG_WINDOWS 1
    #define HG_LINUX 0
#elif defined(__linux__)
    #define HG_WINDOWS 0
    #define HG_LINUX 1
#else
    #define HG_WINDOWS 0
    #define HG_LINUX 0
#endif

// 编译器检测
#if defined(__GNUC__)
    #define HG_GCC 1
    #define HG_CLANG 0
    #define HG_MSVC 0
#elif defined(__clang__)
    #define HG_GCC 0
    #define HG_CLANG 1
    #define HG_MSVC 0
#elif defined(_MSC_VER)
    #define HG_GCC 0
    #define HG_CLANG 0
    #define HG_MSVC 1
#else
    #define HG_GCC 0
    #define HG_CLANG 0
    #define HG_MSVC 0
#endif

// 调试模式检测
#ifdef NDEBUG
    #define HG_DEBUG 0
    #define HG_RELEASE 1
#else
    #define HG_DEBUG 1
    #define HG_RELEASE 0
#endif

// 常用宏定义
#define HG_UNUSED(x) (void)(x)
#define HG_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define HG_SAFE_DELETE(p) do { delete (p); (p) = nullptr; } while(0)
#define HG_SAFE_DELETE_ARRAY(p) do { delete[] (p); (p) = nullptr; } while(0)

// 内联函数宏
#if HG_GCC || HG_CLANG
    #define HG_INLINE inline __attribute__((always_inline))
    #define HG_NOINLINE __attribute__((noinline))
#elif HG_MSVC
    #define HG_INLINE __forceinline
    #define HG_NOINLINE __declspec(noinline)
#else
    #define HG_INLINE inline
    #define HG_NOINLINE
#endif

// 导出/导入宏
#if HG_WINDOWS
    #ifdef HG_EXPORT
        #define HG_API __declspec(dllexport)
    #else
        #define HG_API __declspec(dllimport)
    #endif
#else
    #define HG_API
#endif

// 对齐宏
#if HG_GCC || HG_CLANG
    #define HG_ALIGN(n) __attribute__((aligned(n)))
#elif HG_MSVC
    #define HG_ALIGN(n) __declspec(align(n))
#else
    #define HG_ALIGN(n)
#endif

// 常用类型定义
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

typedef int8_t   hg_int8;
typedef uint8_t  hg_uint8;
typedef int16_t  hg_int16;
typedef uint16_t hg_uint16;
typedef int32_t  hg_int32;
typedef uint32_t hg_uint32;
typedef int64_t  hg_int64;
typedef uint64_t hg_uint64;

typedef float    hg_float32;
typedef double   hg_float64;

// 常用常量
#ifdef __cplusplus
namespace hg {
    constexpr double PI = 3.14159265358979323846;
    constexpr double E = 2.71828182845904523536;
    constexpr int MAX_PATH_LENGTH = 4096;
    constexpr int BUFFER_SIZE = 1024;
}
#else
#define HG_PI 3.14159265358979323846
#define HG_E 2.71828182845904523536
#define HG_MAX_PATH_LENGTH 4096
#define HG_BUFFER_SIZE 1024
#endif

#endif // COMMON_DEFS_H