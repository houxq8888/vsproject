set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS "inc/*.h")

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(AppCupDet inc src ${MOC_SRCS})


target_compile_definitions(AppCupDet PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppCupDet\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGIfCupDet
    HGIfCameraControl
    HGIfLog
    HGServiceInterfaces
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets
    ${OpenCV_LIBRARIES}
)

HG_CMAKE_ADD_EXE(HGAppCupDet main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppCupDet
)