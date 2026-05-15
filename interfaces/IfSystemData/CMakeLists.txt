set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfSystemData inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCoreSystemData
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfSystemData inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCoreSystemData
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfSystemData inc src
        ../../modules/CoreSystemData/inc
        ../../modules/CoreSystemData/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCoreSystemData
    )
endif()

set_target_properties(HGIfSystemData PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
