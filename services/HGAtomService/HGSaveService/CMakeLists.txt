set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

HG_CMAKE_ADD_MODELU(SaveService inc src)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    ${OpenCV_LIBRARIES}
)

HG_CMAKE_ADD_SHARED(HGSaveService inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    ${OpenCV_LIBRARIES}
)

set(SQLCIPHER_INCLUDE_DIR /usr/local/include/sqlcipher)
set(SQLCIPHER_LIBRARY /usr/lib/x86_64-linux-gnu/libsqlcipher.so)

target_include_directories(HGSaveService PRIVATE ${SQLCIPHER_INCLUDE_DIR})
target_link_libraries(HGSaveService PRIVATE ${SQLCIPHER_LIBRARY})

elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
)
HG_CMAKE_ADD_STATIC(HGSaveService inc src)
set(SQLCIPHER_INCLUDE_DIR ${HG_PROJECT_ROOT_FS}/include/sqlcipher)
set(SQLCIPHER_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a)
target_include_directories(HGSaveService PRIVATE ${SQLCIPHER_INCLUDE_DIR})
target_link_libraries(HGSaveService PRIVATE ${SQLCIPHER_LIBRARY})

HG_CMAKE_MODULE_DEPEND(
    HGCommon
)

endif()



set_target_properties(HGSaveService PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)