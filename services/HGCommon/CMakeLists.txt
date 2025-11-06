set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
add_definitions(-DHGCOMMON_LIBRARY)
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")

HG_CMAKE_ADD_MODELU(Common inc src)

# link_directories(${LIBPINYIN_LIBRARY_DIRS})
# target_link_libraries(main ${LIBPINYIN_LIBRARIES})

if (PLATFORM STREQUAL "x64")
HG_CMAKE_MODULE_DEPEND_PUB( 
    ${LOG4CPLUS_LIBRARIES}  
    ALSA::ALSA
    ${LIBPINYIN_LIBRARIES}
)
HG_CMAKE_ADD_SHARED(HGCommon inc src)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(HGCommon ${X11_LIBRARIES} Xrandr)

elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCommon inc src)

# 设置导出符号
target_compile_definitions(HGCommon PRIVATE HGCOMMON_LIBRARY)

target_link_libraries(HGCommon  
    #${HG_PROJECT_ROOT_FS}/lib/liblog4cplusU.dll.a
    ws2_32 iphlpapi)
endif()


set_target_properties(HGCommon PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)