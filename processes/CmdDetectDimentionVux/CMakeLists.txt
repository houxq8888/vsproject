HG_CMAKE_ADD_MODULE(CmdDetectDimentionVux inc src)

target_compile_definitions(CmdDetectDimentionVux PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCmdDetectDimentionVux\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGIfDetectDimentionVux
)


HG_CMAKE_ADD_EXE(HGCmdDetectDimentionVux main.cpp)

HG_CMAKE_MODULE_DEPEND(
    CmdDetectDimentionVux
)

