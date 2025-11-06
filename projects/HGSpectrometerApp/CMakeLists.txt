project(HGSpectrometerApp VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc



file(GLOB MOC_HEADERS 
    "inc/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(SpectrometerApp inc src 
    ${MOC_SRCS})

target_compile_definitions(SpectrometerApp PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGSpectrometerApp\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    HGConfig
    HGSaveService
    # HGFrameService
    # HGAuthorityService
    HGCommunicateService
    # HGImageAlgorithmService
    # SQLite::SQLite3
    # ALSA::ALSA
    ${OpenCV_LIBRARIES}
    # ${OpencvFreeType_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    # ${CUPS_LIBRARY}
    # ${XLSXIO_READ_LIBRARY}
    # ${XLSXIO_WRITE_LIBRARY}
    # ${HARU_LIBRARY}
    # ${ZeroMQ_LIBRARY}
    ${MQTT_LIBRARYS}
    ${LIB_MOSQUITT_LIBRARY}
    # OpenSSL::Crypto
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    # Qt5::PrintSupport
)


HG_CMAKE_ADD_EXE(HGSpectrometerApp main.cpp)

HG_CMAKE_MODULE_DEPEND(
    SpectrometerApp
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

