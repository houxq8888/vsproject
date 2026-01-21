cmake_minimum_required(VERSION 3.22.0)

project(HG_BUILD_PROJECT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



include("cmake/project_macro.cmake")

if(NOT DEFINED HG_PROJECT_ROOT_FS)
    message(FATAL_ERROR "HG_PROJECT_ROOT_FS is not defined. Please set the variable.")
endif()

if(NOT DEFINED PLATFORM)
    message(FATAL_ERROR "PLATFORM is not defined. Please set the variable.")
endif()

message(STATUS "Build platform: ${PLATFORM}")
message(STATUS "Use rootfs: ${HG_PROJECT_ROOT_FS}")

if(CMAKE_CROSSCOMPILING)
    list(APPEND CMAKE_PREFIX_PATH "${HG_PROJECT_ROOT_FS}/cmake") #"${HG_PROJECT_ROOT_FS}/usr/lib/aarch64-linux-gnu/cmake")
    message(STATUS "Cross-compiling: Qt/OpenCV will search in ${CMAKE_PREFIX_PATH}")
endif()

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling detected")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
else()
# 检查CMake版本是否支持预编译头文件
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
    set(HG_USE_PCH ON)
    message(STATUS "预编译头文件支持: 启用 (CMake ${CMAKE_VERSION})")
else()
    set(HG_USE_PCH OFF)
    message(STATUS "预编译头文件支持: 禁用 (CMake版本过低，需要3.16+)")
endif()

# 预编译头文件配置函数
function(hg_add_precompiled_header target header_file)
    if(NOT HG_USE_PCH)
        return()
    endif()
    
    # 检查目标语言
    get_target_property(target_type ${target} TYPE)
    get_target_property(target_sources ${target} SOURCES)
    
    # 检测是否为C项目
    set(is_c_project FALSE)
    if(target_sources)
        foreach(source_file ${target_sources})
            if(source_file MATCHES "\.(c|C)$")
                set(is_c_project TRUE)
                break()
            endif()
        endforeach()
    endif()
    
    # 对于C项目，使用更简单的预编译头文件处理
    if(is_c_project)
        # C项目只需要基本的C头文件支持
        target_compile_definitions(${target} PRIVATE -DUSE_C_PCH)
        message(STATUS "为目标 ${target} (C项目) 添加简化的预编译头文件: ${header_file}")
    else()
        # C++项目使用完整的预编译头文件支持
        # 首先确保编译器能够找到标准库
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            # 添加标准库路径到包含目录
            # 检查是否为Windows交叉编译，如果是则跳过Linux系统路径
            if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
                target_include_directories(${target} SYSTEM PRIVATE 
                    /usr/include/c++/11
                    /usr/include/x86_64-linux-gnu/c++/11
                    /usr/include/c++/11/backward
                    /usr/lib/gcc/x86_64-linux-gnu/11/include
                )
            endif()
        endif()
        
        # 为GCC/Clang设置编译选项
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PRIVATE -Winvalid-pch)
        endif()
        
        message(STATUS "为目标 ${target} (C++项目) 添加预编译头文件: ${header_file}")
    endif()
    
    # 设置预编译头文件
    target_precompile_headers(${target} PRIVATE "${header_file}")
endfunction()
    message(STATUS "Not cross-compiling")
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
    # 设置模块构建选项
    option(BUILD_EXECUTABLE "Build modules as executable" ON)
    option(BUILD_SHARED_LIBS "Build modules as shared libraries" ON)
endif()


if (PLATFORM STREQUAL "stm32")
    #add process sub-project
    add_subdirectory(processes/FreeRTOSTest)
elseif (PLATFORM STREQUAL "zone")
    add_subdirectory(projects/ZoneTest)
    add_subdirectory(adas/adas_log)
    add_subdirectory(adas/adas_pal)
    add_subdirectory(adas/alg_interface)
    add_subdirectory(adas/ce_math)
    add_subdirectory(adas/ce_sv_engine)
