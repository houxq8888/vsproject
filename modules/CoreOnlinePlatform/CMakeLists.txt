set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(CoreOnlinePlatform inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGHwLowerPC
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
    HGSvcCommunicate
    HGSvcAuthority
    HGSvcLog
    HGIfSerial
    HGCoreSerial
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCoreOnlinePlatform inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
    HGSvcCommunicate
    HGSvcAuthority
    HGSvcLog
    HGIfSerial
    HGCoreSerial
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCoreOnlinePlatform inc src)
HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
    HGSvcCommunicate
    HGSvcAuthority
    HGSvcLog
    HGIfSerial
    HGCoreSerial
)
endif()

set_target_properties(HGCoreOnlinePlatform PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
