set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(FrameService inc src)

HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGConfig
    HGSaveService
)

if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGFrameService inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    HGSaveService
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGFrameService inc src
    ../../services/HGAtomService/HGSaveService/inc          ../../services/HGAtomService/HGSaveService/src
)
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    HGSaveService
)
endif()

set_target_properties(HGFrameService PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)