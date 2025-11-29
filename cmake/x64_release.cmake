# x64 Release 版本编译配置
# 优化级别: O2 - 平衡优化
# 定义: NDEBUG - 禁用断言和调试代码

set(FLAGS_COMMONS "-Wall -Wextra -Wpedantic -fPIC")
set(FLAGS_OPTIMAL "-O2 -DNDEBUG -march=native -mtune=native")
# 暂时禁用LTO以避免编译问题
# set(FLAGS_LTO "-flto=auto")

# C++ 编译器标志
set(CMAKE_CXX_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL} -std=c++17 -fopenmp")

# C 编译器标志
set(CMAKE_C_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL}")

# 链接器标志（暂时禁用LTO）
# set(CMAKE_EXE_LINKER_FLAGS "${FLAGS_LTO}")
# set(CMAKE_SHARED_LINKER_FLAGS "${FLAGS_LTO}")

# 发布版本特定设置
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

# 暂时禁用跨过程优化
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE CACHE BOOL "" FORCE)

message(STATUS "使用 x64 Release 配置: O2优化 + 本地架构优化（LTO暂时禁用）")