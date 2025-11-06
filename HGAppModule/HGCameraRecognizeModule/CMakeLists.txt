project(HGCameraRecognizeModule VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(CameraRecognizeModule inc src 
    ${MOC_SRCS})

target_compile_definitions(CameraRecognizeModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCameraRecognizeModule\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGSecurityService
    CameraRecognizeInterface
    HGBaseAppModule
    ${OpenCV_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::PrintSupport
)

# HG_CMAKE_ADD_SHARED(HGCameraRecognizeModule inc src ${MOC_SRCS})
# HG_CMAKE_MODULE_DEPEND(
#     HGLowerPC
#     HGCommon
#     HGConfig
#     HGSaveService
#     HGFrameService
#     HGSecurityService
#     HGAuthorityService
#     HGBaseAppModule
#     ${OpenCV_LIBRARIES}
#     Qt5::Gui
#     Qt5::Core
#     Qt5::Widgets
#     Qt5::Charts
#     Qt5::PrintSupport
# )
HG_CMAKE_ADD_EXE(HGCameraRecognizeModule main.cpp)

HG_CMAKE_MODULE_DEPEND(
    CameraRecognizeModule
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
