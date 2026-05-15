HG_CMAKE_ADD_MODULE(TestCommonCommunicationDemo inc src)

target_compile_definitions(TestCommonCommunicationDemo PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCommonCommunicationDemo\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    HGConfig
    CommonProtocolService
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${LIB_SERIALPORT_LIBRARY}
    OpenSSL::Crypto    
    /usr/lib/x86_64-linux-gnu/libpulse.so
    /usr/lib/x86_64-linux-gnu/libpulse-simple.so
)


HG_CMAKE_ADD_EXE(HGTestCommonCommunicationDemo main.cpp)

HG_CMAKE_MODULE_DEPEND(
    TestCommonCommunicationDemo
)

