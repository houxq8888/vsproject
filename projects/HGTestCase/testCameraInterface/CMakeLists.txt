HG_CMAKE_ADD_MODELU(testCameraInterface inc src)

target_compile_definitions(testCameraInterface PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQtestCameraInterface\")

HG_CMAKE_MODULE_DEPEND_PUB(
    CameraRecognizeInterface
    HGCommon
    HGImageAlgorithmService
    HGConfig
    HGSensor
    HGCupDetModule
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}  
    ${LIBPINYIN_LIBRARIES}
)

HG_CMAKE_ADD_EXE(HXQtestCameraInterface main.cpp)

HG_CMAKE_MODULE_DEPEND(
    testCameraInterface
)

