project(HGAppDashboard VERSION 1.1.7)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc



file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
qt6_add_resources(QT_RESOURCES resources.qrc)

HG_CMAKE_ADD_MODULE(AppDashboard inc src 
    ${MOC_SRCS}
    ${QT_RESOURCES}
)

target_compile_definitions(AppDashboard PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppDashboard\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGIfLog
    HGUIBaseCore
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
   # Qt5::Quick
   # Qt5::QuickWidgets
   # Qt5::Positioning
   # Qt5::Location
   # Qt5::WebEngine
   # Qt5::WebEngineWidgets 
)


HG_CMAKE_ADD_EXE(HGAppDashboard main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppDashboard
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

