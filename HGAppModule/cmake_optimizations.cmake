# HGAppModule 编译优化配置
# 此文件包含针对编译速度优化的CMake设置

# 设置编译优化标志
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # 针对Release模式的优化
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native")
    
    # 启用链接时优化(LTO)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto=auto")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=auto")
    endif()
    
    # 减少调试信息大小
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g1")
    endif()
endif()

# 针对Debug模式的优化
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 保持调试信息完整，但优化编译速度
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O1")
endif()

# 预编译头文件优化
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # 启用预编译头文件支持
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winvalid-pch")
endif()

# 并行编译优化
if(CMAKE_GENERATOR MATCHES "Makefiles")
    # 设置并行构建
    if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
        # 自动检测CPU核心数
        include(ProcessorCount)
        ProcessorCount(N)
        if(NOT N EQUAL 0)
            # 限制最大并行任务数，避免内存不足
            if(N GREATER 8)
                set(N 8)
            endif()
            set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
        endif()
    endif()
endif()

# Qt特定优化
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    # 启用Qt的编译优化
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    
    # 减少MOC生成的文件大小
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-keep-inline-dllexport")
    endif()
endif()

# 内存使用优化
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # 限制每个编译任务的内存使用
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=5")
endif()

# 输出编译优化信息
message(STATUS "编译优化配置已启用:")
message(STATUS "  - 构建类型: ${CMAKE_BUILD_TYPE}")
if(DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    message(STATUS "  - 并行任务数: ${CMAKE_BUILD_PARALLEL_LEVEL}")
endif()
message(STATUS "  - C++标志: ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "  - Release优化标志: ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  - Debug优化标志: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()