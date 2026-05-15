set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(CoreV6 inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGHwLowerPC
    HGBaseConfig
    HGSvcFrame
    HGSvcSecurity
    HGSvcLog
    HGSvcCommunicate
    HGSvcSave
    HGCoreOnlinePlatform
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCoreV6 inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
    HGBaseConfig
    HGSvcFrame
    HGSvcSecurity
    HGSvcLog
    HGSvcCommunicate
    HGSvcSave
    HGCoreOnlinePlatform
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCoreV6 inc src
    ../../services/AtomService/SvcFrame/inc                      ../../services/AtomService/SvcFrame/src
    ../../services/AtomService/SvcLog/inc                        ../../services/AtomService/SvcLog/src
)


HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
    HGBaseConfig
    HGSvcFrame
    HGSvcSecurity
    HGSvcLog
    HGSvcCommunicate
    HGSvcSave
    HGCoreOnlinePlatform
)
endif()

set_target_properties(HGCoreV6 PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
