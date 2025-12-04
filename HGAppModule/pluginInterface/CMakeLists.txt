set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 1)
set(LIBRARY_VERSION_PATCH 1)

file(GLOB MOC_HEADERS "inc/*.h")

# Define Qt libraries based on platform and compilation environment
if (PLATFORM STREQUAL "x64")
    # Linux native compilation - Qt5
    set(QT_LIBRARIES
        Qt5::Gui
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::PrintSupport
    )
    qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
elseif (PLATFORM STREQUAL "win32")
    if(WIN32)
        # Windows native compilation - Qt6
        set(QT_LIBRARIES
            Qt6::Gui
            Qt6::Core
            Qt6::Widgets
            Qt6::Charts
            Qt6::PrintSupport
        )
        # Qt6 uses automatic MOC, no need for qt5_wrap_cpp
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
    else()
        # Linux cross-compilation for Windows - Qt5
        set(QT_LIBRARIES
            Qt5::Gui
            Qt5::Core
            Qt5::Widgets
            Qt5::Charts
            Qt5::PrintSupport
        )
        qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
    endif()
endif()

HG_CMAKE_ADD_MODULE(PluginInterfaceModule inc src ${MOC_SRCS})

target_compile_definitions(PluginInterfaceModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"PluginInterface\"
     -D_NO_BYTESWAP_H)


# 根据平台和编译选项构建不同的目标
# PluginInterface只能编译为静态库或动态库，不能编译为可执行文件
if(BUILD_SHARED_LIBS)
    # Windows下只包含头文件，Linux下链接整个库
    if(WIN32)
        message(STATUS "Windows平台：只包含其他库的头文件")
    else()
        HG_CMAKE_MODULE_DEPEND_PUB(
            HGCommon
            ${OpenCV_LIBRARIES}
            ${LOG4CPLUS_LIBRARIES}
            ${QT_LIBRARIES}
        )
        message(STATUS "Building PluginInterface as shared library")
        HG_CMAKE_ADD_SHARED(PluginInterface inc src ${MOC_SRCS})
        message(STATUS "Linux平台：包含整个库")
        HG_CMAKE_MODULE_DEPEND(
            HGCommon
            ${OpenCV_LIBRARIES}
            ${LOG4CPLUS_LIBRARIES}
            ${QT_LIBRARIES}
        )
        set_target_properties(PluginInterface PROPERTIES 
            VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
            SOVERSION ${LIBRARY_VERSION_MAJOR}
        )
    endif()
    
endif()

if(BUILD_STATIC_LIBS)
    message(STATUS "Building PluginInterface as static library")
    HG_CMAKE_ADD_STATIC(PluginInterfaceStatic inc src ${MOC_SRCS})
    
    # Windows平台下需要链接所有依赖库
    if(WIN32)
        message(STATUS "Windows平台：链接所有依赖库")
        HG_CMAKE_MODULE_DEPEND_PUB(
            ${CMAKE_SOURCE_DIR}/../3rdparty/lib/libPackFunInterface.a
            ${QT_LIBRARIES}
        )
    else()
        message(STATUS "Linux平台：包含整个库")
        HG_CMAKE_MODULE_DEPEND_PUB(
            HGCommon
            ${OpenCV_LIBRARIES}
            ${LOG4CPLUS_LIBRARIES}
            ${QT_LIBRARIES}
        )
    endif()

    set_target_properties(PluginInterfaceStatic PROPERTIES 
        VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
        SOVERSION ${LIBRARY_VERSION_MAJOR}
    )
endif()


