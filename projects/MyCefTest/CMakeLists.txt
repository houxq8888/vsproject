project(CefTest VERSION 1.0.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc
set(CEF_ROOT /mnt/extra/projects/cef_binary_84.3.10+ga46056b+chromium-84.0.4147.105_linux64)

# 添加 CEF 库路径
include_directories(${CEF_ROOT})
link_directories(${CEF_ROOT}/Release)

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(CefTest inc src 
    ${MOC_SRCS})

target_compile_definitions(CefTest PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"CefTest\")

HG_CMAKE_MODULE_DEPEND_PUB(
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::X11Extras
    X11
    # libcef
    ${CEF_ROOT}/Release/libcef.so
)

HG_CMAKE_ADD_EXE(MyCefTest main.cpp)


# set(SQLCIPHER_INCLUDE_DIR /usr/local/include/sqlcipher)
set(CEF_LIBRARY ${CEF_ROOT}/Release/libcef.so)

# target_include_directories(MyCefTest PRIVATE ${SQLCIPHER_INCLUDE_DIR})
# target_link_libraries(MyCefTest PRIVATE ${CEF_LIBRARY})

HG_CMAKE_MODULE_DEPEND(
    CefTest
)

# target_link_libraries(MyCefTest
#     Qt5::Widgets
#     Qt5::X11Extras
#     ${CEF_ROOT}/Release/libcef.so
#     ${CEF_ROOT}/Release/libcef_dll_wrapper.a
# )

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")