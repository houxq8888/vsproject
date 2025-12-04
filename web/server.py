#!/usr/bin/env python3
"""
VSProject 架构展示Web服务器
提供项目架构信息展示页面和模块编译功能
"""

import os
import sys
import subprocess
import threading
import json
from datetime import datetime
from flask import Flask, send_from_directory, request, jsonify
from flask_cors import CORS

app = Flask(__name__, static_folder='.', static_url_path='')
CORS(app)  # 允许跨域请求

# 项目根目录
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HGAPP_MODULE_PATH = os.path.join(PROJECT_ROOT, 'HGAppModule')

# 编译状态存储
compile_status = {
    'current_process': None,
    'last_output': '',
    'is_running': False,
    'start_time': None,
    'module': None,
    'build_type': 'Release',
    'clean_build': False
}

@app.route('/')
def index():
    """重定向到架构展示页面"""
    return send_from_directory('.', 'project_architecture.html')

@app.route('/<path:filename>')
def serve_static(filename):
    """提供静态文件服务"""
    return send_from_directory('.', filename)

@app.route('/api/project-info')
def get_project_info():
    """获取项目基本信息"""
    return {
        "name": "VSProject",
        "description": "基于C++/Qt的大型模块化软件项目",
        "technology_stack": ["C++", "Qt", "CMake", "OpenCV", "SQLCipher", "MQTT"],
        "platforms": ["Windows", "Linux"],
        "modules_count": 20,
        "projects_count": 10,
        "architecture_type": "模块化架构"
    }

@app.route('/api/modules')
def get_modules():
    """获取模块列表"""
    return {
        "application_modules": [
            {"name": "HGDashboardApp", "description": "仪表盘应用", "dependencies": ["Qt", "OpenCV"]},
            {"name": "HGComDebugApp", "description": "通信调试应用", "dependencies": ["Qt", "SerialPort"]},
            {"name": "HGCupDetApp", "description": "杯检测应用", "dependencies": ["Qt", "OpenCV"]},
            {"name": "HGSpectrometerApp", "description": "光谱仪应用", "dependencies": ["Qt", "OpenCV"]},
            {"name": "HGOnlinePlatformApp", "description": "在线平台应用", "dependencies": ["Qt", "MQTT"]}
        ],
        "core_modules": [
            {"name": "HGBaseAppModule", "description": "基础应用模块", "dependencies": ["Qt"]},
            {"name": "HGUserAuditModule", "description": "用户审计模块", "dependencies": ["Qt", "SQLCipher"]},
            {"name": "HGLogModule", "description": "日志模块", "dependencies": ["Qt", "log4cplus"]},
            {"name": "HGLoginModule", "description": "登录模块", "dependencies": ["Qt", "SQLCipher"]}
        ],
        "functional_modules": [
            {"name": "HGChannelModule", "description": "通道管理模块", "dependencies": ["Qt", "SerialPort"]},
            {"name": "HGCurveModule", "description": "曲线处理模块", "dependencies": ["Qt", "OpenCV"]},
            {"name": "HGFlowModule", "description": "流程控制模块", "dependencies": ["Qt"]},
            {"name": "HGMethodModule", "description": "方法管理模块", "dependencies": ["Qt"]},
            {"name": "HGPrintModule", "description": "打印模块", "dependencies": ["Qt", "HPDF"]},
            {"name": "HGReagentModule", "description": "试剂管理模块", "dependencies": ["Qt", "SQLCipher"]},
            {"name": "HGScannerModule", "description": "扫描模块", "dependencies": ["Qt", "ZXing"]},
            {"name": "HGSearchModule", "description": "搜索模块", "dependencies": ["Qt"]},
            {"name": "HGTaskModule", "description": "任务管理模块", "dependencies": ["Qt"]},
            {"name": "HGUartModule", "description": "串口通信模块", "dependencies": ["Qt", "SerialPort"]}
        ],
        "service_modules": [
            {"name": "HGCommon", "description": "通用服务", "dependencies": []},
            {"name": "HGConfig", "description": "配置服务", "dependencies": []},
            {"name": "HGHardware", "description": "硬件服务", "dependencies": []}
        ]
    }

