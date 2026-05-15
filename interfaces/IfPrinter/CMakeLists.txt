set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfPrinter inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCorePrinter
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfPrinter inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCorePrinter
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfPrinter inc src
        ../../modules/CorePrinter/inc
        ../../modules/CorePrinter/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCorePrinter
    )
endif()

set_target_properties(HGIfPrinter PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
