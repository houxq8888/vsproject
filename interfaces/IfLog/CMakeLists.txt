set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfLog inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCoreLog
    HGBaseConfig
    HGBaseCommon
    HGSvcLog
    HGSvcSave
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfLog inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCoreLog
    HGBaseConfig
    HGBaseCommon
    HGSvcLog
    HGSvcSave
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfLog inc src
    ../../BaseConfig/inc                                        ../../BaseConfig/src
    ../../services/BaseCommon/inc                               ../../services/BaseCommon/src
    ../../services/AtomService/SvcLog/inc                       ../../services/AtomService/SvcLog/src
    ../../modules/CoreLog/inc                                   ../../modules/CoreLog/src
)
HG_CMAKE_MODULE_DEPEND(
    HGCoreLog
    HGBaseConfig
    HGBaseCommon
    HGSvcLog
    HGSvcSave
)

endif()

set_target_properties(HGIfLog PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)