else ()
    #find used 3rd oss
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    find_package(X11 REQUIRED)
    find_package(PkgConfig REQUIRED)

    if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "x64_release")
	    find_package(OpenSSL REQUIRED)
        find_package(vsomeip3 REQUIRED)
        find_package(log4cplus REQUIRED)
        find_package(ALSA REQUIRED)
        find_package(Freetype REQUIRED)
        find_package(OpenCV REQUIRED)
        # 查找 Curl
        find_package(CURL REQUIRED)
    #Quick QuickWidgets Positioning Location WebEngine WebEngineWidgets 
        find_package(Qt6 COMPONENTS Core Widgets Gui Charts LinguistTools PrintSupport Designer UiPlugin Qml REQUIRED)  # Xlsx

        pkg_check_modules(LIBPINYIN REQUIRED libpinyin)
        pkg_check_modules(LIBSSH REQUIRED libssh)
        include_directories(${LIBSSH_INCLUDE_DIRS})
        include_directories(${LIBPINYIN_INCLUDE_DIRS})
        pkg_check_modules(SQLCIPHER REQUIRED sqlcipher)
        INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
        include_directories(${FREETYPE_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${Qt5Xlsx_INCLUDE_DIRS})
        # set 
        set(ZeroMQ_LIBRARY /usr/lib/x86_64-linux-gnu/libzmq.so)
        set(CUPS_LIBRARY /usr/lib/x86_64-linux-gnu/libcups.so)
        set(HARU_LIBRARY /usr/lib/x86_64-linux-gnu/libhpdf.so)
        set(XLSXIO_WRITE_LIBRARY /usr/local/lib/libxlsxio_write.so)
        set(XLSXIO_READ_LIBRARY /usr/local/lib/libxlsxio_read.so)
        set(OpencvFreeType_LIBRARY /usr/lib/x86_64-linux-gnu/libopencv_freetype.so)
        set(MQTT_LIBRARYS /usr/local/lib/libpaho-mqtt3c.so
        	/usr/local/lib/libpaho-mqtt3a.so
        	/usr/local/lib/libpaho-mqttpp3.so
        )
        set(ZXING_LIBRARY /usr/local/lib/libZXing.so)
        set(ONNX_LIBRARY /media/ubunu2204/extra1/project/projects/Git/onnxruntime-linux-x64-1.15.0/lib/libonnxruntime.so)
        set(LIB_MOSQUITT_LIBRARY /usr/lib/x86_64-linux-gnu/libmosquitto.so)
        set(LIB_SERIALPORT_LIBRARY /usr/local/lib/libcserialport.so)
        set(TIDY_LIBRARIES /usr/lib/x86_64-linux-gnu/libtidy.so)
        set(LOG4CPLUS_LIBRARIES /usr/lib/x86_64-linux-gnu/liblog4cplus-2.0.so)
        # INCLUDE_DIRECTORIES(/usr/local/include/libpinyin-2.10.2)

        # 查找 FFmpeg 库
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
        set(AVFORMAT_LIBRARIES /usr/lib/x86_64-linux-gnu/libavformat.so)
        set(AVCODEC_LIBRARIES  /usr/lib/x86_64-linux-gnu/libavcodec.so)
        set(AVUTIL_LIBRARIES   /usr/lib/x86_64-linux-gnu/libavutil.so)
        set(SWSCALE_LIBRARIES  /usr/lib/x86_64-linux-gnu/libswscale.so)
        set(USB_LIBRARY /usr/lib/x86_64-linux-gnu/libusb-1.0.so)
    elseif (PLATFORM STREQUAL "win32")
        INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include)
        INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
	    pkg_check_modules(OPENSSL REQUIRED libssl libcrypto)
    	include_directories(${OPENSSL_INCLUDE_DIRS})
    	link_directories(${OPENSSL_LIBRARY_DIRS})
        set(HARU_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libhpdf.dll.a)
        set(XLSXIO_WRITE_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libxlsxio_write.dll.a)
        set(XLSXIO_READ_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libxlsxio_read.dll.a)
        # set(OpencvFreeType_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libopencv_freetype.so)
        # set(MQTT_LIBRARYS ${HG_PROJECT_ROOT_FS}/lib/libpaho-mqtt3c.so
        #     ${HG_PROJECT_ROOT_FS}/lib/libpaho-mqtt3a.so
        #     ${HG_PROJECT_ROOT_FS}/lib/libpaho-mqttpp3.so
        # )
        # set(ONNX_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libonnxruntime.so)
        set(LIB_MOSQUITT_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libmosquitto.dll.a)
        set(LOG4CPLUS_LIBRARIES ${HG_PROJECT_ROOT_FS}/lib/liblog4cplusU.dll.a)
        set(LIB_SERIALPORT_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a)
        # set(ZXING_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libZXing.a)
        set(USB_LIBRARY ${HG_PROJECT_ROOT_FS}/lib/libusb-1.0.a)
        set(OpenCV_LIBRARIES ${HG_PROJECT_ROOT_FS}/lib/libopencv_world460.a)
        link_directories(${HG_PROJECT_ROOT_FS}/lib)
    elseif (PLATFORM STREQUAL "win32_x86")
        INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include)
        INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
        link_directories(${HG_PROJECT_ROOT_FS}/lib)  
    endif()

    #add service sub-project
    add_subdirectory(services/BaseCommon)
    add_subdirectory(services/BaseConfig)
    add_subdirectory(services/Hardware/HwLowerPC)
    add_subdirectory(services/Hardware/HwSensor)
    # add_subdirectory(services/Hardware/HwRobot)
    add_subdirectory(services/AtomService/SvcImageAlgorithm)
    # add_subdirectory(services/AtomService/SvcDataPostProcess)
    # add_subdirectory(services/AtomService/SvcInteractive)
    # add_subdirectory(services/AtomService/SvcOTA)
    add_subdirectory(services/AtomService/SvcSave)
    add_subdirectory(services/AtomService/SvcLog)
    add_subdirectory(services/AtomService/SvcFrame)
    add_subdirectory(services/AtomService/SvcError)
    # add common module sub-project
    # add_subdirectory(modules/CoreV6)
    add_subdirectory(modules/CoreCupDet)
    add_subdirectory(modules/CoreLog)
    #add interface sub-project
    # add_subdirectory(interfaces/IfV6)
    add_subdirectory(interfaces/IfCupDet)
    add_subdirectory(interfaces/IfCameraControl)
    add_subdirectory(interfaces/IfCameraRecognize)
    add_subdirectory(interfaces/IfLog)
    add_subdirectory(interfaces/IfPackFunc)

    if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "x64_release")
        add_subdirectory(HGAppModule/pluginInterface)
        add_subdirectory(services/AtomService/SvcCommunicate)
        add_subdirectory(services/AtomService/SvcSecurity)
        add_subdirectory(services/AtomService/SvcAuthority)
        # add_subdirectory(services/AtomService/SvcCommonProtocol)
        add_subdirectory(modules/CoreDashboard)
        # add_subdirectory(modules/CoreOnlinePlatform)
        # # add_subdirectory(modules/CoreDetectDimentionVux)
        # add_subdirectory(interfaces/IfOnlinePlatform)
        # # add_subdirectory(interfaces/IfDetectDimentionVux)
        # #add process sub-project
        # # add_subdirectory(processes/DetectDimentionVuxProcess)
        # # add_subdirectory(processes/YModemTest)
        # add_subdirectory(processes/CommonCommunicationDemo)
        # add_subdirectory(processes/CommonCommunicationDemoC)
        # add_subdirectory(processes/CommonProtocolAPI)
        # add_subdirectory(processes/TCPPushClient)
        #add app-project
        add_subdirectory(HGAppModule/UIBaseApp)
        # add_subdirectory(HGAppModule/UIUserAudit)
        # add_subdirectory(HGAppModule/UIPrint)
        # add_subdirectory(HGAppModule/UICurve)
        # add_subdirectory(HGAppModule/UILogin)
        # add_subdirectory(HGAppModule/UISearch)
        # add_subdirectory(HGAppModule/UILogModule)
        # add_subdirectory(HGAppModule/UIExceptionHandle)
        # add_subdirectory(HGAppModule/UIGUart)
        # add_subdirectory(HGAppModule/UIEBalance)
        # add_subdirectory(HGAppModule/UISharedFile)
        # add_subdirectory(HGAppModule/UICameraRecognize)
        # add_subdirectory(HGAppModule/UIMethod)
        # add_subdirectory(HGAppModule/UITask)
        # add_subdirectory(HGAppModule/UIFlow)
        # add_subdirectory(HGAppModule/UIAnalysisRecord)
        # add_subdirectory(HGAppModule/UIChannelModule)
        # add_subdirectory(HGAppModule/UIReagentModule)
        # add_subdirectory(HGAppModule/UIScannerModule)
        # add_subdirectory(HGAppModule/TestPlugin)
        # # add_subdirectory(projects/HGCupDetApp)
        # # add_subdirectory(projects/HGComDebugApp)
        # add_subdirectory(projects/HGOnlinePlatformApp)
        # add_subdirectory(projects/HGDashboardApp)
        # add_subdirectory(projects/CloudApp)
        # add_subdirectory(projects/ProductFlow)
        # # add_subdirectory(projects/MyCefTest)

        # #TestCase
        # add_subdirectory(projects/HGTestCase/testCameraInterface)
    endif()
endif()