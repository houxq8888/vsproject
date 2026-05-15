# 依赖关系检查CMake模块
# 用于检查模块间的依赖关系，确保符合依赖层次设计

# 定义模块层次映射
set(LAYER_MAPPING
    "HGCommon;HGConfig;HGExactTime"  # 第0层 - 基础设施层
    "HGLogService;HGErrorService;HGSecurityService"  # 第1层 - 系统服务层
    "HGSaveService;HGFrameService;HGCommunicateService"  # 第2层 - 数据服务层
    "HGImageAlgorithmService;HGAuthorityService;HGDataPostProcessService"  # 第3层 - 业务服务层
    "HGRobot;HGSensor;HGLowerPC"  # 第4层 - 硬件抽象层
    "LogModule;HGCameraRecognizeModule;HGOnlinePlatformModule"  # 第5层 - 模块层
    "LogInterface;CameraControlInterface;HGOnlinePlatformInterface"  # 第6层 - 接口层
    "HGDashboardApp;HGCupDetApp;HGOnlinePlatformApp"  # 第7层 - 应用层
)

# 全局变量，用于存储已检查的依赖关系
set(GLOBAL_DEPENDENCY_GRAPH "" CACHE INTERNAL "Global dependency graph")

# 获取模块所在的层次
function(get_module_layer module result_var)
    set(layer -1)
    list(LENGTH LAYER_MAPPING layer_count)
    math(EXPR max_layer "${layer_count} - 1")
    
    foreach(i RANGE ${max_layer})
        list(GET LAYER_MAPPING ${i} layer_modules)
        string(REPLACE ";" " " layer_modules_str "${layer_modules}")
        if(layer_modules MATCHES "${module}")
            set(layer ${i})
            break()
        endif()
    endforeach()
    
    set(${result_var} ${layer} PARENT_SCOPE)
endfunction()

# 检查循环依赖
function(check_circular_dependencies target)
    if(NOT TARGET ${target})
        message(WARNING "Target ${target} does not exist, skipping circular dependency check")
        return()
    endif()
    
    # 获取目标的所有依赖
    get_target_property(depends ${target} LINK_LIBRARIES)
    if(NOT depends)
        return()
    endif()
    
    # 检查是否存在循环依赖
    foreach(dep ${depends})
        if(dep STREQUAL ${target})
            message(FATAL_ERROR "Circular dependency detected for ${target}")
        endif()
    endforeach()
    
    # 检查间接循环依赖
    foreach(dep ${depends})
        if(TARGET ${dep})
            get_target_property(dep_depends ${dep} LINK_LIBRARIES)
            if(dep_depends)
                foreach(dep_dep ${dep_depends})
                    if(dep_dep STREQUAL ${target})
                        message(FATAL_ERROR "Indirect circular dependency detected: ${target} -> ${dep} -> ${target}")
                    endif()
                endforeach()
            endif()
        endif()
    endforeach()
    
    # 记录依赖关系到全局图
    foreach(dep ${depends})
        if(TARGET ${dep})
            string(APPEND GLOBAL_DEPENDENCY_GRAPH "${target}->${dep}\n")
            set(GLOBAL_DEPENDENCY_GRAPH "${GLOBAL_DEPENDENCY_GRAPH}" CACHE INTERNAL "Global dependency graph")
        endif()
    endforeach()
endfunction()

# 检查跨层依赖
function(check_layer_dependencies target layer)
    if(NOT TARGET ${target})
        message(WARNING "Target ${target} does not exist, skipping layer dependency check")
        return()
    endif()
    
    # 获取目标的所有依赖
    get_target_property(depends ${target} LINK_LIBRARIES)
    if(NOT depends)
        return()
    endif()
    
    # 检查是否存在跨层依赖
    foreach(dep ${depends})
        if(TARGET ${dep})
            get_module_layer(${dep} dep_layer)
            
            # 如果依赖的模块层次未知，跳过检查（可能是第三方库）
            if(dep_layer EQUAL -1)
                continue()
            endif()
            
            # 检查是否依赖了上层模块
            if(dep_layer GREATER ${layer})
                message(WARNING "Invalid cross-layer dependency detected: ${target} (layer ${layer}) -> ${dep} (layer ${dep_layer})")
            endif()
        endif()
    endforeach()
