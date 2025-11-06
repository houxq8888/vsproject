HG_CMAKE_ADD_MODELU(CommonProtocolAPI inc src)

target_compile_definitions(CommonProtocolAPI PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQCommonProtocolAPI\")

HG_CMAKE_ADD_EXE(HXQCommonProtocolAPI main.c)

HG_CMAKE_MODULE_DEPEND(
    CommonProtocolAPI
)

