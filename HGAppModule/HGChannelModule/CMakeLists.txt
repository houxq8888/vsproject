project(HGChannelModule VERSION 1.1.1)

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
        # Qt6 uses automatic MOC, but also need explicit MOC processing for static libraries
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        # 确保所有包含Q_OBJECT的头文件都被MOC处理
        set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
        # 显式调用qt6_wrap_cpp确保MOC文件正确生成
        qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
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

HG_CMAKE_ADD_MODELU(ChannelModule inc src                 
    ${MOC_SRCS})

# 添加pluginInterface头文件路径
target_include_directories(ChannelModule PUBLIC 
    ${CMAKE_SOURCE_DIR}/pluginInterface/inc
)
target_compile_definitions(ChannelModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGChannelModule\")

# 根据平台和编译选项构建不同的目标
if (BUILD_SHARED_LIBS)
    if(WIN32)
    else()
        HG_CMAKE_MODULE_DEPEND_PUB(
            PluginInterface
            HGLowerPC
            HGCommon
            HGConfig
            HGSaveService
            HGFrameService
            HGAuthorityService
            HGCommunicateService
            HGImageAlgorithmService
            HGSecurityService                        
            HGBaseAppModule
            ${ZXING_LIBRARY}
            ALSA::ALSA
            ${OpenCV_LIBRARIES}
            ${OpencvFreeType_LIBRARY}
            ${LOG4CPLUS_LIBRARIES}
            ${CUPS_LIBRARY}
            ${XLSXIO_READ_LIBRARY}
            ${XLSXIO_WRITE_LIBRARY}
            ${HARU_LIBRARY}
            ${ZeroMQ_LIBRARY}
            ${MQTT_LIBRARYS}
            ${LIB_SERIALPORT_LIBRARY}
            ${LIB_MOSQUITT_LIBRARY}
            ${PULSE_LIBRARIES}
            ${LIBSSH_LIBRARIES}
            ${AVFORMAT_LIBRARIES}
            ${AVCODEC_LIBRARIES}
            ${AVUTIL_LIBRARIES}
            ${SWSCALE_LIBRARIES}
            ${Boost_LIBRARIES}
            OpenSSL::Crypto
            ${QT_LIBRARIES}
        )

        HG_CMAKE_ADD_SHARED(HGChannelModule inc src ${MOC_SRCS})

        HG_CMAKE_MODULE_DEPEND(
            PluginInterface
            HGLowerPC
            HGCommon
            HGConfig
            HGSaveService
            HGFrameService
            HGAuthorityService
            HGCommunicateService
            HGImageAlgorithmService
            HGSecurityService
            HGBaseAppModule
            ${ZXING_LIBRARY}
            ALSA::ALSA
            ${OpenCV_LIBRARIES}
            ${OpencvFreeType_LIBRARY}
            ${LOG4CPLUS_LIBRARIES}
            ${CUPS_LIBRARY}
            ${XLSXIO_READ_LIBRARY}
            ${XLSXIO_WRITE_LIBRARY}
            ${HARU_LIBRARY}
            ${ZeroMQ_LIBRARY}
            ${MQTT_LIBRARYS}
            ${LIB_SERIALPORT_LIBRARY}
            ${LIB_MOSQUITT_LIBRARY}
            ${PULSE_LIBRARIES}
            ${LIBSSH_LIBRARIES}
            ${AVFORMAT_LIBRARIES}
            ${AVCODEC_LIBRARIES}
            ${AVUTIL_LIBRARIES}
            ${SWSCALE_LIBRARIES}
            ${Boost_LIBRARIES}
            OpenSSL::Crypto
            ${QT_LIBRARIES}
        )
    endif()
endif()

if (BUILD_STATIC_LIBS)
    if (WIN32)
        HG_CMAKE_ADD_STATIC(HGChannelModuleStatic inc src ${MOC_SRCS})
        # Windows平台只需要HGV6Interface库和基础依赖
        target_include_directories(HGChannelModuleStatic PUBLIC 
            ${CMAKE_SOURCE_DIR}/pluginInterface/inc
        )
        target_link_libraries(HGChannelModuleStatic PUBLIC 
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
    HG_CMAKE_ADD_EXE(HGChannelModuleRun main.cpp)

    
    # 添加HGBaseAppModuleStatic依赖
    if (WIN32)
        HG_CMAKE_MODULE_DEPEND(
            HGChannelModuleStatic
        )
        add_dependencies(HGChannelModuleRun HGBaseAppModuleStatic)
    else()
        HG_CMAKE_MODULE_DEPEND(
            ChannelModule
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
        target_link_libraries(HGChannelModuleRun PUBLIC 
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
        target_link_libraries(HGChannelModuleRun PUBLIC 
            ws2_32
            iphlpapi
            advapi32
            user32
            kernel32
        )
    endif()
endif()

