project(HGUICurve VERSION 1.1.1)

file(GLOB MOC_HEADERS "inc/*.h")

# Define Qt libraries based on platform and compilation environment
if (PLATFORM STREQUAL "x64")
    # Linux native compilation - Qt6
        set(QT_LIBRARIES
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
    )
    # Qt6 uses automatic MOC, no need for qt6_wrap_cpp
    qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        # 确保所有包含Q_OBJECT的头文件都被MOC处理
        set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
elseif (PLATFORM STREQUAL "win32")
    # Windows native compilation - Qt6
    set(QT_LIBRARIES
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
    )
    # Qt6 uses automatic MOC, but also need explicit MOC processing for static libraries
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    # 确保所有包含Q_OBJECT的头文件都被MOC处理
    set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
    # 显式调用qt6_wrap_cpp确保MOC文件正确生成
    qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
endif()

HG_CMAKE_ADD_MODULE(UICurve inc src                 
    ${MOC_SRCS})

# 添加pluginInterface头文件路径
target_include_directories(UICurve PUBLIC 
    ${CMAKE_SOURCE_DIR}/pluginInterface/inc
)

target_compile_definitions(UICurve PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGUICurve\")



# 根据平台和编译选项构建不同的目标
if (BUILD_SHARED_LIBS)
    if(WIN32)
    else()
        HG_CMAKE_MODULE_DEPEND_PUB(
            PluginInterface
            HGUIBaseCore
            HGServiceInterfaces
            HGIfLog
            HGIfChartData
            ${QT_LIBRARIES}
        )
        HG_CMAKE_ADD_SHARED(HGUICurve inc src ${MOC_SRCS})
        # Linux平台需要完整的依赖链
        HG_CMAKE_MODULE_DEPEND(
            PluginInterface
            HGUIBaseCore
            HGServiceInterfaces
            HGIfLog
            HGIfChartData
            ${QT_LIBRARIES}
        )
    endif()
endif()

if (BUILD_STATIC_LIBS)
    if (WIN32)
        HG_CMAKE_ADD_STATIC(HGUICurveStatic inc src ${MOC_SRCS}
            ${CMAKE_SOURCE_DIR}/../../interfaces/IfChartData/inc
            ${CMAKE_SOURCE_DIR}/../../interfaces/IfChartData/src
            ${CMAKE_SOURCE_DIR}/../../modules/CoreChartData/inc
            ${CMAKE_SOURCE_DIR}/../../modules/CoreChartData/src)
        # Windows平台只需要HGV6Interface库和基础依赖
        target_include_directories(HGUICurveStatic PUBLIC 
            ${CMAKE_SOURCE_DIR}/pluginInterface/inc
        )
        target_link_libraries(HGUICurveStatic PUBLIC 
            ${CMAKE_SOURCE_DIR}/../3rdparty/lib/libPackFunInterface.a
            ${CMAKE_BINARY_DIR}/HGBaseAppModule/libHGBaseAppModuleStatic.a
            ${CMAKE_BINARY_DIR}/pluginInterface/libPluginInterfaceStatic.a
            ${OpenCV_LIBRARIES}
            ${OpencvFreeType_LIBRARY}
            ${LOG4CPLUS_LIBRARIES}
            ${QT_LIBRARIES}
        )
    else()
        # Linux平台静态库构建配置
    endif()
endif()


if (BUILD_EXECUTABLE)
    HG_CMAKE_ADD_EXE(HGUICurveRun main.cpp)

    # 在Windows平台明确指定为控制台应用程序
    if (WIN32)
        # 禁用LTO以避免插件错误
        set_target_properties(HGUICurveRun PROPERTIES
            INTERFACE_POSITION_INDEPENDENT_CODE OFF
        )
        # 明确指定为控制台应用程序
        target_link_options(HGUICurveRun PRIVATE -Wl,-subsystem,console)
        
        HG_CMAKE_MODULE_DEPEND(
            HGUICurveStatic
        )
        add_dependencies(HGUICurveRun HGBaseAppModuleStatic)
    else()
        HG_CMAKE_MODULE_DEPEND(
            UICurve
        )
    endif()

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    )

    include_directories("${CMAKE_CURRENT_BINARY_DIR}")
    # 添加HGBaseAppModule的头文件路径
    include_directories("${CMAKE_SOURCE_DIR}/HGBaseAppModule/inc")
    if (WIN32)
        # Windows平台只需要HGV6Interface库和基础依赖
        # 注意：libHGBaseAppModuleStatic.a必须在libPackFunInterface.a之前，因为前者依赖后者的符号
        target_link_libraries(HGUICurveRun PUBLIC 
            ${CMAKE_BINARY_DIR}/HGUICurve/libHGUICurveStatic.a
            ${CMAKE_BINARY_DIR}/HGBaseAppModule/libHGBaseAppModuleStatic.a
            ${CMAKE_BINARY_DIR}/pluginInterface/libPluginInterfaceStatic.a
            ${CMAKE_SOURCE_DIR}/../3rdparty/lib/libPackFunInterface.a
            ${QT_LIBRARIES}
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libopencv_world460.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libssh.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libiphlpapi.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libsqlcipher.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libshlwapi.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libssl.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libcrypto.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libws2_32.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libcserialport.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libsetupapi.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libhpdfs.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libzlibstatic.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libpng.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libtiff.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libjpeg-turbo.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libopenjp2.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libwebp.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libole32.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libuuid.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libstrmiids.a
            ${HG_PROJECT_ROOT_FS}/3rdparty/lib/liboleaut32.a
          #  ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libwinspool.a
        )
        
        # 添加必要的系统库依赖
        target_link_libraries(HGUICurveRun PUBLIC 
            ws2_32
            iphlpapi
            advapi32
            user32
            kernel32
        )
    endif()
endif()
