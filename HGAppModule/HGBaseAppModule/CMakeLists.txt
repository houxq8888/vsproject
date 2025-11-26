# Define Qt libraries based on OS
if(WIN32)
    set(QT_LIBRARIES
        Qt5::Gui
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::PrintSupport
    )
else()
    set(QT_LIBRARIES
        Qt5::Gui
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::PrintSupport
    )
endif()


set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS "inc/*.h")

# 根据Qt版本选择对应的MOC命令


HG_CMAKE_ADD_MODELU(BaseAppModule inc src ${MOC_SRCS})


HG_CMAKE_MODULE_DEPEND_PUB( 
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    HGFrameService
    HGSaveService
    HGLowerPC
    HGSecurityService
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
    ${HARU_LIBRARY}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    ${ZXING_LIBRARY}
    
    
)
if (PLATFORM STREQUAL "x64")
HG_CMAKE_ADD_SHARED(HGBaseAppModule inc src ${MOC_SRCS})

    set_target_properties(HGBaseAppModule PROPERTIES 
        VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
        SOVERSION ${LIBRARY_VERSION_MAJOR}
    )

HG_CMAKE_MODULE_DEPEND(
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    HGFrameService
    HGSaveService
    HGLowerPC
    HGSecurityService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
    ${HARU_LIBRARY}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    
    

    ${QT_LIBRARIES}
)
elseif (PLATFORM STREQUAL "win32")
HG_CMAKE_ADD_STATIC(HGBaseAppModule inc src 
    ../../services/HGCommon/inc                                   ../../services/HGCommon/src
    ../../services/HGConfig/inc                                   ../../services/HGConfig/src
    ../../services/HGAtomService/HGImageAlgorithmService/inc      ../../services/HGImageAlgorithmService/HGSaveService/src
    ../../services/HGHardware/HGSensor/inc                        ../../services/HGHardware/HGSensor/src
    ../../modules/HGCupDetModule/inc                              ../../modules/HGCupDetModule/src

)
INCLUDE_DIRECTORIES(${HG_PROJECT_ROOT_FS}/include/opencv4)
target_link_libraries(HGBaseAppModule
    ${HG_PROJECT_ROOT_FS}/lib/libsqlcipher.a
    ${HG_PROJECT_ROOT_FS}/lib/libcserialport.a
    ${HG_PROJECT_ROOT_FS}/lib/libssl.a
    ${HG_PROJECT_ROOT_FS}/lib/libcrypto.a
    ${QT_LIBRARIES}
    ws2_32 iphlpapi)

    set_target_properties(HGBaseAppModule PROPERTIES 
        VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
        SOVERSION ${LIBRARY_VERSION_MAJOR}
    )

endif()
