set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreSystemData inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseCommon
    HGSvcSave
    HGSvcFrame
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCoreSystemData inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcSave
        HGSvcFrame
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCoreSystemData inc src
        ../../services/AtomService/SvcSave/inc
        ../../services/AtomService/SvcSave/src)
    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcSave
        HGSvcFrame
    )
endif()

set_target_properties(HGCoreSystemData PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
