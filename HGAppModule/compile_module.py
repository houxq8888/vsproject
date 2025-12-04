#!/usr/bin/env python3
"""
HGAppModule 模块编译脚本
支持单个或多个模块的编译，支持Release/Debug模式
"""

import os
import sys
import argparse
import subprocess
import time
from datetime import datetime

def run_command(cmd, cwd=None):
    """执行命令并返回输出"""
    try:
        result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, shell=True)
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def compile_module(module_name, build_type, clean_build=False):
    """编译指定模块"""
    print(f"开始编译模块: {module_name} ({build_type})")
    
    # 构建编译命令
    if build_type.lower() == 'debug':
        build_args = "--debug"
    else:
        build_args = "--release"
    
    if clean_build:
        build_args += " --clean"
    
    # 使用现有的编译脚本
    if os.path.exists("smart_build.bat"):
        cmd = f"smart_build.bat --module {module_name} {build_args} --exe --static"
    elif os.path.exists("windows_build.bat"):
        cmd = f"windows_build.bat --module {module_name} {build_args} --exe --static"
    else:
        print("错误: 未找到编译脚本 (smart_build.bat 或 windows_build.bat)")
        return False
    
    print(f"执行命令: {cmd}")
    
    # 执行编译
    returncode, stdout, stderr = run_command(cmd)
    
    # 输出编译结果
    if stdout:
        print("编译输出:")
        print(stdout)
    
    if stderr:
        print("编译错误:")
        print(stderr)
    
    if returncode == 0:
        print(f"✅ 模块 {module_name} 编译成功")
        
        # 检查生成的文件
        check_generated_files(module_name)
        return True
    else:
        print(f"❌ 模块 {module_name} 编译失败 (退出码: {returncode})")
        return False

def check_generated_files(module_name):
    """检查编译生成的文件"""
    print("\n检查生成的文件:")
    
    # 检查构建目录
    build_dirs = ["windows_build", "smart_build"]
    for build_dir in build_dirs:
        if os.path.exists(build_dir):
            print(f"检查构建目录: {build_dir}")
            
            # 检查静态库
            static_lib = os.path.join(build_dir, f"{module_name}Static.a")
            if os.path.exists(static_lib):
                print(f"✅ 静态库: {static_lib}")
            
            # 检查可执行文件
            exe_file = os.path.join(build_dir, f"{module_name}Run.exe")
            if os.path.exists(exe_file):
                print(f"✅ 可执行文件: {exe_file}")
            
            # 检查动态库
            dll_file = os.path.join(build_dir, f"{module_name}.dll")
            if os.path.exists(dll_file):
                print(f"✅ 动态库: {dll_file}")

def main():
    """主函数"""
    parser = argparse.ArgumentParser(description='HGAppModule 模块编译脚本')
    parser.add_argument('--module', required=True, help='要编译的模块名称')
    parser.add_argument('--build-type', choices=['Release', 'Debug'], default='Release', 
                        help='编译类型 (Release/Debug)')
    parser.add_argument('--clean', action='store_true', help='清理构建目录')
    
    args = parser.parse_args()
    
    # 验证模块是否存在
    module_path = args.module
    if not os.path.exists(module_path):
        print(f"错误: 模块目录 '{module_path}' 不存在")
        sys.exit(1)
    
    # 检查CMakeLists.txt是否存在
    cmake_file = os.path.join(module_path, "CMakeLists.txt")
    if not os.path.exists(cmake_file):
        print(f"警告: 模块 '{module_path}' 没有CMakeLists.txt文件")
    
    print(f"HGAppModule 模块编译")
    print(f"模块: {args.module}")
    print(f"编译类型: {args.build_type}")
    print(f"清理构建: {args.clean}")
    print("-" * 50)
    
    # 开始编译
    start_time = datetime.now()
    success = compile_module(args.module, args.build_type, args.clean)
    end_time = datetime.now()
    
    # 输出编译时间
    compile_time = end_time - start_time
    print(f"\n编译耗时: {compile_time}")
    
    if success:
        print("🎉 编译完成!")
        sys.exit(0)
    else:
        print("💥 编译失败!")
        sys.exit(1)

if __name__ == "__main__":
    main()