#!/usr/bin/env python3
"""
HGAppModule 编译服务器
提供网页界面和编译API
"""

import os
import sys
import json
import subprocess
import threading
import time
from datetime import datetime
from flask import Flask, render_template, request, jsonify, Response, send_file
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# 编译状态字典
build_status = {
    'is_building': False,
    'current_module': None,
    'build_mode': None,
    'progress': 0,
    'log': [],
    'output_files': [],
    'start_time': None,
    'end_time': None,
    'success': False
}

# 模块列表
MODULES = [
    "HGAnalysisRecordModule",
    "HGBaseAppModule",
    "HGCameraRecognizeModule",
    "HGChannelModule",
    "HGCurveModule",
    "HGEBalanceModule",
    "HGExceptionHandleModule",
    "HGFlowModule",
    "HGLogModule",
    "HGLoginModule",
    "HGMethodModule",
    "HGPrintModule",
    "HGReagentModule",
    "HGScannerModule",
    "HGSearchModule",
    "HGSharedFileModule",
    "HGTaskModule",
    "HGUartModule",
    "HGUserAuditModule",
    "TestPlugin"
]

@app.route('/')
def index():
    """显示编译工具网页"""
    return render_template('build_web.html')

@app.route('/api/modules', methods=['GET'])
def get_modules():
    """获取模块列表"""
    return jsonify({
        'modules': MODULES,
        'total': len(MODULES)
    })

@app.route('/api/build/status', methods=['GET'])
def get_build_status():
    """获取编译状态"""
    return jsonify(build_status)

@app.route('/api/build/log', methods=['GET'])
def get_build_log():
    """获取编译日志（流式传输）"""
    def generate_log():
        last_log_length = 0
        try:
            while True:
                current_length = len(build_status['log'])
                if current_length > last_log_length:
                    new_logs = build_status['log'][last_log_length:current_length]
                    for log in new_logs:
                        yield f"data: {json.dumps(log)}\n\n"
                    last_log_length = current_length
                time.sleep(0.1)
        except GeneratorExit:
            # 客户端断开连接
            pass
        except Exception as e:
            print(f"日志生成器错误: {e}")
            yield f"data: {json.dumps({'message': f'日志传输错误: {str(e)}', 'type': 'error'})}\n\n"
    
    response = Response(generate_log(), mimetype='text/event-stream')
    response.headers['Cache-Control'] = 'no-cache'
    response.headers['Connection'] = 'keep-alive'
    response.headers['Access-Control-Allow-Origin'] = '*'
    response.headers['Access-Control-Allow-Headers'] = 'Cache-Control'
    response.headers['X-Accel-Buffering'] = 'no'  # 禁用代理缓冲
    return response

@app.route('/api/build/start', methods=['POST'])
def start_build():
    """开始编译"""
    global build_status
    
    if build_status['is_building']:
        return jsonify({
            'success': False,
            'message': '正在编译中，请稍候...'
        })
    
    data = request.get_json()
    module_name = data.get('module')
    build_mode = data.get('build_mode', 'Release')
    clean_build = data.get('clean_build', False)
    
    if not module_name or module_name not in MODULES:
        return jsonify({
            'success': False,
            'message': '无效的模块名称'
        })
    
    # 重置编译状态
    build_status = {
        'is_building': True,
        'current_module': module_name,
        'build_mode': build_mode,
        'progress': 0,
        'log': [],
        'output_files': [],
        'start_time': datetime.now().isoformat(),
        'end_time': None,
        'success': False
    }
    
    # 在后台线程中执行编译
    thread = threading.Thread(target=run_build, args=(module_name, build_mode, clean_build))
    thread.start()
    
    return jsonify({
        'success': True,
        'message': f'开始编译模块 {module_name} ({build_mode})'
    })

@app.route('/api/build/stop', methods=['POST'])
def stop_build():
    """停止编译"""
    global build_status
    
    if not build_status['is_building']:
        return jsonify({
            'success': False,
            'message': '没有正在进行的编译任务'
        })
    
    # 这里可以添加停止编译的逻辑
    build_status['is_building'] = False
    build_status['end_time'] = datetime.now().isoformat()
    build_status['success'] = False
    build_status['log'].append({
        'timestamp': datetime.now().isoformat(),
        'message': '编译被用户停止',
        'type': 'error'
    })
    
    return jsonify({
        'success': True,
        'message': '编译已停止'
    })

@app.route('/api/package', methods=['POST'])
def package_module():
    """打包模块"""
    data = request.get_json()
    module_name = data.get('module')
    
    if not module_name or module_name not in MODULES:
        return jsonify({
            'success': False,
            'message': '无效的模块名称'
        })
    
    # 检查是否有编译结果
    build_dir = os.path.join('smart_build', module_name)
    if not os.path.exists(build_dir):
        return jsonify({
            'success': False,
            'message': '该模块尚未编译，请先编译模块'
        })
    
    # 执行打包逻辑
    try:
        package_file = create_package(module_name)
        return jsonify({
            'success': True,
            'message': '打包成功',
            'package_file': package_file
        })
    except Exception as e:
        return jsonify({
            'success': False,
            'message': f'打包失败: {str(e)}'
        })

