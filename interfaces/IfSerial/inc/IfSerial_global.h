#ifndef IFSERIAL_GLOBAL_H
#define IFSERIAL_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
// #include <Shlwapi.h>
#include <direct.h>
// #include <Psapi.h>
#include <iphlpapi.h>
#include <wincrypt.h>
#include <io.h>
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#include <unistd.h>
#endif

#if defined(IFSERIAL_LIBRARY)
#  define IFSERIAL_EXPORT Q_DECL_EXPORT
#else
#  define IFSERIAL_EXPORT Q_DECL_IMPORT
#endif


#define IFSERIALNAME "[IfSerial]"
#define IFSERIALAUTHOR "[HG xiaoqin.hou]"


#endif // IFSERIAL_GLOBAL_H