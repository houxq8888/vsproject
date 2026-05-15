set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreMethod inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseCommon
    HGSvcFrame
    HGSvcSave
    HGServiceInterfaces
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGCoreMethod inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
        HGServiceInterfaces
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGCoreMethod inc src
        ../../services/AtomService/SvcFrame/inc
        ../../services/AtomService/SvcFrame/src)
    HG_CMAKE_MODULE_DEPEND(
        HGBaseCommon
        HGSvcFrame
        HGSvcSave
    )
endif()

set_target_properties(HGCoreMethod PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
