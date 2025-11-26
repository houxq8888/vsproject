set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(V6Interface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGV6Module
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGV6Interface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGV6Module
    HGLowerPC
    HGConfig
    HGFrameService
    HGSecurityService
    HGCommunicateService
    HGSaveService
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
    ${LOG4CPLUS_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${MQTT_LIBRARYS}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGV6Interface inc src 
    ../../services/HGCommon/inc                             ../../services/HGCommon/src
    ../../services/HGConfig/inc                             ../../services/HGConfig/src
    ../../services/HGAtomService/HGSaveService/inc          ../../services/HGAtomService/HGSaveService/src
    ../../services/HGAtomService/HGFrameService/inc         ../../services/HGAtomService/HGFrameService/src
    ../../modules/HGV6Module/inc                            ../../modules/HGV6Module/src
    ../../services/HGAtomService/HGCommunicateService/inc   ../../services/HGAtomService/HGCommunicateService/src
)

# HG_CMAKE_MODULE_DEPEND(
#     HGCommon
#     HGConfig
#     HGLowerPC
#     HGV6Module
#     HGFrameService
#     HGSaveService
#     HGCommunicateService
#     HGAuthorityService
# )
target_compile_definitions(HGV6Interface PRIVATE LIBSSH_STATIC)
target_link_libraries(HGV6Interface
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ${HG_PROJECT_ROOT_FS}/lib/libssh.a
    ${HG_PROJECT_ROOT_FS}/lib/libzlibstatic.a
    ws2_32 iphlpapi)

endif()

set_target_properties(HGV6Interface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
