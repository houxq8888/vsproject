#!/usr/bin/env python3
import os
import re

# 需要处理的文件列表
files_to_process = [
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\Device\DeviceManage\hgdevicemanagewidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\HGOnlineMainWidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\Maintenance\Material\hgmaterialmanagewidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\Maintenance\Device\hgdevicemaintenancewidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\MainPage\RunStatus\hgrunningchartwidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\MainPage\RunStatus\hgrunningwidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\src\Data\ErrCodeInfo\hgerrorcodewidget.cpp",
    r"D:\virtualMachine\github\vsproject\projects\AppOnlinePlatform\main.cpp",
]

def process_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # 检查是否包含 RWDb::writeAuditTrailLog
        if 'RWDb::writeAuditTrailLog' not in content:
            print(f"跳过 {file_path} (不包含 RWDb::writeAuditTrailLog)")
            return
        
        # 替换 RWDb::writeAuditTrailLog 为 LOG_IF.writeAuditTrailLog
        new_content = content.replace('RWDb::writeAuditTrailLog', 'LOG_IF.writeAuditTrailLog')
        
        # 检查是否已经包含了 loginterface.h
        if '#include "loginterface.h"' not in content and '#include <loginterface.h>' not in content:
            # 找到最后一个 #include，在其后添加 loginterface.h
            includes = re.findall(r'^\s*#include\s+[<"][^>"]+[>"]', content, re.MULTILINE)
            if includes:
                last_include = includes[-1]
                last_include_pos = content.rfind(last_include) + len(last_include)
                # 在最后一个 #include 后添加
                new_content = content[:last_include_pos] + '\n#include "loginterface.h"' + content[last_include_pos:]
                # 再次替换
                new_content = new_content.replace('RWDb::writeAuditTrailLog', 'LOG_IF.writeAuditTrailLog')
        
        # 写回文件
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        print(f"处理完成: {file_path}")
        
    except Exception as e:
        print(f"处理 {file_path} 时出错: {e}")

if __name__ == "__main__":
    print("开始处理 AppOnlinePlatform 文件...")
    for file_path in files_to_process:
        if os.path.exists(file_path):
            process_file(file_path)
        else:
            print(f"文件不存在: {file_path}")
    print("处理完成！")
