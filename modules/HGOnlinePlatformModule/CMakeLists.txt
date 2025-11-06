set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(OnlinePlatformModule inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGLowerPC
    HGFrameService
    HGSaveService
    HGConfig
    HGCommunicateService
    HGAuthorityService
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
HG_CMAKE_ADD_SHARED(HGOnlinePlatformModule inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGLowerPC
    HGSaveService
    HGConfig
    HGFrameService
    HGCommunicateService
    HGAuthorityService
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
HG_CMAKE_ADD_STATIC(HGOnlinePlatformModule inc src)
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    HGLowerPC
    HGFrameService
    HGSaveService
    HGCommunicateService
    HGAuthorityService
)
endif()

set_target_properties(HGOnlinePlatformModule PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
