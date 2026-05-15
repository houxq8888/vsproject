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
        "LogModule": "CoreLog",  # 第3层不带UI的日志模块
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
            "UIPrint", "UIEBalance", "UIExceptionHandle", "UIPluginInterface"],
        6: ["AppDashboard", "AppOnlinePlatform", "AppComDebug"]
    }
    
    # 创建模块到层的映射
    module_to_layer = {}
    for layer, modules in design_layers.items():
        for module in modules:
            module_to_layer[module] = layer
    
    return design_layers, module_to_layer

def debug_corelog_dependencies(validation_file):
    """调试CoreLog模块的依赖关系"""
    # 加载设计的分层结构
    design_layers, module_to_layer = load_design_layers()
    
    # 加载实际的依赖关系
    with open(validation_file, 'r', encoding='utf-8') as f:
        validation_data = json.load(f)
    
    modules = validation_data.get("modules", {})
    
    # 查找CoreLog模块
    for module_name, module_info in modules.items():
        normalized_name = normalize_module_name(module_name)
        if normalized_name == "CoreLog":
            print(f"找到模块: {module_name} -> {normalized_name}")
            print(f"模块路径: {module_info.get('path', 'N/A')}")
            print(f"CMake文件: {module_info.get('cmake_file', 'N/A')}")
            
            dependencies = module_info.get("dependencies", [])
            print(f"依赖数量: {len(dependencies)}")
            print("依赖列表:")
            for dep in dependencies:
                # 跳过第三方库和系统库
                if dep.startswith("OpenSSL") or dep.startswith("ALSA") or dep.startswith("Qt") or dep.startswith("SQLite") or dep.startswith("CURL") or dep.startswith("X") or dep.startswith("cv") or dep.startswith("avm") or dep.startswith("log") or dep.startswith("perception") or dep.startswith("libcef") or dep.startswith("advapi32") or dep.startswith("user32") or dep.startswith("iphlpapi") or dep.startswith("ws2_32") or dep.startswith("kernel32") or dep == "#":
                    print(f"  {dep} (第三方库，跳过)")
                    continue
                
                # 标准化依赖名称
                normalized_dep = normalize_module_name(dep)
                
                # 检查依赖是否在设计分层中
                if normalized_dep in module_to_layer:
                    dep_layer = module_to_layer[normalized_dep]
                    print(f"  {dep} -> {normalized_dep} (第{dep_layer}层)")
                else:
                    print(f"  {dep} -> {normalized_dep} (未在设计分层中)")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python debug_corelog.py <validation_result.json>")
        sys.exit(1)
    
    debug_corelog_dependencies(sys.argv[1])