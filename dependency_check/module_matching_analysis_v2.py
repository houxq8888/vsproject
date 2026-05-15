#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
模块匹配分析脚本 v2
基于实际模块命名和分层结构进行更准确的分析
"""

import json
import sys
from collections import defaultdict

def normalize_module_name(module_name):
    """标准化模块名称，移除HG和HXQ前缀，处理测试模块名称"""
    # 移除HG和HXQ前缀
    if module_name.startswith("HG"):
        module_name = module_name[2:]
    elif module_name.startswith("HXQ"):
        module_name = module_name[3:]
    
    # 处理测试模块名称的特殊情况
    if module_name == "TestPlugin":
        return "TestPlugin"
    elif module_name == "CommonCommunicationDemo":
        return "TestCommonCommunicationDemo"
    elif module_name == "CommonCommunicationDemoC":
        return "TestCommonCommunicationDemoC"
    elif module_name == "testCameraInterface":
        return "TestCameraInterface"
    elif module_name == "YModemTest":
        return "TestYModem"
    
    return module_name

def load_actual_design_layers():
    """加载基于实际模块的分层结构，参考 docs/layered_view_benchmark.txt"""
    # 基于实际模块的分层结构，来自 docs/layered_view_benchmark.txt
    design_layers = {
        0: ["BaseCommon", "BaseConfig"],  # 基础设施层 (Base前缀)
        1: ["SvcLog", "SvcAuthority", "SvcInteractive", "SvcImageAlgorithm", 
            "SvcDataPostProcess", "SvcSave", "SvcFrame", "SvcCommunicate", 
            "SvcError", "SvcOTA", "SvcSecurity", "SvcCommonProtocol"],  # 原子服务层 (Svc前缀)
        2: ["HwRobot", "HwSensor", "HwLowerPC"],  # 硬件抽象层 (Hw前缀)
        3: ["CoreCupDet", "CoreDashboard", "CoreOnlinePlatform", "CoreV6", "CoreLog", 
            "CoreDetectDimentionVux"],  # 模块层 (Core前缀)
        4: ["IfCameraControl", "IfCameraRecognize", "IfCupDet", "IfOnlinePlatform", 
            "IfV6", "IfLog", "IfPackFunc", "IfDetectDimentionVux"],  # 接口层 (If前缀)
        5: ["UIAnalysisRecord", "UIBaseApp", "UICameraRecognize", "UIChannel", 
            "UICurve", "UISharedFile", "UIReagent", "UIScanner", "UISearch", 
            "UIUart", "UILog", "UILogin", "UIMethod", "UIUserAudit", 
            "UITask", "UIFlow", "UIPrint", "UIEBalance", "UIExceptionHandle", 
            "UIPluginInterface"],  # UI应用模块层 (UI前缀)
        6: ["AppDashboard", "AppOnlinePlatform", "AppComDebug", "AppCloud", 
            "AppCupDet", "AppSpectrometer", "CmdCommonProtocol", "CmdDetectDimentionVux", 
            "CmdDFP"],  # 应用层 (App前缀和Cmd前缀)
        7: ["TestPlugin", "TestCommonCommunicationDemo", "TestCommonCommunicationDemoC", 
            "TestTCPPushClient", "TestYModem"]  # 测试层 (Test前缀)
    }
    
    module_to_layer = {}
    for layer, modules in design_layers.items():
        for module in modules:
            module_to_layer[module] = layer
    
    return design_layers, module_to_layer

def analyze_module_matching(validation_file):
    """分析模块匹配情况"""
    # 加载设计的分层结构
    design_layers, module_to_layer = load_actual_design_layers()
    
    # 加载实际的依赖关系
    with open(validation_file, 'r', encoding='utf-8') as f:
        validation_data = json.load(f)
    
    modules = validation_data.get("modules", {})
    
    # 分析模块匹配情况
    matched_modules = []
    unmatched_modules = []
    
    for module_name, module_info in modules.items():
        normalized_name = normalize_module_name(module_name)
        if normalized_name in module_to_layer:
            matched_modules.append((module_name, normalized_name, module_to_layer[normalized_name]))
        else:
            unmatched_modules.append((module_name, normalized_name))
    
    # 统计每层的模块数量
    layer_counts = defaultdict(int)
    for _, normalized_name, layer in matched_modules:
        layer_counts[layer] += 1
    
    # 生成分析报告
    report = []
    report.append("# 模块匹配分析报告 v2\n")
    report.append(f"总模块数: {len(modules)}")
    report.append(f"匹配设计的模块数: {len(matched_modules)}")
    report.append(f"未匹配设计的模块数: {len(unmatched_modules)}")
    report.append(f"匹配率: {len(matched_modules)/len(modules)*100:.2f}%\n")
    
    # 按层统计匹配的模块
    report.append("## 按层统计匹配的模块\n")
    for layer in sorted(layer_counts.keys()):
        count = layer_counts[layer]
        layer_modules = [m[0] for m in matched_modules if m[2] == layer]
        report.append(f"### 第{layer}层 ({count}个模块)")
        for module in sorted(layer_modules):
            report.append(f"- {module}")
        report.append("")
    
    # 列出未匹配的模块
    report.append("## 未匹配的模块\n")
    for module_name, normalized_name in unmatched_modules:
        report.append(f"- {module_name} -> {normalized_name}")
    
    # 分析未匹配模块的特征
    report.append("\n## 未匹配模块特征分析\n")
    
    # 检查是否有Static或Run后缀的模块
    static_run_modules = []
    for module_name, normalized_name in unmatched_modules:
        if normalized_name.endswith("Static") or normalized_name.endswith("Run"):
            static_run_modules.append((module_name, normalized_name))
    
    if static_run_modules:
        report.append(f"### Static/Run后缀模块 ({len(static_run_modules)}个)")
        for module_name, normalized_name in static_run_modules:
            base_name = normalized_name[:-6] if normalized_name.endswith("Static") else normalized_name[:-3]
            report.append(f"- {module_name} -> {normalized_name} (基础模块: {base_name})")
    
    # 按前缀分类未匹配模块
    prefix_categories = {
        "Test": [],
        "Demo": [],
        "Plugin": [],
        "Interface": [],
        "App": [],
        "Module": [],
        "Process": [],
        "Other": []
    }
    
    for module_name, normalized_name in unmatched_modules:
        if normalized_name.startswith("Test"):
            prefix_categories["Test"].append((module_name, normalized_name))
        elif normalized_name.startswith("Demo") or "Demo" in normalized_name:
            prefix_categories["Demo"].append((module_name, normalized_name))
        elif normalized_name.startswith("Plugin"):
            prefix_categories["Plugin"].append((module_name, normalized_name))
        elif normalized_name.startswith("Interface") or "Interface" in normalized_name:
            prefix_categories["Interface"].append((module_name, normalized_name))
        elif normalized_name.startswith("App"):
            prefix_categories["App"].append((module_name, normalized_name))
        elif normalized_name.endswith("Module"):
            prefix_categories["Module"].append((module_name, normalized_name))
        elif normalized_name.endswith("Process"):
            prefix_categories["Process"].append((module_name, normalized_name))
        else:
            prefix_categories["Other"].append((module_name, normalized_name))
    
    # 按类别输出未匹配模块
    for category, modules in prefix_categories.items():
        if modules:
            report.append(f"\n### {category}类模块 ({len(modules)}个)")
            for module_name, normalized_name in modules:
                report.append(f"- {module_name} -> {normalized_name}")
    
    # 保存报告
    report_content = "\n".join(report)
    with open("module_matching_analysis_v2.md", "w", encoding="utf-8") as f:
        f.write(report_content)
    
    return matched_modules, unmatched_modules, layer_counts

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python module_matching_analysis_v2.py <validation_result.json>")
        sys.exit(1)
    
    validation_file = sys.argv[1]
    matched, unmatched, layer_counts = analyze_module_matching(validation_file)
    
    print(f"\n匹配率: {len(matched)/(len(matched)+len(unmatched))*100:.2f}%")
    print(f"匹配模块数: {len(matched)}")
    print(f"未匹配模块数: {len(unmatched)}")
    
    print("\n按层统计:")
    for layer in sorted(layer_counts.keys()):
        print(f"第{layer}层: {layer_counts[layer]}个模块")
    
    print("\n详细分析报告已保存到 module_matching_analysis_v2.md")