set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(CCInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    ${OpenCV_LIBRARIES}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(CameraControlInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    ${OpenCV_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(CameraControlInterface inc src 
    ../../services/HGCommon/inc                                   ../../services/HGCommon/src
    ../../services/HGConfig/inc                                   ../../services/HGConfig/src
    ../../services/HGAtomService/HGImageAlgorithmService/inc      ../../services/HGImageAlgorithmService/src
    ../../services/HGHardware/HGSensor/inc                        ../../services/HGHardware/HGSensor/src
)

INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
target_link_libraries(CameraControlInterface
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(CameraControlInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()