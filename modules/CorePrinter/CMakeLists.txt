set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CorePrinter inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseCommon
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCorePrinter inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        ${CUPS_LIBRARY}
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCorePrinter inc src)
    
    target_link_libraries(HGCorePrinter PUBLIC
        ws2_32 iphlpapi)
endif()

set_target_properties(HGCorePrinter PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
