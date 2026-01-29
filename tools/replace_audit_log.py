#!/usr/bin/env python3
import os
import re

# 需要处理的文件列表
files_to_process = [
    r"D:\virtualMachine\github\vsproject\HGAppModule\UILogin\src\hgloginwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\warningwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\userwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\networkwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\userinfoeditwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\languagewidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\hgsystemsetwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\hgdeviceinfowidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\hgserviceinfowidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\displayandlightwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIUserAudit\src\authoritywidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UITask\src\hgtaskmanagewidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIReagent\src\hgreagentmanagewidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIFlow\src\hgflowwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIExceptionHandle\src\hgexceptionhandlewidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIChannel\src\hgchannelsetwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIAnalysisRecord\src\hganalysisrecordwidget.cpp",
    r"D:\virtualMachine\github\vsproject\HGAppModule\UIMethod\src\hgmethodmanagewidget.cpp",
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
    print("开始处理文件...")
    for file_path in files_to_process:
        if os.path.exists(file_path):
            process_file(file_path)
        else:
            print(f"文件不存在: {file_path}")
    print("处理完成！")
