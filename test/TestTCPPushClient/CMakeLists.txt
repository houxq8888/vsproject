HG_CMAKE_ADD_MODULE(TestTCPPushClient inc src)

target_compile_definitions(TestTCPPushClient PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGTestTCPPushClient\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    HGConfig
    HGCommunicateService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${VSOMEIP_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    ${PULSE_LIBRARIES}
    ${LIBSSH_LIBRARIES}
    ${LIB_SERIALPORT_LIBRARY}
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)


HG_CMAKE_ADD_EXE(HGTestTCPPushClient main.cpp)

HG_CMAKE_MODULE_DEPEND(
    TestTCPPushClient
)

