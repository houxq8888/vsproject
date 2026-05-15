set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfCommunicate inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCoreCommunicate
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfCommunicate inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCoreCommunicate
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfCommunicate inc src
        ../../modules/CoreCommunicate/inc
        ../../modules/CoreCommunicate/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCoreCommunicate
    )
endif()

set_target_properties(HGIfCommunicate PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
