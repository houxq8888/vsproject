import os
import re

# 定义要替换的内容
def fix_cmake_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 替换qt5_wrap_cpp
    qt5_wrap_cpp_pattern = re.compile(r'qt5_wrap_cpp\((.*?)\)', re.DOTALL)
    content = qt5_wrap_cpp_pattern.sub('''# 根据Qt版本选择对应的MOC命令
if(WIN32)
    qt_wrap_cpp(\g<1>)
else()
    qt5_wrap_cpp(\g<1>)
endif()''', content)
    
    # 替换Qt5::模块
    qt5_module_pattern = re.compile(r'Qt5::(\w+)')
    content = qt5_module_pattern.sub('''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::\g<1>
else()
    Qt5::\g<1>
endif()''', content)
    
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

# 遍历所有子模块的CMakeLists.txt
for root, dirs, files in os.walk('.'):
    for file in files:
        if file == 'CMakeLists.txt' and not 'test_build' in root and not 'build_windows' in root:
            file_path = os.path.join(root, file)
            print(f'Fixing {file_path}...')
            fix_cmake_file(file_path)