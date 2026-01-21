set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(LogService inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGConfig
    HGCommon
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGLogService inc src)

HG_CMAKE_MODULE_DEPEND(
    HGConfig
    HGCommon
    ${LOG4CPLUS_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGLogService inc src
    ../../HGConfig/inc          ../../services/HGConfig/src
    ../../HGCommon/inc         ../../services/HGCommon/src
)
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
)
endif()

set_target_properties(HGLogService PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)