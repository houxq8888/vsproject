HG_CMAKE_ADD_MODELU(CommonCommunicationDemo inc src)

target_compile_definitions(CommonCommunicationDemo PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQCommonCommunicationDemo\")

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


HG_CMAKE_ADD_EXE(HXQCommonCommunicationDemo main.cpp)

HG_CMAKE_MODULE_DEPEND(
    CommonCommunicationDemo
)

