project(HGProductFlow VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"

)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(ProductFlow inc src 
    ${MOC_SRCS})

target_compile_definitions(ProductFlow PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGProductFlow\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGBaseAppModule
    PluginInterface
    HGFrameService
    HGSaveService
    HGCommon
    HGConfig
    HGLowerPC
    HGSecurityService
    ${ZXING_LIBRARY}
    ${USB_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${OpenCV_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets


)


HG_CMAKE_ADD_EXE(HGProductFlow main.cpp)

HG_CMAKE_MODULE_DEPEND(
    ProductFlow
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
