# 用于 Linux 上交叉编译生成 Windows x86_64 可执行文件
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 你的 mingw-w64 安装路径，例如 /usr/bin，或你自己下载的路径
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
set(CMAKE_C_COMPILER   /usr/bin/${TOOLCHAIN_PREFIX}-gcc-posix)
set(CMAKE_CXX_COMPILER /usr/bin/${TOOLCHAIN_PREFIX}-g++-posix)

# 禁用默认查找路径，避免用到 Linux 系统库
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 不要加 pthread，g++-posix 自动链接 winpthreads
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

