HG_CMAKE_ADD_MODELU(DetectDimentionVuxProcess inc src)

target_compile_definitions(DetectDimentionVuxProcess PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQDetectDimentionVuxProcess\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
    ${ZXING_LIBRARY}
    HXQDetectDimentionVuxInterface
)


HG_CMAKE_ADD_EXE(HXQDetectDimentionVuxProcess main.cpp)

HG_CMAKE_MODULE_DEPEND(
    DetectDimentionVuxProcess
)

