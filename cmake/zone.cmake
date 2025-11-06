set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)


# if(NOT DEFINED TOOLCHAIN_DIR)
#     message(FATAL_ERROR "TOOLCHAIN_DIR is not defined. Please set the toolchain dir.")
# endif()

set(COMPILER_PREFIX "aarch64-linux-gnu-")
set(CMAKE_SYSROOT ${HG_PROJECT_ROOT_FS})
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/${COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/${COMPILER_PREFIX}g++)

set(FLAGS_COMMONS "-fPIC -fstack-protector-all -Wformat -Wformat-security -Werror=format-security")
set(FLAGS_OPTIMAL "-Og -pipe -g -feliminate-unused-debug-types -Wl,--strip-all")
set(FLAGS_LD "-Wl,-Og -fstack-protector-all -Wl,-z,relro,-z,now")
set(CMAKE_CXX_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL} ${FLAGS_SPECIAL} -std=c++11" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "${FLAGS_COMMONS} ${FLAGS_OPTIMAL} ${FLAGS_SPECIAL} " CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "${FLAGS_LD}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${FLAGS_LD}" CACHE STRING "" FORCE)


option(BUILD_TESTING "Build tests" OFF)