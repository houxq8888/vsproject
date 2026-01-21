set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(SvcFrame inc src)

HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGBaseConfig
    HGSvcSave
    HGSvcLog
)

if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGSvcFrame inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGBaseConfig
    HGSvcSave
    HGSvcLog
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGSvcFrame inc src
    ../../services/AtomService/SvcSave/inc          ../../services/AtomService/SvcSave/src
    ../../services/AtomService/SvcLog/inc           ../../services/AtomService/SvcLog/src
)
HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGBaseConfig
    HGSvcSave
    HGSvcLog
)
endif()

set_target_properties(HGSvcFrame PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)