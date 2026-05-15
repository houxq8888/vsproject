set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfPackFunc inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGHwLowerPC
    HGBaseConfig
    HGSvcFrame
    HGSvcSecurity
    HGSvcCommunicate
    HGSvcAuthority
    HGSvcSave
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfPackFunc inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
    HGBaseConfig
    HGSvcFrame
    HGSvcSecurity
    HGSvcCommunicate
    HGSvcAuthority
    HGSvcSave
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfPackFunc inc src 
    ../../services/BaseCommon/inc                       ../../services/BaseCommon/src
    ../../services/BaseConfig/inc                       ../../services/BaseConfig/src
    ../../services/Hardware/HwLowerPC/inc               ../../services/Hardware/HwLowerPC/src
    ../../services/Hardware/HwSensor/inc                ../../services/Hardware/HwSensor/src
    ../../services/AtomService/SvcSecurity/inc          ../../services/AtomService/SvcSecurity/src
    ../../services/AtomService/SvcLog/inc               ../../services/AtomService/SvcLog/src
    ../../services/AtomService/SvcSave/inc              ../../services/AtomService/SvcSave/src
    ../../services/AtomService/SvcFrame/inc             ../../services/AtomService/SvcFrame/src
    ../../services/AtomService/SvcCommunicate/inc       ../../services/AtomService/SvcCommunicate/src
    ../../services/AtomService/SvcImageAlgorithm/inc    ../../services/AtomService/SvcImageAlgorithm/src
    ../../services/AtomService/SvcAuthority/inc         ../../services/AtomService/SvcAuthority/src
    ../../modules/CoreCupDetModule/inc                  ../../modules/CoreCupDetModule/src
    ../../interfaces/IfCameraRecognize/inc              ../../interfaces/IfCameraRecognize/src
)

target_compile_definitions(HGIfPackFunc PRIVATE LIBSSH_STATIC)
target_link_libraries(HGIfPackFunc
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ${HG_PROJECT_ROOT_FS}/lib/libssh.a
    ${HG_PROJECT_ROOT_FS}/lib/libzlibstatic.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(HGIfPackFunc PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()