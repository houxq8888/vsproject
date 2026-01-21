set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(MLogModule inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGLogService
    HGFrameService
    HGSaveService
    HGConfig
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGMLogModule inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGLogService
    HGFrameService
    HGSaveService
    HGConfig
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGMLogModule inc src
    ../../services/HGCommon/inc                             ../../services/HGCommon/src 
    ../../services/HGAtomService/HGLogService/inc           ../../services/HGAtomService/HGLogService/src
    ../../services/HGAtomService/HGFrameService/inc         ../../services/HGAtomService/HGFrameService/src
    ../../services/HGAtomService/HGSaveService/inc          ../../services/HGAtomService/HGSaveService/src
)
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGLogService
    HGFrameService
    HGSaveService
)

endif()

set_target_properties(HGMLogModule PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)