@app.route('/api/build-info')
def get_build_info():
    """获取构建信息"""
    return {
        "build_system": "CMake",
        "min_cmake_version": "3.22.0",
        "build_scripts": ["build.bat", "build.sh", "build_release.sh"],
        "platforms": ["x64", "win32", "stm32", "zone"],
        "third_party_libraries": [
            "Qt5", "OpenCV", "SQLCipher", "log4cplus", "ZXing", 
            "CSerialPort", "HPDF", "libssh", "vsomeip3"
        ]
    }

@app.route('/api/compile/status')
def get_compile_status():
    """获取编译状态"""
    return jsonify({
        'is_running': compile_status['is_running'],
        'last_output': compile_status['last_output'],
        'start_time': compile_status['start_time'],
        'module': compile_status['module'],
        'build_type': compile_status['build_type'],
        'clean_build': compile_status['clean_build']
    })

@app.route('/api/compile/start', methods=['POST'])
def start_compile():
    """开始编译模块"""
    if compile_status['is_running']:
        return jsonify({'error': '编译正在进行中，请等待完成'}), 400
    
    data = request.json
    module = data.get('module', 'HGUserAuditModule')
    build_type = data.get('build_type', 'Release')
    clean_build = data.get('clean_build', False)
    
    # 验证模块是否存在
    module_path = os.path.join(HGAPP_MODULE_PATH, module)
    if not os.path.exists(module_path):
        return jsonify({'error': f'模块 {module} 不存在'}), 400
    
    # 启动编译线程
    thread = threading.Thread(target=compile_module, args=(module, build_type, clean_build))
    thread.daemon = True
    thread.start()
    
    return jsonify({'message': f'开始编译模块 {module} ({build_type})', 'module': module})

@app.route('/api/compile/stop', methods=['POST'])
def stop_compile():
    """停止编译"""
    if compile_status['is_running'] and compile_status['current_process']:
        try:
            compile_status['current_process'].terminate()
            compile_status['is_running'] = False
            compile_status['last_output'] += '\n编译已手动停止'
            return jsonify({'message': '编译已停止'})
        except Exception as e:
            return jsonify({'error': f'停止编译失败: {str(e)}'}), 500
    else:
        return jsonify({'error': '没有正在运行的编译进程'}), 400

def compile_module(module, build_type, clean_build):
    """编译模块的线程函数"""
    compile_status['is_running'] = True
    compile_status['module'] = module
    compile_status['build_type'] = build_type
    compile_status['clean_build'] = clean_build
    compile_status['start_time'] = datetime.now().isoformat()
    compile_status['last_output'] = f'开始编译模块: {module} ({build_type})\n'
    
    try:
        # 构建编译命令
        cmd = ['python', 'compile_module.py', '--module', module, '--build-type', build_type]
        if clean_build:
            cmd.append('--clean')
        
        # 执行编译
        process = subprocess.Popen(
            cmd,
            cwd=HGAPP_MODULE_PATH,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
            universal_newlines=True
        )
        
        compile_status['current_process'] = process
        
        # 实时读取输出
        for line in process.stdout:
            compile_status['last_output'] += line
        
        # 等待进程结束
        process.wait()
        
        if process.returncode == 0:
            compile_status['last_output'] += f'\n编译成功: {module}'
        else:
            compile_status['last_output'] += f'\n编译失败: {module} (退出码: {process.returncode})'
            
    except Exception as e:
        compile_status['last_output'] += f'\n编译异常: {str(e)}'
    finally:
        compile_status['is_running'] = False
        compile_status['current_process'] = None

if __name__ == '__main__':
    print("VSProject 架构展示服务器启动中...")
    print("访问地址: http://localhost:8080/")
    print("按 Ctrl+C 停止服务器")
    
    # 启动Flask服务器
    app.run(host='0.0.0.0', port=8080, debug=True)