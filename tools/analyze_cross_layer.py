#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
from collections import defaultdict
import sys

def normalize_module_name(name):
    """标准化模块名称，处理命名差异"""
    # 处理HG前缀 - 如果模块名以HG开头，去掉HG前缀
    if name.startswith("HG"):
        name = name[2:]
    
    # 处理常见的后缀
    if name.endswith("Module"):
        name = name[:-6] + "Module"
    elif name.endswith("Service"):
        name = name[:-7] + "Service"
    elif name.endswith("Interface"):
        name = name[:-9] + "Interface"
    elif name.endswith("App"):
        name = name[:-3] + "App"
    
    # 处理特殊映射
    special_mappings = {
        # 第0层 - 基础设施层 (Base前缀)
        "Common": "BaseCommon",
        "Config": "BaseConfig",
        
        # 第1层 - 原子服务层 (Svc前缀)
        "LogService": "SvcLog",
        "AuthorityService": "SvcAuthority",
        "InteractiveService": "SvcInteractive",
        "ImageAlgorithmService": "SvcImageAlgorithm",
        "DataPostProcessService": "SvcDataPostProcess",
        "SaveService": "SvcSave",
        "FrameService": "SvcFrame",
        "CommunicateService": "SvcCommunicate",
        "ErrorService": "SvcError",
        "OTAService": "SvcOTA",
        "SecurityService": "SvcSecurity",
        
        # 第2层 - 硬件抽象层 (Hw前缀)
        "Robot": "HwRobot",
        "Sensor": "HwSensor",
        "LowerPC": "HwLowerPC",
        
        # 第3层 - 模块层 (Core前缀)
        "CupDet": "CoreCupDet",
        "Dashboard": "CoreDashboard",
        "OnlinePlatform": "CoreOnlinePlatform",
        "V6": "CoreV6",
        "MLogModule": "CoreLog",  # 第3层，不带UI
        
        # 第4层 - 接口层 (If前缀)
        "CameraControlInterface": "IfCameraControl",
        "CameraRecognizeInterface": "IfCameraRecognize",
        "CupDetInterface": "IfCupDet",
        "OnlinePlatformInterface": "IfOnlinePlatform",
        "V6Interface": "IfV6",
        "LogInterface": "IfLog",
        "PackFuncInterface": "IfPackFunc",
        
        # 第5层 - UI应用模块层 (UI前缀)
        "AnalysisRecordModule": "UIAnalysisRecord",
        "BaseAppModule": "UIBaseApp",
        "CameraRecognizeModule": "UICameraRecognize",
        "ChannelModule": "UIChannel",
        "CurveModule": "UICurve",
        "SharedFileModule": "UISharedFile",
        "ReagentModule": "UIReagent",
        "ScannerModule": "UIScanner",
        "SearchModule": "UISearch",
        "UartModule": "UIUart",
        "LoginModule": "UILogin",
        "MethodModule": "UIMethod",
        "UserAuditModule": "UIUserAudit",
        "TaskModule": "UITask",
        "FlowModule": "UIFlow",
        "PrintModule": "UIPrint",
        "EBalanceModule": "UIEBalance",
        "ExceptionHandleModule": "UIExceptionHandle",
        "pluginInterface": "UIPluginInterface",
        "LogModule": "UILog",  # 第5层，带UI
        "UILogModule": "UILog",  # 第5层，带UI
        
        # 第6层 - 应用层 (App前缀)
        "DashboardApp": "AppDashboard",
        "OnlinePlatformApp": "AppOnlinePlatform",
        "ComDebugApp": "AppComDebug"
    }
    
    if name in special_mappings:
        return special_mappings[name]
    
    return name

def load_design_layers():
    """加载设计的分层结构"""
    design_layers = {
        0: ["BaseCommon", "BaseConfig"],
        1: ["SvcLog", "SvcAuthority", "SvcInteractive", "SvcImageAlgorithm", 
            "SvcDataPostProcess", "SvcSave", "SvcFrame", "SvcCommunicate", 
            "SvcError", "SvcOTA", "SvcSecurity"],
        2: ["HwRobot", "HwSensor", "HwLowerPC"],
        3: ["CoreCupDet", "CoreDashboard", "CoreOnlinePlatform", "CoreV6", "CoreLog"],
        4: ["IfCameraControl", "IfCameraRecognize", "IfCupDet", 
            "IfOnlinePlatform", "IfV6", "IfLog", "IfPackFunc"],
        5: ["UIAnalysisRecord", "UIBaseApp", "UICameraRecognize", "UIChannel", 
            "UICurve", "UISharedFile", "UIReagent", "UIScanner", "UISearch", 
            "UIUart", "UILogin", "UIMethod", "UIUserAudit", "UITask", "UIFlow", 
            "UIPrint", "UIEBalance", "UIExceptionHandle", "UIPluginInterface", "UILog"],
        6: ["AppDashboard", "AppOnlinePlatform", "AppComDebug"]
    }
    
    # 创建模块到层的映射
    module_to_layer = {}
    for layer, modules in design_layers.items():
        for module in modules:
            module_to_layer[module] = layer
    
    return design_layers, module_to_layer

