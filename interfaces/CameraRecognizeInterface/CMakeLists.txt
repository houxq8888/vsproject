set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(CRInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    HGSaveService
    HGFrameService
    HGCupDetModule
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(CameraRecognizeInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    HGSaveService
    HGFrameService
    HGCupDetModule
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(CameraRecognizeInterface inc src 
    ../../services/HGCommon/inc                                   ../../services/HGCommon/src
    ../../services/HGConfig/inc                                   ../../services/HGConfig/src
    ../../services/HGAtomService/HGImageAlgorithmService/inc      ../../services/HGImageAlgorithmService/src
    ../../services/HGAtomService/HGFrameService/inc               ../../services/HGFrameService/src
    ../../services/HGAtomService/HGSaveService/inc                ../../services/HGSaveService/src
    ../../services/HGHardware/HGSensor/inc                        ../../services/HGHardware/HGSensor/src
    ../../modules/HGCupDetModule/inc                              ../../modules/HGCupDetModule/src

)
INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
target_link_libraries(CameraRecognizeInterface
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(CameraRecognizeInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()