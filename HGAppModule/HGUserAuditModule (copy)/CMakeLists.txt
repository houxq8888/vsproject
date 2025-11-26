project(HGUserAuditModule VERSION 1.1.1)

# Define Qt libraries based on OS



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

# 根据Qt版本选择对应的MOC命令


HG_CMAKE_ADD_MODELU(UserAuditModule inc src 
    ${MOC_SRCS})

target_compile_definitions(UserAuditModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGUserAuditModule\")

HG_CMAKE_MODULE_DEPEND_PUB(
    PluginInterface
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGAuthorityService
    HGCommunicateService
    HGImageAlgorithmService
    HGSecurityService
    HGBaseAppModule
    ${ZXING_LIBRARY}
    ALSA::ALSA
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    ${HARU_LIBRARY}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_SERIALPORT_LIBRARY}
    ${LIB_MOSQUITT_LIBRARY}
    ${PULSE_LIBRARIES}
    ${LIBSSH_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    ${Boost_LIBRARIES}
    OpenSSL::Crypto
    

    

    

    

    

)

HG_CMAKE_ADD_SHARED(HGUserAuditModule inc src ${MOC_SRCS})
HG_CMAKE_MODULE_DEPEND(
    PluginInterface
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGAuthorityService
    HGCommunicateService
    HGImageAlgorithmService
    HGSecurityService
    HGBaseAppModule
    ${ZXING_LIBRARY}
    ALSA::ALSA
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${CUPS_LIBRARY}
    ${XLSXIO_READ_LIBRARY}
    ${XLSXIO_WRITE_LIBRARY}
    ${HARU_LIBRARY}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_SERIALPORT_LIBRARY}
    ${LIB_MOSQUITT_LIBRARY}
    ${PULSE_LIBRARIES}
    ${LIBSSH_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    ${Boost_LIBRARIES}
    OpenSSL::Crypto
    

    

    

    

    


    ${QT_LIBRARIES}
)

# HG_CMAKE_ADD_EXE(HGUserAuditModule main.cpp)

# HG_CMAKE_MODULE_DEPEND(
#     UserAuditModule
# 
    ${QT_LIBRARIES}
)

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/version.h"
# )

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
