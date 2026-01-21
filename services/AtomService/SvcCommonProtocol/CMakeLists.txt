set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_MODULE(SvcCommonProtocol inc src)
HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGConfig
    ${LIB_SERIALPORT_LIBRARY}
)

HG_CMAKE_ADD_SHARED(HGSvcCommonProtocol inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGConfig
    ${LIB_SERIALPORT_LIBRARY}
)

elseif (PLATFORM STREQUAL "win32")

# 头文件目录
include_directories(
   inc
   /home/ubuntu1804/code/install/win32/include
   ../../HGConfig/inc
)

# 收集源文件
file(GLOB SOURCES src/*.cpp)

add_library(HGSvcCommonProtocol STATIC ${SOURCES})


endif()


set_target_properties(HGSvcCommonProtocol PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)