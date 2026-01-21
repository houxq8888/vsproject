#!/usr/bin/env python3
"""
分析实际代码依赖关系与设计分层的一致性
"""

import json
import sys
from collections import defaultdict

def load_design_layers():
    """加载设计的分层结构"""
    layers = {
        0: {  # 基础设施层
            "BaseCommon", "BaseConfig"
        },
        1: {  # 原子服务层
            "SvcLog", "SvcAuthority", "SvcInteractive", 
            "SvcImageAlgorithm", "SvcDataPostProcess", "SvcSave", 
            "SvcFrame", "SvcCommunicate", "SvcError", 
            "SvcOTA", "SvcSecurity"
        },
        2: {  # 硬件抽象层
            "HwRobot", "HwSensor", "HwLowerPC"
        },
        3: {  # 模块层
            "CoreCupDet", "CoreDashboard", "CoreOnlinePlatform", 
            "CoreV6", "CoreLog"
        },
        4: {  # 接口层
            "IfCameraControl", "IfCameraRecognize", "IfCupDet", 
            "IfOnlinePlatform", "IfV6", "IfLog", 
            "IfPackFunc"
        },
        5: {  # UI应用模块层
            "UIAnalysisRecord", "UIBaseApp", "UICameraRecognize", 
            "UIChannel", "UICurve", "UISharedFile", 
            "UIReagent", "UIScanner", "UISearch", 
            "UIUart", "UILog", "UILogin", "UIMethod", 
            "UIUserAudit", "UITask", "UIFlow", "UIPrint", 
            "UIEBalance", "UIExceptionHandle", "UIPluginInterface"
        },
        6: {  # 应用层
            "AppDashboard", "AppOnlinePlatform", "AppComDebug"
        }
    }
    
    # 创建模块到层的映射
    module_to_layer = {}
    for layer, modules in layers.items():
        for module in modules:
            module_to_layer[module] = layer
    
    return layers, module_to_layer

def normalize_module_name(name):
    """标准化模块名称，处理命名差异"""
    # 移除常见的后缀
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
        "HGCommon": "BaseCommon",
        "HGConfig": "BaseConfig",
        
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
        "HGLogService": "SvcLog",
        "HGAuthorityService": "SvcAuthority",
        "HGInteractiveService": "SvcInteractive",
        "HGImageAlgorithmService": "SvcImageAlgorithm",
        "HGDataPostProcessService": "SvcDataPostProcess",
        "HGSaveService": "SvcSave",
        "HGFrameService": "SvcFrame",
        "HGCommunicateService": "SvcCommunicate",
        "HGErrorService": "SvcError",
        "HGOTAService": "SvcOTA",
        "HGSecurityService": "SvcSecurity",
        
        # 第2层 - 硬件抽象层 (Hw前缀)
        "Robot": "HwRobot",
        "Sensor": "HwSensor",
        "LowerPC": "HwLowerPC",
        "HGRobot": "HwRobot",
        "HGSensor": "HwSensor",
        "HGLowerPC": "HwLowerPC",
        
        # 第3层 - 模块层 (Core前缀)
        "CupDetModule": "CoreCupDet",
        "DashboardModule": "CoreDashboard",
        "OnlinePlatformModule": "CoreOnlinePlatform",
        "V6Module": "CoreV6",
        "LogModule": "CoreLog",  # 第3层不带UI的日志模块
        "MLogModule": "CoreLog",  # 第3层，不带UI
        "HGCupDetModule": "CoreCupDet",
        "HGDashboardModule": "CoreDashboard",
        "HGOnlinePlatformModule": "CoreOnlinePlatform",
        "HGV6Module": "CoreV6",
        
        # 第4层 - 接口层 (If前缀)
        "CameraControlInterface": "IfCameraControl",
        "CameraRecognizeInterface": "IfCameraRecognize",
        "CupDetInterface": "IfCupDet",
        "OnlinePlatformInterface": "IfOnlinePlatform",
        "V6Interface": "IfV6",
        "LogInterface": "IfLog",
        "PackFuncInterface": "IfPackFunc",
        "HGCupDetInterface": "IfCupDet",
        "HGOnlinePlatformInterface": "IfOnlinePlatform",
        "HGV6Interface": "IfV6",
        
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
        "HGLogModule": "UILog",  # 第5层，带UI
        "HGAnalysisRecordModule": "UIAnalysisRecord",
        "HGBaseAppModule": "UIBaseApp",
        "HGCameraRecognizeModule": "UICameraRecognize",
        "HGChannelModule": "UIChannel",
        "HGCurveModule": "UICurve",
        "HGSharedFileModule": "UISharedFile",
        "HGReagentModule": "UIReagent",
        "HGScannerModule": "UIScanner",
        "HGSearchModule": "UISearch",
        "HGUartModule": "UIUart",
        "HGLoginModule": "UILogin",
        "HGMethodModule": "UIMethod",
        "HGUserAuditModule": "UIUserAudit",
        "HGTaskModule": "UITask",
        "HGFlowModule": "UIFlow",
        "HGPrintModule": "UIPrint",
        "HGEBalanceModule": "UIEBalance",
        "HGExceptionHandleModule": "UIExceptionHandle",
        
        # 第6层 - 应用层 (App前缀)
        "DashboardApp": "AppDashboard",
        "OnlinePlatformApp": "AppOnlinePlatform",
        "ComDebugApp": "AppComDebug",
        "HGDashboardApp": "AppDashboard",
        "HGOnlinePlatformApp": "AppOnlinePlatform",
        "HGComDebugApp": "AppComDebug"
    }
    
    if name in special_mappings:
        return special_mappings[name]
    
    # 添加HG前缀（如果没有）
    if not name.startswith("HG") and not name.startswith("OpenSSL") and not name.startswith("ALSA") and not name.startswith("Qt") and not name.startswith("SQLite") and not name.startswith("CURL") and not name.startswith("X") and not name.startswith("cv") and not name.startswith("avm") and not name.startswith("log") and not name.startswith("perception") and not name.startswith("libcef") and not name.startswith("advapi32") and not name.startswith("user32") and not name.startswith("iphlpapi") and not name.startswith("ws2_32") and not name.startswith("kernel32"):
        name = "HG" + name
    
    return name

