set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfScanner inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCoreScanner
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfScanner inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCoreScanner
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfScanner inc src
        ../../modules/CoreScanner/inc
        ../../modules/CoreScanner/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCoreScanner
    )
endif()

set_target_properties(HGIfScanner PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
