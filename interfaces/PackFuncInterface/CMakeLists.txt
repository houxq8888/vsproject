set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(FunInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(PackFunInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(PackFunInterface inc src 
    ../../services/HGCommon/inc                                 ../../services/HGCommon/src
    ../../services/HGConfig/inc                                 ../../services/HGConfig/src
    ../../services/HGHardware/HGLowerPC/inc                     ../../services/HGHardware/HGLowerPC/src
    ../../services/HGHardware/HGSensor/inc                      ../../services/HGHardware/HGSensor/src
    ../../services/HGAtomService/HGSecurityService/inc          ../../services/HGAtomService/HGSecurityService/src
    ../../services/HGAtomService/HGLogService/inc               ../../services/HGAtomService/HGLogService/src
    ../../services/HGAtomService/HGSaveService/inc              ../../services/HGAtomService/HGSaveService/src
    ../../services/HGAtomService/HGFrameService/inc             ../../services/HGAtomService/HGFrameService/src
    ../../services/HGAtomService/HGCommunicateService/inc       ../../services/HGAtomService/HGCommunicateService/src
    ../../services/HGAtomService/HGImageAlgorithmService/inc    ../../services/HGAtomService/HGImageAlgorithmService/src
    ../../services/HGAtomService/HGAuthorityService/inc         ../../services/HGAtomService/HGAuthorityService/src
    ../../modules/HGCupDetModule/inc                            ../../modules/HGCupDetModule/src
    ../../interfaces/CameraRecognizeInterface/inc               ../../interfaces/CameraRecognizeInterface/src
)

target_compile_definitions(PackFunInterface PRIVATE LIBSSH_STATIC)
target_link_libraries(PackFunInterface
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ${HG_PROJECT_ROOT_FS}/lib/libssh.a
    ${HG_PROJECT_ROOT_FS}/lib/libzlibstatic.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(PackFunInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()