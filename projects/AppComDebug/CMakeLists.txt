set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS "inc/*.h")

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(AppComDebug inc src ${MOC_SRCS})

target_compile_definitions(AppComDebug PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppComDebug\")

HG_CMAKE_MODULE_DEPEND_PUB(
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets
    HGIfSerial
    HGIfLog
)


HG_CMAKE_ADD_EXE(HGAppComDebug main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppComDebug
)