def analyze_dependencies(validation_file):
    """分析依赖关系与设计分层的一致性"""
    # 加载设计的分层结构
    design_layers, module_to_layer = load_design_layers()
    
    # 加载实际的依赖关系
    with open(validation_file, 'r', encoding='utf-8') as f:
        validation_data = json.load(f)
    
    modules = validation_data.get("modules", {})
    
    # 统计信息
    total_modules = 0
    matched_modules = 0
    unmatched_modules = []
    layer_violations = []
    cross_layer_dependencies = []
    
    # 分析每个模块
    for module_name, module_info in modules.items():
        # 跳过运行时和静态库变体
        if module_name.endswith("Run") or module_name.endswith("Static"):
            continue
            
        total_modules += 1
        
        # 标准化模块名称
        normalized_name = normalize_module_name(module_name)
        
        # 检查模块是否在设计分层中
        if normalized_name in module_to_layer:
            matched_modules += 1
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
                    
                    # 检查是否违反分层规则（高层依赖低层是正常的，低层依赖高层是违规的）
                    if actual_layer < dep_layer:
                        # 低层模块依赖高层模块，违反分层原则
                        layer_violations.append((normalized_name, actual_layer, normalized_dep, dep_layer))
                    elif actual_layer > dep_layer + 1:
                        # 跨多层依赖，可能需要通过接口层
                        cross_layer_dependencies.append((normalized_name, actual_layer, normalized_dep, dep_layer))
        else:
            unmatched_modules.append((module_name, normalized_name))
    
    # 打印分析结果
    print("="*60)
    print("依赖关系与设计分层一致性分析")
    print("="*60)
    print(f"总模块数: {total_modules}")
    print(f"匹配设计的模块数: {matched_modules}")
    print(f"未匹配设计的模块数: {len(unmatched_modules)}")
    print(f"匹配率: {matched_modules/total_modules*100:.2f}%")
    print()
    
    # 打印未匹配的模块
    if unmatched_modules:
        print("未匹配设计的模块:")
        for original, normalized in unmatched_modules[:10]:  # 只显示前10个
            print(f"  {original} -> {normalized}")
        if len(unmatched_modules) > 10:
            print(f"  ... 还有 {len(unmatched_modules)-10} 个")
        print()
    
    # 打印分层违规
    if layer_violations:
        print("分层违规 (低层依赖高层):")
        for module, module_layer, dep, dep_layer in layer_violations[:10]:  # 只显示前10个
            print(f"  第{module_layer}层 {module} 依赖 第{dep_layer}层 {dep}")
        if len(layer_violations) > 10:
            print(f"  ... 还有 {len(layer_violations)-10} 个")
        print()
    
    # 打印跨多层依赖
    if cross_layer_dependencies:
        print("跨多层依赖 (可能需要通过接口层):")
        for module, module_layer, dep, dep_layer in cross_layer_dependencies[:10]:  # 只显示前10个
            print(f"  第{module_layer}层 {module} 依赖 第{dep_layer}层 {dep}")
        if len(cross_layer_dependencies) > 10:
            print(f"  ... 还有 {len(cross_layer_dependencies)-10} 个")
        print()
    
    # 按层统计模块分布（去重）
    print("实际模块按设计分层的分布:")
    layer_counts = defaultdict(int)
    layer_modules = defaultdict(set)
    for module_name, module_info in modules.items():
        if module_name.endswith("Run") or module_name.endswith("Static"):
            continue
            
        normalized_name = normalize_module_name(module_name)
        if normalized_name in module_to_layer:
            layer = module_to_layer[normalized_name]
            layer_counts[layer] += 1
            layer_modules[layer].add(normalized_name)
    
    for layer in range(7):
        print(f"  第{layer}层: {len(layer_modules[layer])} 个模块 (去重后)")
    print()
    
    # 重复模块分析
    print("重复模块分析:")
    module_names = defaultdict(list)
    for module_name, module_info in modules.items():
        if module_name.endswith("Run") or module_name.endswith("Static"):
            continue
            
        normalized_name = normalize_module_name(module_name)
        if normalized_name in module_to_layer:
            module_names[normalized_name].append(module_name)
    
    duplicates = {name: variants for name, variants in module_names.items() if len(variants) > 1}
    if duplicates:
        for name, variants in duplicates.items():
            print(f"  {name}: {', '.join(variants)}")
    else:
        print("  没有发现重复模块")
    print()
    
    # 总结
    print("="*60)
    print("总结:")
    if matched_modules / total_modules > 0.8:
        print("√ 大部分模块符合设计分层")
    else:
        print("× 较多模块不符合设计分层")
    
    if not layer_violations:
        print("√ 没有发现分层违规")
    else:
        print(f"× 发现 {len(layer_violations)} 个分层违规")
    
    if len(cross_layer_dependencies) < 10:
        print("√ 跨多层依赖较少")
    else:
        print(f"× 发现 {len(cross_layer_dependencies)} 个跨多层依赖")
    
    if not duplicates:
        print("√ 没有发现重复模块")
    else:
        print(f"× 发现 {len(duplicates)} 个重复模块")
    
    print("="*60)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("用法: python analyze_layer_compliance.py <validation_result.json>")
        sys.exit(1)
    
    validation_file = sys.argv[1]
    analyze_dependencies(validation_file)