set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(IfAnalysisRecord inc src)

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCoreAnalysisRecord
)

if (PLATFORM STREQUAL "x64")
    HG_CMAKE_ADD_SHARED(HGIfAnalysisRecord inc src)

    HG_CMAKE_MODULE_DEPEND(
        HGCoreAnalysisRecord
    )
elseif (PLATFORM STREQUAL "win32")
    HG_CMAKE_ADD_STATIC(HGIfAnalysisRecord inc src
        ../../modules/CoreAnalysisRecord/inc
        ../../modules/CoreAnalysisRecord/src
        ../../services/AtomService/SvcFrame/inc
        ../../services/AtomService/SvcFrame/src)
    HG_CMAKE_MODULE_DEPEND(
        HGCoreAnalysisRecord
    )
endif()

set_target_properties(HGIfAnalysisRecord PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
