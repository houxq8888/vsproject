set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODULE(SvcSave inc src)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGSvcLog
    ${OpenCV_LIBRARIES}
)

HG_CMAKE_ADD_SHARED(HGSvcSave inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGSvcLog
    ${OpenCV_LIBRARIES}
)

set(SQLCIPHER_INCLUDE_DIR /usr/local/include/sqlcipher)
set(SQLCIPHER_LIBRARY /usr/lib/x86_64-linux-gnu/libsqlcipher.so)

target_include_directories(HGSvcSave PRIVATE ${SQLCIPHER_INCLUDE_DIR})
target_link_libraries(HGSvcSave PRIVATE ${SQLCIPHER_LIBRARY})

elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
)
HG_CMAKE_ADD_STATIC(HGSvcSave inc src)
set(SQLCIPHER_INCLUDE_DIR ${HG_PROJECT_ROOT_FS}/include/sqlcipher)
set(SQLCIPHER_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a)
target_include_directories(HGSvcSave PRIVATE ${SQLCIPHER_INCLUDE_DIR})
target_link_libraries(HGSvcSave PRIVATE ${SQLCIPHER_LIBRARY})

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
)


endif()



set_target_properties(HGSvcSave PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)