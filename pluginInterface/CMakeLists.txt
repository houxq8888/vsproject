project(PluginInterface VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(PluginInterfaceModule inc src 
    ${MOC_SRCS})

target_compile_definitions(PluginInterfaceModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"PluginInterface\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    ${ZXING_LIBRARY}
    ${USB_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${OpenCV_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
)

HG_CMAKE_ADD_SHARED(PluginInterface inc src ${MOC_SRCS})
# HG_CMAKE_ADD_STATIC(PluginInterface inc src ${MOC_SRCS})
HG_CMAKE_MODULE_DEPEND(
    HGCommon
    ${ZXING_LIBRARY}
    ${USB_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${OpenCV_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
)

