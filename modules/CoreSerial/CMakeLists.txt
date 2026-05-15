set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreSerial inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGHwLowerPC
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCoreSerial inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCoreSerial inc src
    ../../services/BaseCommon/inc
    ../../services/BaseCommon/src
    ../../services/Hardware/HwLowerPC/inc
    ../../services/Hardware/HwLowerPC/src)
HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGHwLowerPC
)

endif()

set_target_properties(HGCoreSerial PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
