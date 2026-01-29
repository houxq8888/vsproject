set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfChartData inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCoreChartData
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfChartData inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCoreChartData
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfChartData inc src
        ../../modules/CoreChartData/inc
        ../../modules/CoreChartData/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCoreChartData
    )
endif()

set_target_properties(HGIfChartData PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
