set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODULE(IfCupDet inc src ../../)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCoreCupDet
    HGServiceInterfaces
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGIfCupDet inc src
    ../../
)

HG_CMAKE_MODULE_DEPEND(
    HGCoreCupDet
    HGServiceInterfaces
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGIfCupDet inc src
    ../../
    ../../services/BaseCommon/inc                                 ../../services/BaseCommon/src
    ../../services/BaseConfig/inc                                 ../../services/BaseConfig/src    
    ../../services/AtomService/SvcImageAlgorithm/inc              ../../services/AtomService/SvcImageAlgorithm/src
    ../../services/AtomService/SvcSave/inc                        ../../services/AtomService/SvcSave/src
    ../../modules/CoreCupDet/inc                                  ../../modules/CoreCupDet/src
)

INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
endif()

set_target_properties(HGIfCupDet PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)