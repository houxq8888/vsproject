HG_CMAKE_ADD_MODULE(CmdCommonProtocol inc src)

target_compile_definitions(CmdCommonProtocol PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCmdCommonProtocol\")

HG_CMAKE_ADD_EXE(HGCmdCommonProtocol main.c)

HG_CMAKE_MODULE_DEPEND(
    CmdCommonProtocol
)

