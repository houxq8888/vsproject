# 示例CMakeLists.txt文件，展示如何使用依赖检查模块

# 引入依赖检查模块
include(cmake/dependency_check.cmake)

# 定义项目
project(vsproject)

# 添加子目录（按层次顺序）
add_subdirectory(services/HGAtomService/HGCommon)
add_subdirectory(services/HGAtomService/HGConfig)
add_subdirectory(services/HGAtomService/HGLogService)
add_subdirectory(services/HGAtomService/HGErrorService)
add_subdirectory(services/HGAtomService/HGSecurityService)
add_subdirectory(services/HGAtomService/HGSaveService)
add_subdirectory(services/HGAtomService/HGFrameService)
add_subdirectory(services/HGAtomService/HGCommunicateService)
add_subdirectory(services/HGAtomService/HGImageAlgorithmService)
add_subdirectory(services/HGAtomService/HGAuthorityService)
add_subdirectory(services/HGAtomService/HGDataPostProcessService)
add_subdirectory(services/HGAtomService/HGRobot)
add_subdirectory(services/HGAtomService/HGSensor)
add_subdirectory(services/HGAtomService/HGLowerPC)
add_subdirectory(modules/LogModule)
add_subdirectory(modules/HGCameraRecognizeModule)
add_subdirectory(modules/HGOnlinePlatformModule)
add_subdirectory(interfaces/LogInterface)
add_subdirectory(interfaces/CameraControlInterface)
add_subdirectory(interfaces/HGOnlinePlatformInterface)
add_subdirectory(apps/HGDashboardApp)
add_subdirectory(apps/HGCupDetApp)
add_subdirectory(apps/HGOnlinePlatformApp)

# 设置模块层次
setup_module_layers()

# 验证所有模块的依赖关系
validate_all_dependencies()

# 生成依赖关系图
generate_dependency_graph(${CMAKE_BINARY_DIR}/dependency_graph.dot)

# 可选：使用Graphviz生成PNG图片
find_program(DOT_EXECUTABLE dot)
if(DOT_EXECUTABLE)
    add_custom_target(dependency_graph_png
        COMMAND ${DOT_EXECUTABLE} -Tpng ${CMAKE_BINARY_DIR}/dependency_graph.dot -o ${CMAKE_BINARY_DIR}/dependency_graph.png
        DEPENDS ${CMAKE_BINARY_DIR}/dependency_graph.dot
        COMMENT "Generating dependency graph PNG"
    )
endif()