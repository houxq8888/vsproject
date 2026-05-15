set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfCameraControl inc src ../../)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGHwSensor
    HGServiceInterfaces
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfCameraControl inc src
    ../../
)

HG_CMAKE_MODULE_DEPEND(
    HGHwSensor
    HGServiceInterfaces
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfCameraControl inc src 
    ../../
    ../../services/BaseCommon/inc                                   ../../services/BaseCommon/src
    ../../services/BaseConfig/inc                                   ../../services/BaseConfig/src
    ../../services/AtomService/SvcImageAlgorithm/inc      ../../services/AtomService/SvcImageAlgorithm/src
    ../../services/Hardware/HwSensor/inc                        ../../services/Hardware/HwSensor/src
)

INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
target_link_libraries(HGIfCameraControl
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(HGIfCameraControl PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()