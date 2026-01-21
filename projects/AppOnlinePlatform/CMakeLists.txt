project(HGAppOnlinePlatform VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
    "inc/Device/SystemSet/*.h"
    "inc/Device/DeviceManage/*.h"
    "inc/Data/WarnInfo/*.h"
    "inc/Data/ErrCodeInfo/*.h"
    "inc/Maintenance/Device/*.h"
    "inc/Maintenance/Material/*.h"
    "inc/MainPage/RunStatus/*.h"
    "inc/Control/*.h"

)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(AppOnlinePlatform inc src 
    inc/Device/SystemSet                        src/Device/SystemSet 
    inc/Device/DeviceManage                     src/Device/DeviceManage
    inc/Data/WarnInfo                           src/Data/WarnInfo
    inc/Data/ErrCodeInfo                        src/Data/ErrCodeInfo
    inc/Maintenance/Device                      src/Maintenance/Device
    inc/Maintenance/Material                    src/Maintenance/Material
    inc/MainPage/RunStatus                      src/MainPage/RunStatus
    inc/Control                                 src/Control
    ${MOC_SRCS})

target_compile_definitions(AppOnlinePlatform PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppOnlinePlatform\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGAuthorityService
    HGCommunicateService
    HGImageAlgorithmService
    HGSecurityService
    HGOnlinePlatformModule
    HGOnlinePlatformInterface
    HGUserAuditModule
    HGPrintModule
    HGCurveModule
    HGLoginModule
    HGMethodModule
    HGFlowModule
    HGTaskModule
    HGLogModule
    HGExceptionHandleModule
    HGSearchModule
    HGBaseAppModule
    HGAnalysisRecordModule
    HGChannelModule
    HGReagentModule
    ${ZXING_LIBRARY}
    # SQLite::SQLite3
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
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
    OpenSSL::Crypto
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::PrintSupport


)


HG_CMAKE_ADD_EXE(HGAppOnlinePlatform main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppOnlinePlatform
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
