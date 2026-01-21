#!/usr/bin/env python3
"""
依赖关系分析工具
用于分析vsproject中的模块依赖关系，识别循环依赖
"""

import os
import re
import sys
import json
from collections import defaultdict, deque
import argparse

class DependencyAnalyzer:
    def __init__(self, project_root):
        self.project_root = project_root
        self.dependencies = defaultdict(set)
        self.reverse_dependencies = defaultdict(set)
        self.modules = set()
        self.cycles = []
        
    def find_cmake_files(self):
        """查找所有CMakeLists.txt文件"""
        cmake_files = []
        for root, dirs, files in os.walk(self.project_root):
            if 'CMakeLists.txt' in files:
                cmake_files.append(os.path.join(root, 'CMakeLists.txt'))
        return cmake_files
    
    def extract_module_name(self, cmake_path):
        """从CMakeLists.txt路径提取模块名"""
        # 将路径转换为相对路径
        rel_path = os.path.relpath(cmake_path, self.project_root)
        # 提取模块名，通常是包含CMakeLists.txt的目录名
        module_name = os.path.basename(os.path.dirname(cmake_path))
        return module_name
    
    def parse_cmake_file(self, cmake_path):
        """解析CMakeLists.txt文件，提取依赖关系"""
        module_name = self.extract_module_name(cmake_path)
        self.modules.add(module_name)
        
        with open(cmake_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # 查找HG_CMAKE_MODULE_DEPEND和HG_CMAKE_MODULE_DEPEND_PUB
        depend_pattern = r'HG_CMAKE_MODULE_DEPEND(_PUB)?\s*\(\s*([^)]+)\s*\)'
        matches = re.findall(depend_pattern, content, re.MULTILINE | re.DOTALL)
        
        for _, deps in matches:
            # 清理依赖列表，移除空格、换行等
            deps = re.sub(r'\s+', ' ', deps).strip()
            # 分割依赖项
            dep_list = [dep.strip() for dep in deps.split() if dep.strip()]
            
            for dep in dep_list:
                # 跳过非模块依赖（如${LOG4CPLUS_LIBRARIES}等）
                if dep.startswith('${') or dep.startswith('#'):
                    continue
                
                # 添加依赖关系
                self.dependencies[module_name].add(dep)
                self.reverse_dependencies[dep].add(module_name)
    
    def find_cycles(self):
        """查找循环依赖"""
        visited = set()
        rec_stack = set()
        path = []
        
        def dfs(node):
            visited.add(node)
            rec_stack.add(node)
            path.append(node)
            
            for neighbor in self.dependencies[node]:
                if neighbor not in visited:
                    if dfs(neighbor):
                        return True
                elif neighbor in rec_stack:
                    # 找到循环依赖
                    cycle_start = path.index(neighbor)
                    cycle = path[cycle_start:] + [neighbor]
                    self.cycles.append(cycle)
                    return True
            
            rec_stack.remove(node)
            path.pop()
            return False
        
        for node in self.modules:
            if node not in visited:
                dfs(node)
        
        return self.cycles
    
    def generate_dependency_graph(self):
        """生成依赖关系图（DOT格式）"""
        dot_content = ["digraph Dependencies {"]
        dot_content.append("    rankdir=TB;")
        dot_content.append("    node [shape=box];")
        
        # 添加节点
        for module in self.modules:
            dot_content.append(f'    "{module}";')
        
        # 添加边
        for module, deps in self.dependencies.items():
            for dep in deps:
                if dep in self.modules:  # 只显示模块间的依赖
                    dot_content.append(f'    "{module}" -> "{dep}";')
        
        dot_content.append("}")
        return "\n".join(dot_content)
    
    def generate_layered_view(self):
        """生成分层视图"""
        # 计算每个模块的层级（从没有依赖的模块开始）
        in_degree = {module: 0 for module in self.modules}
        for module in self.modules:
            for dep in self.dependencies[module]:
                if dep in self.modules:
                    in_degree[module] += 1
        
        # 拓扑排序
        layers = defaultdict(list)
        queue = deque([module for module in self.modules if in_degree[module] == 0])
        current_layer = 0
        
        while queue:
            next_queue = deque()
            while queue:
                module = queue.popleft()
                layers[current_layer].append(module)
                
                for dependent in self.reverse_dependencies[module]:
                    if dependent in self.modules:
                        in_degree[dependent] -= 1
                        if in_degree[dependent] == 0:
                            next_queue.append(dependent)
            
            queue = next_queue
            current_layer += 1
        
        return dict(layers)
    
    def analyze(self):
        """执行依赖分析"""
        cmake_files = self.find_cmake_files()
        print(f"找到 {len(cmake_files)} 个CMakeLists.txt文件")
        
        for cmake_file in cmake_files:
            self.parse_cmake_file(cmake_file)
        
        print(f"分析了 {len(self.modules)} 个模块")
        
        # 查找循环依赖
        cycles = self.find_cycles()
        if cycles:
            print(f"\n发现 {len(cycles)} 个循环依赖:")
            for i, cycle in enumerate(cycles, 1):
                print(f"  循环 {i}: {' -> '.join(cycle)}")
        else:
            print("\n未发现循环依赖")
        
        return {
            'modules': list(self.modules),
            'dependencies': {k: list(v) for k, v in self.dependencies.items()},
            'cycles': self.cycles,
            'layered_view': self.generate_layered_view()
        }
    
    def save_results(self, results, output_dir):
        """保存分析结果"""
        os.makedirs(output_dir, exist_ok=True)
        
        # 保存JSON格式的分析结果
        with open(os.path.join(output_dir, 'dependency_analysis.json'), 'w', encoding='utf-8') as f:
            json.dump(results, f, indent=2, ensure_ascii=False)
        
        # 保存DOT格式的依赖图
        with open(os.path.join(output_dir, 'dependency_graph.dot'), 'w', encoding='utf-8') as f:
            f.write(self.generate_dependency_graph())
        
        # 保存分层视图
        with open(os.path.join(output_dir, 'layered_view.txt'), 'w', encoding='utf-8') as f:
            f.write("模块分层视图:\n\n")
            for layer, modules in results['layered_view'].items():
                f.write(f"第 {layer} 层:\n")
                for module in modules:
                    f.write(f"  - {module}\n")
                f.write("\n")
        
        print(f"\n分析结果已保存到 {output_dir}")

def main():
    parser = argparse.ArgumentParser(description='分析vsproject模块依赖关系')
    parser.add_argument('--project-root', default='.', help='项目根目录路径')
    parser.add_argument('--output-dir', default='./dependency_analysis', help='输出目录')
    
    args = parser.parse_args()
    
    analyzer = DependencyAnalyzer(args.project_root)
    results = analyzer.analyze()
    analyzer.save_results(results, args.output_dir)
    
    return 0 if not results['cycles'] else 1

if __name__ == '__main__':
    sys.exit(main())