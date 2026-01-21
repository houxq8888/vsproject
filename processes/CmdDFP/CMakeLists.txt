HG_CMAKE_ADD_MODULE(CmdDFP inc src)

target_compile_definitions(CmdDFP PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCmdDFP\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    ${OpenCV_LIBRARIES}
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
)


HG_CMAKE_ADD_EXE(HGCmdDFP inc src main.cpp)

HG_CMAKE_MODULE_DEPEND(
    CmdDFP
)

