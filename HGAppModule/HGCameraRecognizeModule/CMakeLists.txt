project(HGCameraRecognizeModule VERSION 1.1.1)

# Define Qt libraries based on OS



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # QML
set(CMAKE_AUTORCC ON) # rc

file(GLOB MOC_HEADERS 
    "inc/*.h"
)

# 根据Qt版本选择对应的MOC命令


HG_CMAKE_ADD_MODELU(CameraRecognizeModule inc src 
    ${MOC_SRCS})

target_compile_definitions(CameraRecognizeModule PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGCameraRecognizeModule\")

HG_CMAKE_MODULE_DEPEND_PUB(
    PluginInterface
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGSecurityService
    CameraRecognizeInterface
    HGBaseAppModule
    ${ZXING_LIBRARY}
    ${OpenCV_LIBRARIES}
    

    

    

    

    

)

HG_CMAKE_ADD_SHARED(HGCameraRecognizeModule inc src ${MOC_SRCS})
HG_CMAKE_MODULE_DEPEND(
    PluginInterface
    HGLowerPC
    HGCommon
    HGConfig
    HGSaveService
    HGFrameService
    HGSecurityService
    HGAuthorityService
    CameraRecognizeInterface
    ${ZXING_LIBRARY}
    HGBaseAppModule
    ${OpenCV_LIBRARIES}
    

    

    

    

    


    ${QT_LIBRARIES}
)
# HG_CMAKE_ADD_EXE(HGCameraRecognizeModule main.cpp)

# HG_CMAKE_MODULE_DEPEND(
#     CameraRecognizeModule
# 
#     ${QT_LIBRARIES}
# )

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/version.h"
# )

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
