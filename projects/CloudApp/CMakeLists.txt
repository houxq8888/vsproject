project(CloudApp VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

find_package(PCL REQUIRED PATHS /opt/pcl-1.11.1)
find_package(VTK REQUIRED PATHS /opt/vtk-8.2)
#     COMPONENTS
#     CommonCore
#     CommonDataModel
#     FiltersSources
#     RenderingCore
#     RenderingOpenGL2
#     InteractionStyle
#     # RenderingQt
#     # GUISupportQt
# )

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})

# 添加 PCL 定义宏
add_definitions(${PCL_DEFINITIONS})


file(GLOB MOC_HEADERS 
    "inc/*.h"
    # "inc/Device/SystemSet/*.h"
    # "inc/Device/DeviceManage/*.h"
    # "inc/Device/ReagentManage/*.h"
    # "inc/Device/ChannelSet/*.h"
    # "inc/Data/AnalysisRecord/*.h"
    # "inc/Data/WarnInfo/*.h"
    # "inc/Data/ErrCodeInfo/*.h"
    # "inc/Data/LogRecord/*.h"
    # "inc/Maintenance/Device/*.h"
    # "inc/Maintenance/Material/*.h"
    # "inc/Maintenance/Exception/*.h"
    # "inc/MainPage/Method/*.h"
    # "inc/MainPage/Flow/*.h"
    # "inc/MainPage/RunStatus/*.h"
    # "inc/MainPage/TaskSequence/*.h"
    # "inc/Control/*.h"
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

HG_CMAKE_ADD_MODELU(MCloudApp inc src 
    # inc/Device/SystemSet        src/Device/SystemSet 
    # inc/Device/DeviceManage     src/Device/DeviceManage
    # inc/Device/ChannelSet       src/Device/ChannelSet
    # inc/Device/ReagentManage    src/Device/ReagentManage
    # inc/Data/AnalysisRecord     src/Data/AnalysisRecord
    # inc/Data/WarnInfo           src/Data/WarnInfo
    # inc/Data/ErrCodeInfo        src/Data/ErrCodeInfo
    # inc/Data/LogRecord          src/Data/LogRecord
    # inc/Maintenance/Device      src/Maintenance/Device
    # inc/Maintenance/Material    src/Maintenance/Material
    # inc/Maintenance/Exception   src/Maintenance/Exception
    # inc/MainPage/Method         src/MainPage/Method
    # inc/MainPage/Flow           src/MainPage/Flow
    # inc/MainPage/RunStatus      src/MainPage/RunStatus
    # inc/MainPage/TaskSequence   src/MainPage/TaskSequence
    # inc/Control                 src/Control

    ${MOC_SRCS})

target_compile_definitions(MCloudApp PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"CloudApp\")

HG_CMAKE_MODULE_DEPEND_PUB(
    HGCommon
    HGConfig
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
    ${PCL_LIBRARIES}
    ${VTK_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::PrintSupport
)


HG_CMAKE_ADD_EXE(CloudApp main.cpp)

HG_CMAKE_MODULE_DEPEND(
    MCloudApp
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
