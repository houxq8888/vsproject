#ifndef DASHBOARDRWDB_H
#define DASHBOARDRWDB_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#include <shlwapi.h>
#include <direct.h>
// #include <Psapi.h>
#include <iphlpapi.h>
#include <wincrypt.h>
#include <io.h>
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#define HGDashboardModule_LIBRARY
#if defined(HGDashboardModule_LIBRARY)
#  define HGDashboardModule_EXPORT Q_DECL_EXPORT
#else
#  define HGDashboardModule_EXPORT Q_DECL_IMPORT
#endif

#include <string>
#include "hgsavedatatodb.h"

namespace HGMACHINE{
    #define DASHBOARDDBNAME "dashboardData"
    #define DASHBOARD_MAINTENANCE_TABLE "maintenance"
    #define DASHBOARD_DEVICE_MAP_TABLE "deviceMap"
    #define DASHBOARDDB_PATH "/home/database/HGDashboard.db"

    class /*HGDashboardModule_EXPORT*/ DashboardRWDB{
    public:
        DashboardRWDB();
        ~DashboardRWDB();
        static void openDB(const std::string &basePath="");
        static void closeDB();
        static void clearDB(const std::string &tableName);

        static void writeDashboardDataInfo(std::string tableName,
            const std::map<std::string,std::string> &contents);
        static std::vector<std::map<std::string,std::string>> readDashboardDataInfo(std::string tableName);

        static void writeMaintenanceInfo(const std::map<std::string,std::string> &contents,const int& type);
        static std::map<std::string,std::string> readMaintenanceInfo(const int& type);

        static void writeDeviceMapInfo(const std::map<std::string,std::string> &info);
        static std::vector<std::map<std::string,std::string>> readDeviceMapInfo();

    private:
        static HGSaveDataToDB dbOpera;
    };
}

#endif
