set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreTask inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseCommon
    HGSvcFrame
    HGSvcSave
    HGBaseConfig
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCoreTask inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
        HGBaseConfig
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCoreTask inc src
        ../../services/AtomService/SvcFrame/inc
        ../../services/AtomService/SvcFrame/src)
    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
        HGBaseConfig
    )
endif()

set_target_properties(HGCoreTask PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
