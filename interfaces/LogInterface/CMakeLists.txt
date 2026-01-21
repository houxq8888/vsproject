set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(ILogInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGMLogModule
    HGConfig
    HGCommon
    HGLogService
    HGSaveService
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(IHGLogInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGMLogModule
    HGConfig
    HGCommon
    HGLogService
    HGSaveService
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(IHGLogInterface inc src
    ../../HGConfig/inc                                      ../../HGConfig/src
    ../../HGCommon/inc                                      ../../HGCommon/src 
    ../../services/HGAtomService/HGLogService/inc           ../../services/HGAtomService/HGLogService/src
    ../../modules/LogModule/inc                             ../../modules/LogModule/src
)
HG_CMAKE_MODULE_DEPEND(
    HGMLogModule
    HGConfig
    HGCommon
    HGLogService
    HGSaveService
)

endif()

set_target_properties(IHGLogInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)