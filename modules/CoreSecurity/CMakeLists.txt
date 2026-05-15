set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(CoreSecurity inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGSvcSecurity
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCoreSecurity inc src)

elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCoreSecurity inc src
    ../../services/BaseCommon/inc                             ../../services/BaseCommon/src 
    ../../services/AtomService/SvcLog/inc           ../../services/AtomService/SvcLog/src
    ../../services/AtomService/SvcFrame/inc         ../../services/AtomService/SvcFrame/src
    ../../services/AtomService/SvcSave/inc          ../../services/AtomService/SvcSave/src
)


endif()

HG_CMAKE_MODULE_DEPEND(
    HGSvcSecurity
)
set_target_properties(HGCoreSecurity PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)