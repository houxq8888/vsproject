set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreUserAudit inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGSvcLog
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
    HGCoreLog
    HGCoreSystemData
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCoreUserAudit inc src)
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCoreUserAudit inc src
        ../../modules/CoreSystemData/inc
        ../../modules/CoreSystemData/src)
endif()

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGSvcLog
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
    HGCoreLog
    HGCoreSystemData
)

set_target_properties(HGCoreUserAudit PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
