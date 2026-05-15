HG_CMAKE_ADD_MODULE(TestYModem inc src)

target_compile_definitions(TestYModem PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGTestYModem\")

HG_CMAKE_MODULE_DEPEND_PUB(
    ${LIBSERIAL_LIBRARIES}
)


HG_CMAKE_ADD_EXE(HGTestYModem a.cpp)

HG_CMAKE_MODULE_DEPEND(
    TestYModem
)