def analyze_cross_layer_dependencies(validation_file):
    """分析跨多层依赖"""
    # 加载设计的分层结构
    design_layers, module_to_layer = load_design_layers()
    
    # 加载实际的依赖关系
    with open(validation_file, 'r', encoding='utf-8') as f:
        validation_data = json.load(f)
    
    modules = validation_data.get("modules", {})
    
    # 统计信息
    cross_layer_dependencies = []
    cross_layer_by_module = defaultdict(list)
    
    # 分析每个模块
    for module_name, module_info in modules.items():
        # 跳过运行时和静态库变体
        if module_name.endswith("Run") or module_name.endswith("Static"):
            continue
            
        # 标准化模块名称
        normalized_name = normalize_module_name(module_name)
        
        # 检查模块是否在设计分层中
        if normalized_name in module_to_layer:
            actual_layer = module_to_layer[normalized_name]
            
            # 分析依赖关系
            dependencies = module_info.get("dependencies", [])
            for dep in dependencies:
                # 跳过第三方库和系统库
                if dep.startswith("OpenSSL") or dep.startswith("ALSA") or dep.startswith("Qt") or dep.startswith("SQLite") or dep.startswith("CURL") or dep.startswith("X") or dep.startswith("cv") or dep.startswith("avm") or dep.startswith("log") or dep.startswith("perception") or dep.startswith("libcef") or dep.startswith("advapi32") or dep.startswith("user32") or dep.startswith("iphlpapi") or dep.startswith("ws2_32") or dep.startswith("kernel32") or dep == "#":
                    continue
                
                # 标准化依赖名称
                normalized_dep = normalize_module_name(dep)
                
                # 检查依赖是否在设计分层中
                if normalized_dep in module_to_layer:
                    dep_layer = module_to_layer[normalized_dep]
                    
                    # 检查是否是跨多层依赖（跨越超过一层）
                    if actual_layer > dep_layer + 1:
                        # 跨多层依赖，可能需要通过接口层
                        cross_layer_dependencies.append((normalized_name, actual_layer, normalized_dep, dep_layer))
                        cross_layer_by_module[normalized_name].append((normalized_dep, dep_layer))
    
    # 按模块分组显示跨多层依赖
    print("="*80)
    print("跨多层依赖详细分析")
    print("="*80)
    print(f"总跨多层依赖数: {len(cross_layer_dependencies)}")
    print()
    
    # 按模块分组显示
    for module, deps in sorted(cross_layer_by_module.items()):
        module_layer = module_to_layer.get(module, "未知")
        print(f"模块: {module} (第{module_layer}层)")
        print("-" * 40)
        for dep, dep_layer in sorted(deps, key=lambda x: x[1], reverse=True):
            layer_diff = module_layer - dep_layer
            print(f"  依赖: {dep} (第{dep_layer}层) - 跨越{layer_diff}层")
        print()
    
    # 按跨越层级数量排序显示最严重的跨多层依赖
    print("="*80)
    print("跨越层级最多的依赖 (前20个)")
    print("="*80)
    
    sorted_deps = sorted(cross_layer_dependencies, key=lambda x: x[1] - x[3], reverse=True)
    for module, module_layer, dep, dep_layer in sorted_deps[:20]:
        layer_diff = module_layer - dep_layer
        print(f"第{module_layer}层 {module} 依赖 第{dep_layer}层 {dep} - 跨越{layer_diff}层")
    
    # 统计每层的跨多层依赖数量
    print("="*80)
    print("各层跨多层依赖统计")
    print("="*80)
    
    layer_cross_stats = defaultdict(int)
    for module, module_layer, dep, dep_layer in cross_layer_dependencies:
        layer_cross_stats[module_layer] += 1
    
    for layer in sorted(layer_cross_stats.keys()):
        print(f"第{layer}层: {layer_cross_stats[layer]}个跨多层依赖")
    
    # 分析和建议
    print("="*80)
    print("分析和建议")
    print("="*80)
    
    # 找出跨多层依赖最多的模块
    module_cross_count = defaultdict(int)
    for module, module_layer, dep, dep_layer in cross_layer_dependencies:
        module_cross_count[module] += 1
    
    top_modules = sorted(module_cross_count.items(), key=lambda x: x[1], reverse=True)[:5]
    print("跨多层依赖最多的模块:")
    for module, count in top_modules:
        module_layer = module_to_layer.get(module, "未知")
        print(f"  {module} (第{module_layer}层): {count}个跨多层依赖")
    
    print("\n建议:")
    print("1. 对于跨越多层的依赖，考虑引入中间接口层")
    print("2. 将频繁跨多层依赖的低层模块提升到更高的层级")
    print("3. 将频繁跨多层依赖的高层模块拆分为更小的模块")
    print("4. 考虑使用依赖注入或事件总线模式减少直接依赖")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python analyze_cross_layer.py <validation_result.json>")
        sys.exit(1)
    
    analyze_cross_layer_dependencies(sys.argv[1])