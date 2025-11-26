project(HGAnalysisRecordModule VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
    "../../pluginInterface/inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(AnalysisRecordModule inc src   
    ../../pluginInterface/inc ../../pluginInterface/src  
    ${MOC_SRCS})

target_compile_definitions(AnalysisRecordModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAnalysisRecordModule\")

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
    HGPrintModule
    HGSearchModule
    HGCurveModule
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
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
    OpenSSL::Crypto
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::PrintSupport
)

HG_CMAKE_ADD_SHARED(HGAnalysisRecordModule inc src   
    ../../pluginInterface/inc ../../pluginInterface/src  
    ${MOC_SRCS})

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
    HGPrintModule
    HGSearchModule
    HGCurveModule
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
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::PrintSupport
)

# HG_CMAKE_ADD_EXE(HGAnalysisRecordModule main.cpp)

# HG_CMAKE_MODULE_DEPEND(
#     AnalysisRecordModule
# )

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/version.h"
# )

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