def run_build(module_name, build_mode, clean_build):
    """执行编译任务"""
    global build_status
    
    try:
        add_log(f'开始编译模块: {module_name}', 'info')
        add_log(f'编译模式: {build_mode}', 'info')
        add_log(f'清理构建: {clean_build}', 'info')
        add_log('=' * 50, 'info')
        
        update_progress(10)
        
        # 构建编译命令
        build_args = f"--module {module_name} --{build_mode.lower()}"
        if clean_build:
            build_args += " --clean"
        
        # 添加编译目标
        build_args += " --exe --static --shared"
        
        # 使用windows_build.bat进行编译
        cmd = f"windows_build.bat {build_args}"
        add_log(f'执行命令: {cmd}', 'info')
        
        update_progress(20)
        
        # 执行编译命令
        process = subprocess.Popen(
            cmd,
            shell=True,
            cwd=os.getcwd(),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,  # 将stderr合并到stdout
            text=True,
            encoding='gbk',  # 使用gbk编码支持中文
            bufsize=1,  # 行缓冲
            universal_newlines=True
        )
        
        update_progress(30)
        
        # 实时读取输出
        if process.stdout:
            for line in iter(process.stdout.readline, ''):
                if not build_status['is_building']:
                    break
                line = line.strip()
                if line:
                    add_log(line, 'info')
                    
                    # 根据输出内容更新进度
                    if '正在配置 CMake' in line or 'CMake 配置' in line:
                        update_progress(40)
                    elif '开始编译' in line or '正在编译' in line:
                        update_progress(50)
                    elif '编译完成' in line or '编译成功' in line:
                        update_progress(90)
                    elif 'Linking CXX executable' in line:
                        update_progress(80)
        
        # 等待命令执行完成
        return_code = process.wait()
        
        if return_code == 0:
            update_progress(100)
            build_status['success'] = True
            build_status['log'].append({
                'timestamp': datetime.now().isoformat(),
                'message': f'✅ 模块 {module_name} 编译成功！',
                'type': 'success'
            })
            
            # 查找生成的文件
            find_output_files(module_name)
        else:
            build_status['success'] = False
            build_status['log'].append({
                'timestamp': datetime.now().isoformat(),
                'message': f'❌ 模块 {module_name} 编译失败 (退出码: {return_code})',
                'type': 'error'
            })
            
    except Exception as e:
        build_status['success'] = False
        build_status['log'].append({
            'timestamp': datetime.now().isoformat(),
            'message': f'❌ 编译过程中发生错误: {str(e)}',
            'type': 'error'
        })
    
    finally:
        build_status['is_building'] = False
        build_status['end_time'] = datetime.now().isoformat()
        update_progress(100)

def add_log(message, log_type='info'):
    """添加日志"""
    global build_status
    build_status['log'].append({
        'timestamp': datetime.now().isoformat(),
        'message': message,
        'type': log_type
    })

def update_progress(percent):
    """更新进度"""
    global build_status
    build_status['progress'] = min(100, max(0, percent))

def find_output_files(module_name):
    """查找生成的输出文件"""
    global build_status
    
    build_dir = os.path.join('smart_build', module_name)
    output_files = []
    
    if os.path.exists(build_dir):
        # 查找exe、a、dll文件
        for root, dirs, files in os.walk(build_dir):
            for file in files:
                if file.endswith(('.exe', '.a', '.dll')):
                    file_path = os.path.relpath(os.path.join(root, file), os.getcwd())
                    output_files.append(file_path)
    
    build_status['output_files'] = output_files
    
    if output_files:
        add_log(f'生成的文件 ({len(output_files)} 个):', 'success')
        for file in output_files:
            add_log(f'  - {file}', 'success')

def create_package(module_name):
    """创建模块分发包"""
    import zipfile
    
    build_dir = os.path.join('smart_build', module_name)
    package_name = f"{module_name}_package.zip"
    
    if not os.path.exists(build_dir):
        raise Exception(f'模块构建目录不存在: {build_dir}')
    
    # 创建zip文件
    with zipfile.ZipFile(package_name, 'w', zipfile.ZIP_DEFLATED) as zipf:
        # 添加模块文件
        for root, dirs, files in os.walk(build_dir):
            for file in files:
                file_path = os.path.join(root, file)
                arcname = os.path.relpath(file_path, build_dir)
                zipf.write(file_path, arcname)
        
        # 添加必要的依赖文件
        # 这里可以根据需要添加Qt运行时库、配置文件等
        
    add_log(f'模块 {module_name} 打包成功: {package_name}', 'success')
    
    return package_name

if __name__ == '__main__':
    # 确保在HGAppModule目录下运行
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    print('HGAppModule 编译服务器启动...')
    print(f'模块列表: {MODULES}')
    print('服务器地址: http://localhost:5000')
    print('按 Ctrl+C 停止服务器')
    
    app.run(debug=True, host='0.0.0.0', port=5000, threaded=True)