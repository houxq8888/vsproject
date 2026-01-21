set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfCameraRecognize inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGBaseCommon
    HGSvcImageAlgorithm
    HGBaseConfig
    HGHwSensor
    HGSvcSave
    HGSvcFrame
    HGCoreCupDet
    HGIfCameraControl
    HGIfCupDet
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfCameraRecognize inc src)

HG_CMAKE_MODULE_DEPEND(
    HGBaseCommon
    HGSvcImageAlgorithm
    HGBaseConfig
    HGHwSensor
    HGSvcSave
    HGSvcFrame
    HGCoreCupDet
    HGIfCameraControl
    HGIfCupDet
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfCameraRecognize inc src 
    ../../services/BaseCommon/inc                                   ../../services/BaseCommon/src
    ../../services/BaseConfig/inc                                   ../../services/BaseConfig/src
    ../../services/AtomService/SvcImageAlgorithm/inc                ../../services/AtomService/SvcImageAlgorithm/src
    ../../services/AtomService/SvcFrame/inc                         ../../services/AtomService/SvcFrame/src
    ../../services/AtomService/SvcSave/inc                          ../../services/AtomService/SvcSave/src
    ../../services/Hardware/HwSensor/inc                            ../../services/Hardware/HwSensor/src
    ../../modules/CoreCupDet/inc                                    ../../modules/CoreCupDet/src
    ../IfCameraControl/inc                                          ../IfCameraControl/src
    ../IfCupDet/inc                                                 ../IfCupDet/src

)
INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
target_link_libraries(HGIfCameraRecognize
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ws2_32 iphlpapi)

endif()

if (PLATFORM STREQUAL "x64" OR PLATFORM STREQUAL "win32")
set_target_properties(HGIfCameraRecognize PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
endif()