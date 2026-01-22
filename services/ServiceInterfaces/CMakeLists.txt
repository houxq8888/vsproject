set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(ServiceInterfaces inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseConfig
    HGSvcSave
    HGSvcFrame
    HGBaseCommon
    HGSvcLog
    UIBaseApp
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGServiceInterfaces inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseConfig
    HGSvcSave
    HGSvcFrame
    HGBaseCommon
    HGSvcLog
)

elseif (PLATFORM STREQUAL "win32")

endif()

set_target_properties(HGServiceInterfaces PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)