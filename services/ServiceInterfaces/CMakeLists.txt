set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(ServiceInterfaces inc src ../../)
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseConfig
    HGSvcSave
    HGSvcFrame
    HGBaseCommon
    HGSvcLog
    HGSvcError
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGServiceInterfaces inc src ../../)

elseif (PLATFORM STREQUAL "win32")
endif()


HG_CMAKE_MODULE_DEPEND(
    HGBaseConfig
    HGSvcSave
    HGSvcFrame
    HGBaseCommon
    HGSvcLog
    HGSvcError
)
set_target_properties(HGServiceInterfaces PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)