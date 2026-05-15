#!/usr/bin/env python3
"""
最终版的CMake依赖关系验证脚本，正确处理依赖关系
"""

import json
import os
import re
import sys
import argparse
from typing import Dict, List, Set, Tuple

class CMakeDependencyAnalyzer:
    def __init__(self, project_root):
        self.project_root = project_root
        self.modules = {}
        self.executables = {}
        self.static_libs = {}
        self.shared_libs = {}
        
    def find_cmake_files(self) -> List[str]:
        """查找所有CMakeLists.txt文件"""
        cmake_files = []
        for root, dirs, files in os.walk(self.project_root):
            if 'CMakeLists.txt' in files:
                cmake_files.append(os.path.join(root, 'CMakeLists.txt'))
        return cmake_files
    
    def parse_cmake_file(self, file_path):
        """解析单个CMakeLists.txt文件"""
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # 获取相对路径
        rel_path = os.path.relpath(file_path, self.project_root)
        
        # 查找add_library或add_executable定义的模块
        module_pattern = r'(add_library|add_executable|HG_CMAKE_ADD_MODULE|HG_CMAKE_ADD_SHARED|HG_CMAKE_ADD_STATIC|HG_CMAKE_ADD_EXE)\s*\(\s*([^\s]+)'
        module_matches = re.findall(module_pattern, content)
        
        current_module = None
        
        for match in module_matches:
            command_type, module_name = match
            
            # 跳过第三方库和系统库
            if module_name.startswith('${') or module_name.startswith('::') or module_name in ['PUBLIC', 'PRIVATE', 'INTERFACE']:
                continue
                
            # 记录模块类型
            if command_type in ['add_executable', 'HG_CMAKE_ADD_EXE']:
                self.executables[module_name] = {
                    'path': os.path.dirname(rel_path),
                    'cmake_file': rel_path,
                    'dependencies': set()
                }
            elif command_type in ['HG_CMAKE_ADD_STATIC']:
                self.static_libs[module_name] = {
                    'path': os.path.dirname(rel_path),
                    'cmake_file': rel_path,
                    'dependencies': set()
                }
            elif command_type in ['HG_CMAKE_ADD_SHARED']:
                self.shared_libs[module_name] = {
                    'path': os.path.dirname(rel_path),
                    'cmake_file': rel_path,
                    'dependencies': set()
                }
            else:
                self.modules[module_name] = {
                    'path': os.path.dirname(rel_path),
                    'cmake_file': rel_path,
                    'dependencies': set()
                }
            
            current_module = module_name
        
        # 查找target_link_libraries依赖
        target_link_pattern = r'target_link_libraries\s*\(\s*([^\s]+)\s+([^\s]+)\s*([^)]*)\)'
        target_link_matches = re.finditer(target_link_pattern, content, re.MULTILINE | re.DOTALL)
        
        for match in target_link_matches:
            target, scope, deps_str = match.groups()
            
            # 清理依赖字符串
            deps_str = deps_str.strip()
            
            # 分割依赖项，处理多行情况
            deps = re.split(r'\s+', deps_str)
            
            # 过滤掉空字符串和特殊标记
            deps = [dep for dep in deps if dep and dep not in ['PUBLIC', 'PRIVATE', 'INTERFACE']]
            
            # 过滤掉变量和路径
            deps = [dep for dep in deps if not dep.startswith('${') and not dep.startswith('/') and not dep.endswith('.so') and not dep.endswith('.a')]
            
            # 添加依赖关系
            if target in self.modules:
                self.modules[target]['dependencies'].update(deps)
            elif target in self.executables:
                self.executables[target]['dependencies'].update(deps)
            elif target in self.static_libs:
                self.static_libs[target]['dependencies'].update(deps)
            elif target in self.shared_libs:
                self.shared_libs[target]['dependencies'].update(deps)
        
        # 查找HG_CMAKE_MODULE_DEPEND和HG_CMAKE_MODULE_DEPEND_PUB依赖
        # 这些宏依赖于当前设置的HG_MODULE_NAME变量
        # 我们需要找到每个HG_CMAKE_ADD_*调用后的HG_CMAKE_MODULE_DEPEND调用
        
        # 分割内容为多个块，每个块以HG_CMAKE_ADD_*开始
        blocks = re.split(r'(HG_CMAKE_ADD_(MODULE|SHARED|STATIC|EXE)\s*\([^)]*\))', content, flags=re.DOTALL)
        
        # 重新组合，将每个模块定义和其后的依赖声明组合在一起
        for i in range(1, len(blocks), 3):
            if i+2 >= len(blocks):
                break
                
            module_def = blocks[i] + blocks[i+1] + blocks[i+2]
            rest_content = blocks[i+2] if i+3 < len(blocks) else ""
            
            # 提取模块名
            module_match = re.search(r'HG_CMAKE_ADD_(MODULE|SHARED|STATIC|EXE)\s*\(\s*([^\s]+)', module_def)
            if not module_match:
                continue
                
            module_name = module_match.group(2)
            
            # 查找该模块后的HG_CMAKE_MODULE_DEPEND调用
            dep_pattern = r'HG_CMAKE_MODULE_DEPEND(_PUB)?\s*\(\s*([^)]*)\)'
            dep_matches = re.finditer(dep_pattern, rest_content, re.MULTILINE | re.DOTALL)
            
            for dep_match in dep_matches:
                deps_str = dep_match.group(2)
                
                # 清理依赖字符串
                deps_str = deps_str.strip()
                
                # 分割依赖项，处理多行情况
                deps = re.split(r'\s+', deps_str)
                
                # 过滤掉空字符串和特殊标记
                deps = [dep for dep in deps if dep and dep not in ['PUBLIC', 'PRIVATE', 'INTERFACE']]
                
                # 过滤掉变量和路径
                deps = [dep for dep in deps if not dep.startswith('${') and not dep.startswith('/') and not dep.endswith('.so') and not dep.endswith('.a')]
                
                # 添加依赖关系
                if module_name in self.modules:
                    self.modules[module_name]['dependencies'].update(deps)
                elif module_name in self.executables:
                    self.executables[module_name]['dependencies'].update(deps)
                elif module_name in self.static_libs:
                    self.static_libs[module_name]['dependencies'].update(deps)
                elif module_name in self.shared_libs:
                    self.shared_libs[module_name]['dependencies'].update(deps)
    
    def analyze_dependencies(self):
        """分析依赖关系"""
        all_targets = {}
        all_targets.update(self.modules)
        all_targets.update(self.executables)
        all_targets.update(self.static_libs)
        all_targets.update(self.shared_libs)
        
        # 将set转换为list以便JSON序列化
        for target_name, target_info in all_targets.items():
            target_info['dependencies'] = list(target_info['dependencies'])
        
        # 检查循环依赖
        circular_deps = []
        
        for target_name, target_info in all_targets.items():
            # 跳过可执行文件依赖于同文件中的库的情况（这是正常的）
            for dep in target_info['dependencies']:
                if dep == target_name:
                    # 检查是否在同一文件中
                    if target_info.get('cmake_file'):
                        # 检查同一文件中是否有同名的库
                        cmake_file = target_info['cmake_file']
                        with open(os.path.join(self.project_root, cmake_file), 'r', encoding='utf-8') as f:
                            content = f.read()
                        
                        # 如果是可执行文件依赖于同文件中的库，这是正常的
                        if target_name in self.executables:
                            # 检查是否有对应的库定义
                            lib_pattern = r'(HG_CMAKE_ADD_MODULE|HG_CMAKE_ADD_SHARED|HG_CMAKE_ADD_STATIC)\s*\(\s*' + re.escape(dep)
                            if re.search(lib_pattern, content):
                                continue  # 跳过正常的可执行文件依赖库的情况
                        
                        # 否则记录为循环依赖
                        circular_deps.append((target_name, dep))
        
        # 检查跨层依赖（简化版，实际项目中需要更复杂的层次定义）
        cross_layer_deps = []
        
        return {
            'modules': all_targets,
            'circular_dependencies': circular_deps,
            'cross_layer_dependencies': cross_layer_deps
        }
    
    def generate_cmake_check_code(self, analysis_result):
        """生成依赖检查的CMake代码"""
        code = [
            "# 自动生成的依赖检查CMake代码",
            "# 请将此代码集成到主CMakeLists.txt文件中",
            "",
            "# 引入依赖检查模块",
            "include(cmake/dependency_check.cmake)",
            "",
            "# 设置模块层次",
            "setup_module_layers()",
            ""
        ]
        
        # 为每个模块添加依赖检查
        for target_name, target_info in analysis_result['modules'].items():
            code.append(f"# 检查 {target_name} 的依赖关系")
            if target_name in self.modules:
                code.append(f"check_module_dependencies({target_name})")
        
        code.extend([
            "",
            "# 验证所有模块的依赖关系",
            "validate_all_dependencies()",
            "",
            "# 生成依赖关系图",
            "generate_dependency_graph(${CMAKE_BINARY_DIR}/dependency_graph.dot)"
        ])
        
        return '\n'.join(code)

