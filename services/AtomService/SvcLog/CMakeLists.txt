set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(SvcLog inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseConfig
    HGBaseCommon
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGSvcLog inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseConfig
    HGBaseCommon
    ${LOG4CPLUS_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGSvcLog inc src
    ../../BaseConfig/inc          ../../services/BaseConfig/src
    ../../BaseCommon/inc         ../../services/BaseCommon/src
)
HG_CMAKE_MODULE_DEPEND(
    HGBaseConfig
    HGBaseCommon
)
endif()

set_target_properties(HGSvcLog PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)