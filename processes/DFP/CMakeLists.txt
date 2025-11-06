HG_CMAKE_ADD_MODELU(DFPProcess inc src)

target_compile_definitions(DFPProcess PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQDFPProcess\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    ${OpenCV_LIBRARIES}
    ${LOG4CPLUS_LIBRARIES}
)


HG_CMAKE_ADD_EXE(HXQDFPProcess inc src main.cpp)

HG_CMAKE_MODULE_DEPEND(
    DFPProcess
)

