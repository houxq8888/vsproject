HG_CMAKE_ADD_MODELU(ZoneTest inc src)

target_compile_definitions(ZoneTest PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HXQZoneTest\")

HG_CMAKE_MODULE_DEPEND_PUB(

)


HG_CMAKE_ADD_EXE(HXQZoneTest main.c)

# 强制静态链接
set_target_properties(HXQZoneTest PROPERTIES LINK_FLAGS "-static")

HG_CMAKE_MODULE_DEPEND(
    ZoneTest
)

