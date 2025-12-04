# HGAppModule Windows独立构建配置
# 专门用于Windows环境下独立编译HGAppModule中的所有模块

cmake_minimum_required(VERSION 3.16)
project(HGAppModule)

# 设置根目录和平台
set(HG_PROJECT_ROOT_FS ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(PLATFORM win32)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含编译优化配置
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake_optimizations.cmake")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_optimizations.cmake)
endif()

# 包含项目宏定义
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/project_macro.cmake)

# 包含3rdparty目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${HG_PROJECT_ROOT_FS}/3rdparty/include
    ${HG_PROJECT_ROOT_FS}/3rdparty/include/opencv4
    ${HG_PROJECT_ROOT_FS}/services/HGAtomService/HGSaveService/inc
    ${HG_PROJECT_ROOT_FS}/services/HGAtomService/HGFrameService/inc
    ${HG_PROJECT_ROOT_FS}/services/HGAtomService/HGSecurityService/inc
    ${HG_PROJECT_ROOT_FS}/services/HGAtomService/HGCommunicateService/inc
    ${HG_PROJECT_ROOT_FS}/services/HGAtomService/HGAuthorityService/inc 
    ${HG_PROJECT_ROOT_FS}/HGAppModule/HGBaseAppModule/inc 
    ${HG_PROJECT_ROOT_FS}/HGAppModule/pluginInterface/inc 
    ${HG_PROJECT_ROOT_FS}/HGAppModule/HGSearchModule/inc
    ${HG_PROJECT_ROOT_FS}/HGAppModule/HGUartModule/inc
    ${HG_PROJECT_ROOT_FS}/HGAppModule/HGPrintModule/inc
    ${HG_PROJECT_ROOT_FS}/HGAppModule/HGCurveModule/inc
    ${HG_PROJECT_ROOT_FS}/services/HGHardware/HGLowerPC/inc
    ${HG_PROJECT_ROOT_FS}/services/HGCommon/inc
    ${HG_PROJECT_ROOT_FS}/services/HGConfig/inc
    ${HG_PROJECT_ROOT_FS}/interfaces/CameraRecognizeInterface/inc
)

# 链接3rdparty目录
link_directories(
    ${HG_PROJECT_ROOT_FS}/3rdparty/lib
)

# 查找Qt6依赖，禁用Vulkan支持
find_package(Qt6 COMPONENTS Core Widgets Gui Charts LinguistTools PrintSupport Designer UiPlugin Qml REQUIRED)

# 禁用Vulkan支持
set_target_properties(Qt6::Gui PROPERTIES INTERFACE_QT_FEATURE_vulkan OFF)

# 设置库路径
set(ZXING_LIBRARY ${HG_PROJECT_ROOT_FS}/3rdparty/lib/libZXing.a)

# 链接Qt库
link_libraries(
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Charts
    Qt6::PrintSupport
    Qt6::Designer
    Qt6::UiPlugin
    Qt6::Qml
)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 根据操作系统查找对应的Qt版本


# 设置自动处理MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/include
)

# 链接目录
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/lib
)

# 设置模块构建选项
option(BUILD_EXECUTABLE "Build modules as executable" OFF)
option(BUILD_SHARED_LIBS "Build modules as shared libraries" OFF)
option(BUILD_STATIC_LIBS "Build modules as static libraries" ON)

# 设置HGBaseAppModule的特殊构建选项（只能编译为静态库或动态库）
set(BUILD_HGBASEAPP_EXECUTABLE OFF)

# 模块化编译支持
option(MODULAR_BUILD_ENABLED "启用模块化编译" OFF)
set(BUILD_MODULES "" CACHE STRING "要编译的模块列表（空格分隔）")

# 模块化编译逻辑
if(MODULAR_BUILD_ENABLED AND BUILD_MODULES)
    message(STATUS "模块化编译已启用")
    message(STATUS "要编译的模块: ${BUILD_MODULES}")
    
    # 将模块列表转换为CMake列表
    string(REPLACE " " ";" MODULE_LIST "${BUILD_MODULES}")
    
    # 检查并添加指定的模块
    foreach(MODULE IN LISTS MODULE_LIST)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/CMakeLists.txt")
            message(STATUS "添加模块: ${MODULE}")
            add_subdirectory(${MODULE})
        else()
            message(WARNING "模块 ${MODULE} 不存在或没有CMakeLists.txt")
        endif()
    endforeach()
else()
    # 传统编译模式 - 添加所有HGAppModule子模块
    add_subdirectory(HGBaseAppModule)
    
    # 添加PluginInterface模块
    add_subdirectory(pluginInterface)
    # 传统编译模式 - 编译所有模块
    message(STATUS "使用传统编译模式 - 编译所有模块")
    
    # 添加所有模块
  #  add_subdirectory(HGCurveModule)
  #  add_subdirectory(HGSearchModule)
  #  add_subdirectory(HGLogModule)
    add_subdirectory(HGUserAuditModule)
  #  add_subdirectory(HGUartModule)
 #   add_subdirectory(HGLoginModule)
 #   add_subdirectory(HGEBalanceModule)
 #   add_subdirectory(HGSharedFileModule)
  #  add_subdirectory(HGScannerModule)
 #   add_subdirectory(HGMethodModule)
  #  add_subdirectory(HGTaskModule)
 #   add_subdirectory(HGFlowModule)
  #  add_subdirectory(HGChannelModule)
  #  add_subdirectory(HGReagentModule)
  #  add_subdirectory(HGAnalysisRecordModule)
  #  add_subdirectory(HGPrintModule)
  #  add_subdirectory(HGCameraRecognizeModule)
  #  add_subdirectory(HGExceptionHandleModule)
endif()