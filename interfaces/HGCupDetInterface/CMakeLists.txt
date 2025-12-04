set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(CupDetInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCupDetModule
    HGSensor
    HGImageAlgorithmService
    HGCommon
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGCupDetInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HGCupDetModule
    HGSensor
    HGImageAlgorithmService
    HGCommon
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGCupDetInterface inc src
    ../../services/HGCommon/inc                                 ../../services/HGCommon/src
    ../../services/HGConfig/inc                                 ../../services/HGConfig/src    
    ../../services/HGHardware/HGSensor/inc                      ../../services/HGHardware/HGSensor/src
    ../../services/HGAtomService/HGImageAlgorithmService/inc    ../../services/HGAtomService/HGImageAlgorithmService/src
    ../../services/HGAtomService/HGSaveService/inc              ../../services/HGAtomService/HGSaveService/src
    ../../modules/HGCupDetModule/inc                            ../../modules/HGCupDetModule/src
)

INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
endif()

set_target_properties(HGCupDetInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)