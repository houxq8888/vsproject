project(HGAppOnlinePlatform VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
    "inc/Device/SystemSet/*.h"
    "inc/Device/DeviceManage/*.h"
    "inc/Data/WarnInfo/*.h"
    "inc/Data/ErrCodeInfo/*.h"
    "inc/Maintenance/Device/*.h"
    "inc/Maintenance/Material/*.h"
    "inc/MainPage/RunStatus/*.h"
    "inc/Control/*.h"

)

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODULE(AppOnlinePlatform inc src 
    inc/Device/SystemSet                        src/Device/SystemSet 
    inc/Device/DeviceManage                     src/Device/DeviceManage
    inc/Data/WarnInfo                           src/Data/WarnInfo
    inc/Data/ErrCodeInfo                        src/Data/ErrCodeInfo
    inc/Maintenance/Device                      src/Maintenance/Device
    inc/Maintenance/Material                    src/Maintenance/Material
    inc/MainPage/RunStatus                      src/MainPage/RunStatus
    inc/Control                                 src/Control
    ${CMAKE_SOURCE_DIR}/../../interfaces/IfChartData/inc
    ${CMAKE_SOURCE_DIR}/../../interfaces/IfChartData/src
    ${CMAKE_SOURCE_DIR}/../../modules/CoreChartData/inc
    ${CMAKE_SOURCE_DIR}/../../modules/CoreChartData/src
    ${MOC_SRCS})

target_compile_definitions(AppOnlinePlatform PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGAppOnlinePlatform\")

HG_CMAKE_MODULE_DEPEND_PUB(
    UIBaseCore
    HGUILog
    HGUIFlow
    HGUILogin
    HGUIChannel
    HGUITask
    HGUIMethod
    HGUIReagent
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    Qt6::PrintSupport
)


HG_CMAKE_ADD_EXE(HGAppOnlinePlatform main.cpp)

HG_CMAKE_MODULE_DEPEND(
    AppOnlinePlatform
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
