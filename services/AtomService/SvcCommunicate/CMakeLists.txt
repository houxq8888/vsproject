set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



HG_CMAKE_ADD_MODULE(SvcCommunicate inc src)
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGBaseConfig
    HGSvcFrame
    HGSvcLog
    ${VSOMEIP_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${LIBSSH_LIBRARIES}
    ${OpenCV_LIBRARIES}  
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)

if (PLATFORM STREQUAL "x64")

HG_CMAKE_ADD_SHARED(HGSvcCommunicate inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGBaseConfig
    HGSvcFrame
    HGSvcLog
    ${VSOMEIP_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${LIBSSH_LIBRARIES}
    ${OpenCV_LIBRARIES}  
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)

elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGSvcCommunicate inc src)
HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGBaseConfig
    HGSvcFrame
    HGSvcLog
    ${VSOMEIP_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${LIBSSH_LIBRARIES}
    ${OpenCV_LIBRARIES} 
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)

endif()

set_target_properties(HGSvcCommunicate PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
