set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
set(CMAKE_C_COMPILER   /usr/bin/${TOOLCHAIN_PREFIX}-gcc-posix)
set(CMAKE_CXX_COMPILER /usr/bin/${TOOLCHAIN_PREFIX}-g++-posix)

# 禁用默认查找路径，避免用到 Linux 系统库
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 不要加 pthread，g++-posix 自动链接 winpthreads
# set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_POSIX_C_SOURCE=200809L)
add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7 及以上


set(FLAGS_COMMONS "-Wall -fPIC")
set(FLAGS_OPTIMAL "-g")
# 添加 pthread 和 std 线程支持
set(CMAKE_CXX_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL} -fopenmp" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL}" CACHE STRING "" FORCE)
# 编译器选项
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++" CACHE STRING "" FORCE)
