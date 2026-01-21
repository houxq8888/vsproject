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
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
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
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
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
)
endif()

set_target_properties(HGCoreOnlinePlatform PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
