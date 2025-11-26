# 简单的Qt6测试程序
cmake_minimum_required(VERSION 3.16)
project(HGAppModuleTest)

# Define Qt libraries based on OS



# 查找Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 设置自动MOC
set(CMAKE_AUTOMOC ON)

# 创建可执行文件
add_executable(HGAppModuleTest main.cpp)

# 链接Qt6库
target_link_libraries(HGAppModuleTest  )