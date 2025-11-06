HG_CMAKE_ADD_MODELU(YModemTest inc src)

target_compile_definitions(YModemTest PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"YModemTests\")

HG_CMAKE_MODULE_DEPEND_PUB(
    ${LIBSERIAL_LIBRARIES}
)


HG_CMAKE_ADD_EXE(HGYModemTest a.cpp)

HG_CMAKE_MODULE_DEPEND(
    YModemTest
)

