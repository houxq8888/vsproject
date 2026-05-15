set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreReagent inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseCommon
    HGSvcFrame
    HGSvcSave
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCoreReagent inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCoreReagent inc src
        ../../services/AtomService/SvcFrame/inc
        ../../services/AtomService/SvcFrame/src)
    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
    )
endif()

set_target_properties(HGCoreReagent PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
