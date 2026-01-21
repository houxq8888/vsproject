set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreLog inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGSvcLog
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCoreLog inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGSvcLog
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCoreLog inc src
    ../../services/BaseCommon/inc                             ../../services/BaseCommon/src 
    ../../services/AtomService/SvcLog/inc           ../../services/AtomService/SvcLog/src
    ../../services/AtomService/SvcFrame/inc         ../../services/AtomService/SvcFrame/src
    ../../services/AtomService/SvcSave/inc          ../../services/AtomService/SvcSave/src
)
HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGSvcLog
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
)

endif()

set_target_properties(HGCoreLog PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)