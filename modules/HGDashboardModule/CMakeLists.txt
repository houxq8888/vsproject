set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(DashboardModule inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGSaveService
    HGConfig
    HGCommunicateService
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${PULSE_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    ${HARU_LIBRARY}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    /usr/lib/x86_64-linux-gnu/libpulse.so
)

if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGDashboardModule inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGSaveService
    HGConfig
    HGCommunicateService
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${PULSE_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    ${HARU_LIBRARY}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGDashboardModule inc src)
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    HGSaveService
    HGCommunicateService
)
endif()

set_target_properties(HGDashboardModule PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
