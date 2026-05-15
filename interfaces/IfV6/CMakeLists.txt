set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfV6 inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCoreV6
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfV6 inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCoreV6
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfV6 inc src 
    ../../services/BaseCommon/inc                   ../../services/BaseCommon/src
    ../../services/BaseConfig/inc                   ../../services/BaseConfig/src
    ../../services/AtomService/SvcSave/inc          ../../services/AtomService/SvcSave/src
    ../../services/AtomService/SvcFrame/inc         ../../services/AtomService/SvcFrame/src
    ../../modules/CoreV6Module/inc                  ../../modules/CoreV6Module/src
    ../../services/AtomService/SvcCommunicate/inc   ../../services/AtomService/SvcCommunicate/src
    ../../services/AtomService/SvcLog/inc           ../../services/AtomService/SvcLog/src
)

# HG_CMAKE_MODULE_DEPEND(
#     HGBaseCommon
    # HGCoreV6
    # HGHwLowerPC
    # HGBaseConfig
    # HGSvcFrame
    # HGSvcLog
    # HGSvcSecurity
    # HGSvcCommunicate
    # HGSvcSave
# )
target_compile_definitions(HGIfV6 PRIVATE LIBSSH_STATIC)
target_link_libraries(HGIfV6
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ${HG_PROJECT_ROOT_FS}/lib/libssh.a
    ${HG_PROJECT_ROOT_FS}/lib/libzlibstatic.a
    ws2_32 iphlpapi)

endif()

set_target_properties(HGIfV6 PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
