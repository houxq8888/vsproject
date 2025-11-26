set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS "inc/*.h")

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(ComDebugApp inc src ${MOC_SRCS})

target_compile_definitions(ComDebugApp PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGComDebugApp\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGLowerPC
    HGCommon
    HGConfig
    HGCommunicateService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${LIB_SERIALPORT_LIBRARY}
    ${VSOMEIP_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    ${PULSE_LIBRARIES}
    ${LIBSSH_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
)


HG_CMAKE_ADD_EXE(HGComDebugApp main.cpp)

HG_CMAKE_MODULE_DEPEND(
    ComDebugApp
)

