set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(V6Module inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    HGOnlinePlatformModule
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGV6Module inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    HGOnlinePlatformModule
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGV6Module inc src
    ../../services/HGAtomService/HGFrameService/inc                      ../../services/HGAtomService/HGFrameService/src
)


HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    HGLowerPC
    HGFrameService
    HGSaveService
    HGCommunicateService
    HGAuthorityService
    HGOnlinePlatformModule
)
endif()

set_target_properties(HGV6Module PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
