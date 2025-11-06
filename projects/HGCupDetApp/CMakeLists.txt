set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS "inc/*.h")

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(CupDetApp inc src ${MOC_SRCS})


target_compile_definitions(CupDetApp PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCupDetApp\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCupDetInterface
    HGSensor
    HGCommon
    HGSaveService
    HGImageAlgorithmService
    ${OpenCV_LIBRARIES}
    ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
)

HG_CMAKE_ADD_EXE(HGCupDetApp main.cpp)

HG_CMAKE_MODULE_DEPEND(
    CupDetApp
)