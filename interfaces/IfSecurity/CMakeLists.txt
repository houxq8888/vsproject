set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfSecurity inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCoreSecurity
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfSecurity inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCoreSecurity
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfSecurity inc src
    ../../modules/CoreSecurity/inc                                   ../../modules/CoreSecurity/src)
HG_CMAKE_MODULE_DEPEND(
    HGCoreSecurity
)

endif()

set_target_properties(HGIfSecurity PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)