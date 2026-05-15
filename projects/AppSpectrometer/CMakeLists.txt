project(HGAppSpectrometer VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc



file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(AppSpectrometer inc src 
    ${MOC_SRCS})

target_compile_definitions(AppSpectrometer PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppSpectrometer\")

HG_CMAKE_MODULE_DEPEND_PUB(
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    # Qt6::PrintSupport
)


HG_CMAKE_ADD_EXE(HGAppSpectrometer main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppSpectrometer
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