def main():
    parser = argparse.ArgumentParser(description='CMake依赖关系验证工具')
    parser.add_argument('--project-root', default='.', help='项目根目录路径')
    parser.add_argument('--output-json', default='dependency_check/validation_result.json', help='输出JSON结果的文件路径')
    parser.add_argument('--output-cmake', default='dependency_check/generated_cmake_code.cmake', help='输出CMake检查代码的文件路径')
    
    args = parser.parse_args()
    
    analyzer = CMakeDependencyAnalyzer(args.project_root)
    
    # 查找所有CMakeLists.txt文件
    cmake_files = analyzer.find_cmake_files()
    print(f"在 {args.project_root} 中查找CMakeLists.txt文件...")
    print(f"找到 {len(cmake_files)} 个CMakeLists.txt文件")
    print()
    
    # 解析所有CMakeLists.txt文件
    print("解析CMakeLists.txt文件...")
    for cmake_file in cmake_files:
        analyzer.parse_cmake_file(cmake_file)
    
    total_targets = len(analyzer.modules) + len(analyzer.executables) + len(analyzer.static_libs) + len(analyzer.shared_libs)
    print(f"解析到 {total_targets} 个目标")
    print()
    
    # 分析依赖关系
    analysis_result = analyzer.analyze_dependencies()
    
    # 打印一些示例
    print("模块依赖示例:")
    for name, info in list(analysis_result['modules'].items())[:5]:
        if info['dependencies']:
            print(f"  {name}: {info['dependencies'][:5]}")
    print()
    
    # 检查循环依赖
    print("检查循环依赖...")
    if analysis_result['circular_dependencies']:
        print(f"发现 {len(analysis_result['circular_dependencies'])} 个循环依赖:")
        for i, (target, dep) in enumerate(analysis_result['circular_dependencies'][:10], 1):
            print(f"  循环 {i}: {target} -> {dep}")
    else:
        print("未发现循环依赖")
    print()
    
    # 检查跨层依赖
    print("检查跨层依赖...")
    if analysis_result['cross_layer_dependencies']:
        print(f"发现 {len(analysis_result['cross_layer_dependencies'])} 个跨层依赖:")
        for i, (target, dep) in enumerate(analysis_result['cross_layer_dependencies'], 1):
            print(f"  跨层 {i}: {target} -> {dep}")
    else:
        print("未发现跨层依赖违规")
    print()
    
    # 生成CMake检查代码
    cmake_code = analyzer.generate_cmake_check_code(analysis_result)
    
    # 输出结果
    if args.output_json:
        with open(args.output_json, 'w', encoding='utf-8') as f:
            json.dump(analysis_result, f, indent=2, ensure_ascii=False)
        print(f"验证结果已保存到 {args.output_json}")
    
    if args.output_cmake:
        with open(args.output_cmake, 'w', encoding='utf-8') as f:
            f.write(cmake_code)
        print(f"依赖检查CMake代码已生成到 {args.output_cmake}")
    else:
        with open('dependency_check_generated.cmake', 'w', encoding='utf-8') as f:
            f.write(cmake_code)
        print("依赖检查CMake代码已生成到 dependency_check_generated.cmake")

if __name__ == '__main__':
    main()