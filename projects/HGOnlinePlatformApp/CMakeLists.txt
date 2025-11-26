project(HGOnlinePlatformApp VERSION 1.1.1)

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

HG_CMAKE_ADD_MODELU(OnlinePlatformApp inc src 
    inc/Device/SystemSet                        src/Device/SystemSet 
    inc/Device/DeviceManage                     src/Device/DeviceManage
    inc/Data/WarnInfo                           src/Data/WarnInfo
    inc/Data/ErrCodeInfo                        src/Data/ErrCodeInfo
    inc/Maintenance/Device                      src/Maintenance/Device
    inc/Maintenance/Material                    src/Maintenance/Material
    inc/MainPage/RunStatus                      src/MainPage/RunStatus
    inc/Control                                 src/Control
    ${MOC_SRCS})

target_compile_definitions(OnlinePlatformApp PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGOnlinePlatformApp\")

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


HG_CMAKE_ADD_EXE(HGOnlinePlatformApp main.cpp)

HG_CMAKE_MODULE_DEPEND(
    OnlinePlatformApp
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# # 配置翻译文件
# set(TS_FILES
#     ${CMAKE_CURRENT_SOURCE_DIR}/translations/HGOnlinePlatformApp_zh_CN.ts
#     ${CMAKE_CURRENT_SOURCE_DIR}/translations/HGOnlinePlatformApp_en_US.ts
# )
# # 添加自定义目标来生成 .ts 文件
# find_program(LUPDATE_EXECUTABLE lupdate)
# if (LUPDATE_EXECUTABLE)
#     message(STATUS "Translation files: ${CMAKE_CURRENT_SOURCE_DIR}")
#     add_custom_target(update_translations
#         COMMAND ${LUPDATE_EXECUTABLE} -recursive -ts ${TS_FILES}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#         COMMENT "Updating translations"
#     )
# endif()

# # 添加自定义目标来编译 .ts 文件为 .qm 文件
# find_program(LRELEASE_EXECUTABLE lrelease)
# if (LRELEASE_EXECUTABLE)
#     add_custom_target(release_translations
#         COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILES}
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#         COMMENT "Compiling translations"
#     )
# endif()

# # 添加翻译资源文件
# set(QM_FILES
#     translations/HGOnlinePlatformApp_en.qm
#     translations/HGOnlinePlatformApp_zh.qm
# )

# # 将 .qm 文件添加到资源
# QT5_ADD_RESOURCES(QM_RESOURCES "translations" PREFIX "/translations" FILES ${QM_FILES})
# target_sources(HGOnlinePlatformApp PRIVATE ${QM_RESOURCES})
