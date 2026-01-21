#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
跨多层依赖分析脚本
分析跨多层依赖的具体模式和频率，为重构计划提供数据支持
"""

import json
import sys
from collections import defaultdict, Counter
import os

def normalize_module_name(module_name):
    """标准化模块名称，移除HG前缀"""
    if module_name.startswith("HG"):
        return module_name[2:]
    return module_name

def load_design_layers():
    """加载设计的分层结构"""
    design_layers = {
        0: ["Common", "Config"],  # 基础设施层
        1: ["SvcFrame", "SvcLog", "SvcSave", "BaseConfig", "BaseCommon"],  # 原子服务层
        2: ["IfData", "IfLog", "IfSave", "IfPackFun", "IfConfig", "IfCommon"],  # 接口层
        3: ["CoreLog", "CoreConfig", "CoreCommon", "CorePackFun", "CoreData", "CoreSave"],  # 模块层
        4: ["UIBaseApp", "UISearch", "UIExceptionHandle", "UIPrint", "UIAnalysisRecord", "UIChannel"],  # UI应用模块层
        5: ["AppDemo", "AppBenchmark"]  # 应用层
    }
    
    module_to_layer = {}
    for layer, modules in design_layers.items():
        for module in modules:
            module_to_layer[module] = layer
    
    return design_layers, module_to_layer

def analyze_cross_layer_dependencies(validation_file):
    """分析跨多层依赖的具体模式和频率"""
    # 加载设计的分层结构
    design_layers, module_to_layer = load_design_layers()
    
    # 加载实际的依赖关系
    with open(validation_file, 'r', encoding='utf-8') as f:
        validation_data = json.load(f)
    
    modules = validation_data.get("modules", {})
    
    # 统计跨多层依赖
    cross_layer_deps = []  # (源模块, 源层级, 目标模块, 目标层级, 跨越层数)
    layer_to_modules = defaultdict(list)
    
    for module_name, module_info in modules.items():
        normalized_name = normalize_module_name(module_name)
        if normalized_name in module_to_layer:
            source_layer = module_to_layer[normalized_name]
            layer_to_modules[source_layer].append(module_name)
            
            dependencies = module_info.get("dependencies", [])
            for dep in dependencies:
                # 跳过第三方库和系统库
                if (dep.startswith("OpenSSL") or dep.startswith("ALSA") or 
                    dep.startswith("Qt") or dep.startswith("SQLite") or 
                    dep.startswith("CURL") or dep.startswith("X") or 
                    dep.startswith("cv") or dep.startswith("avm") or 
                    dep.startswith("log") or dep.startswith("perception") or 
                    dep.startswith("libcef") or dep.startswith("advapi32") or 
                    dep.startswith("user32") or dep.startswith("iphlpapi") or 
                    dep.startswith("ws2_32") or dep.startswith("kernel32") or 
                    dep == "#"):
                    continue
                
                # 标准化依赖名称
                normalized_dep = normalize_module_name(dep)
                
                # 检查依赖是否在设计分层中
                if normalized_dep in module_to_layer:
                    target_layer = module_to_layer[normalized_dep]
                    layer_diff = abs(source_layer - target_layer)
                    
                    # 只关注跨越多层（层数差>1）的依赖
                    if layer_diff > 1:
                        cross_layer_deps.append((module_name, source_layer, dep, target_layer, layer_diff))
    
    # 按跨越层数排序
    cross_layer_deps.sort(key=lambda x: x[4], reverse=True)
    
    # 分析依赖模式
    layer_pairs = Counter()  # (源层级, 目标层级) -> 频率
    modules_by_layer = defaultdict(lambda: defaultdict(int))  # 层级 -> 模块 -> 跨层依赖数量
    
    for source_module, source_layer, target_module, target_layer, layer_diff in cross_layer_deps:
        layer_pairs[(source_layer, target_layer)] += 1
        modules_by_layer[source_layer][source_module] += 1
    
    # 生成分析报告
    report = []
    report.append("# 跨多层依赖分析报告\n")
    report.append(f"总跨多层依赖数: {len(cross_layer_deps)}\n")
    
    # 按跨越层数统计
    report.append("\n## 按跨越层数统计\n")
    layer_diff_count = Counter([dep[4] for dep in cross_layer_deps])
    for diff in sorted(layer_diff_count.keys(), reverse=True):
        count = layer_diff_count[diff]
        report.append(f"- 跨越{diff}层: {count}个依赖")
    
    # 按层级对统计
    report.append("\n## 按层级对统计\n")
    for (source_layer, target_layer), count in sorted(layer_pairs.items(), key=lambda x: x[1], reverse=True):
        report.append(f"- 第{source_layer}层 -> 第{target_layer}层: {count}个依赖")
    
    # 问题最严重的模块
    report.append("\n## 问题最严重的模块\n")
    for layer in sorted(modules_by_layer.keys(), reverse=True):
        report.append(f"\n### 第{layer}层模块\n")
        for module, count in sorted(modules_by_layer[layer].items(), key=lambda x: x[1], reverse=True):
            if count > 0:
                report.append(f"- {module}: {count}个跨多层依赖")
    
    # 具体的跨多层依赖示例
    report.append("\n## 具体的跨多层依赖示例\n")
    for source_module, source_layer, target_module, target_layer, layer_diff in cross_layer_deps[:20]:  # 只显示前20个
        report.append(f"- 第{source_layer}层 {source_module} -> 第{target_layer}层 {target_module} (跨越{layer_diff}层)")
    
    # 保存报告
    report_content = "\n".join(report)
    with open(os.path.join(os.path.dirname(validation_file), "cross_layer_analysis.md"), "w", encoding="utf-8") as f:
        f.write(report_content)
    
    return cross_layer_deps, layer_pairs, modules_by_layer

def identify_common_patterns(cross_layer_deps):
    """识别常见的依赖模式"""
    patterns = []
    
    # 模式1: UI层直接依赖基础设施层
    ui_to_infra = [dep for dep in cross_layer_deps if dep[1] >= 4 and dep[3] <= 1]
    patterns.append(("UI层直接依赖基础设施层", ui_to_infra))
    
    # 模式2: 应用层直接依赖基础设施层
    app_to_infra = [dep for dep in cross_layer_deps if dep[1] == 5 and dep[3] <= 1]
    patterns.append(("应用层直接依赖基础设施层", app_to_infra))
    
    # 模式3: UI层直接依赖接口层
    ui_to_interface = [dep for dep in cross_layer_deps if dep[1] >= 4 and dep[3] == 2]
    patterns.append(("UI层直接依赖接口层", ui_to_interface))
    
    # 模式4: 跨越3层以上的依赖
    long_span = [dep for dep in cross_layer_deps if dep[4] >= 3]
    patterns.append(("跨越3层以上的依赖", long_span))
    
    return patterns

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python cross_layer_analysis.py <validation_result.json>")
        sys.exit(1)
    
    validation_file = sys.argv[1]
    cross_layer_deps, layer_pairs, modules_by_layer = analyze_cross_layer_dependencies(validation_file)
    
    # 识别常见模式
    patterns = identify_common_patterns(cross_layer_deps)
    
    print("\n## 常见依赖模式\n")
    for pattern_name, pattern_deps in patterns:
        print(f"{pattern_name}: {len(pattern_deps)}个依赖")
        
        # 统计每个模式中最常被依赖的模块
        target_counter = Counter([dep[2] for dep in pattern_deps])
        print("  最常被依赖的模块:")
        for target, count in target_counter.most_common(5):
            print(f"    {target}: {count}次")
        print()
    
    print("详细分析报告已保存到 cross_layer_analysis.md")