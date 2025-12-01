set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(OnlinePlatformInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGOnlinePlatformModule
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGAuthorityService
    HGSaveService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    OpenSSL::Crypto
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)

HG_CMAKE_ADD_SHARED(HGOnlinePlatformInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGOnlinePlatformModule
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGAuthorityService
    HGCommunicateService
    HGSaveService
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    OpenSSL::Crypto
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)
set_target_properties(HGOnlinePlatformInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