endfunction()

# 检查同层依赖
function(check_same_layer_dependencies target layer)
    if(NOT TARGET ${target})
        message(WARNING "Target ${target} does not exist, skipping same-layer dependency check")
        return()
    endif()
    
    # 获取目标的所有依赖
    get_target_property(depends ${target} LINK_LIBRARIES)
    if(NOT depends)
        return()
    endif()
    
    # 检查同层依赖
    foreach(dep ${depends})
        if(TARGET ${dep})
            get_module_layer(${dep} dep_layer)
            
            # 如果是同层依赖，发出警告（同层依赖应尽量减少）
            if(dep_layer EQUAL ${layer})
                message(STATUS "Same-layer dependency detected: ${target} (layer ${layer}) -> ${dep} (layer ${dep_layer})")
            endif()
        endif()
    endforeach()
endfunction()

# 综合依赖检查函数
function(check_module_dependencies target layer)
    message(STATUS "Checking dependencies for ${target} (layer ${layer})")
    
    check_circular_dependencies(${target})
    check_layer_dependencies(${target} ${layer})
    check_same_layer_dependencies(${target} ${layer})
endfunction()

# 生成依赖关系图
function(generate_dependency_graph output_file)
    file(WRITE ${output_file} "digraph Dependencies {\n")
    file(APPEND ${output_file} "    rankdir=TB;\n")
    file(APPEND ${output_file} "    node [shape=box];\n")
    
    # 添加节点和层次信息
    list(LENGTH LAYER_MAPPING layer_count)
    math(EXPR max_layer "${layer_count} - 1")
    
    foreach(i RANGE ${max_layer})
        list(GET LAYER_MAPPING ${i} layer_modules)
        string(REPLACE ";" " " layer_modules_str "${layer_modules}")
        file(APPEND ${output_file} "    subgraph cluster_layer${i} {\n")
        file(APPEND ${output_file} "        label=\"Layer ${i}\";\n")
        
        foreach(module ${layer_modules})
            if(TARGET ${module})
                file(APPEND ${output_file} "        \"${module}\";\n")
            endif()
        endforeach()
        
        file(APPEND ${output_file} "    }\n")
    endforeach()
    
    # 添加边
    file(APPEND ${output_file} "\n")
    file(APPEND ${output_file} "${GLOBAL_DEPENDENCY_GRAPH}")
    file(APPEND ${output_file} "}\n")
    
    message(STATUS "Dependency graph generated to ${output_file}")
endfunction()

# 验证所有模块的依赖关系
function(validate_all_dependencies)
    list(LENGTH LAYER_MAPPING layer_count)
    math(EXPR max_layer "${layer_count} - 1")
    
    foreach(i RANGE ${max_layer})
        list(GET LAYER_MAPPING ${i} layer_modules)
        foreach(module ${layer_modules})
            if(TARGET ${module})
                check_module_dependencies(${module} ${i})
            endif()
        endforeach()
    endforeach()
endfunction()

# 设置目标层次属性
function(set_target_layer target layer)
    if(NOT TARGET ${target})
        message(WARNING "Target ${target} does not exist, cannot set layer")
        return()
    endif()
    
    set_target_properties(${target} PROPERTIES LAYER ${layer})
endfunction()

# 批量设置模块层次
function(setup_module_layers)
    list(LENGTH LAYER_MAPPING layer_count)
    math(EXPR max_layer "${layer_count} - 1")
    
    foreach(i RANGE ${max_layer})
        list(GET LAYER_MAPPING ${i} layer_modules)
        foreach(module ${layer_modules})
            if(TARGET ${module})
                set_target_layer(${module} ${i})
            endif()
        endforeach()
    endforeach()
endfunction()