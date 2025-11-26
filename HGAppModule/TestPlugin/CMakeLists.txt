project(HGTestPlugin VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(TestPlugin inc src 
    ${MOC_SRCS})

target_compile_definitions(TestPlugin PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGTestPlugin\")

HG_CMAKE_MODULE_DEPEND_PUB(
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::UiPlugin
    Qt5::Designer
)

HG_CMAKE_ADD_SHARED(HGTestPlugin inc src ${MOC_SRCS})
HG_CMAKE_MODULE_DEPEND(
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::UiPlugin
    Qt5::Designer
)
# HG_CMAKE_ADD_EXE(HGTestPlugin main.cpp)

# HG_CMAKE_MODULE_DEPEND(
#     TestPlugin
# )

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/version.h"
# )

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
