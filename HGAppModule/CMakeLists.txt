# HGAppModule Windows独立构建配置
# 专门用于Windows环境下独立编译HGAppModule中的所有模块

cmake_minimum_required(VERSION 3.16)
project(HGAppModule)

# Define Qt libraries based on OS



# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含项目宏定义
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/project_macro.cmake)

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
)

# 添加所有HGAppModule子模块
add_subdirectory(HGBaseAppModule)
add_subdirectory(HGUserAuditModule)
add_subdirectory(HGPrintModule)
add_subdirectory(HGCurveModule)
add_subdirectory(HGLoginModule)
add_subdirectory(HGSearchModule)
add_subdirectory(HGLogModule)
add_subdirectory(HGExceptionHandleModule)
add_subdirectory(HGUartModule)
add_subdirectory(HGEBalanceModule)
add_subdirectory(HGSharedFileModule)
add_subdirectory(HGCameraRecognizeModule)
add_subdirectory(HGMethodModule)
add_subdirectory(HGTaskModule)
add_subdirectory(HGFlowModule)
add_subdirectory(HGAnalysisRecordModule)
add_subdirectory(HGChannelModule)
add_subdirectory(HGReagentModule)