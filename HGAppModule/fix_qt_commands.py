import os
import glob

# 定义要替换的内容
def fix_cmake_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 替换qt5_wrap_cpp
    content = content.replace('qt5_wrap_cpp', '''# 根据Qt版本选择对应的MOC命令
if(WIN32)
    qt_wrap_cpp
else()
    qt5_wrap_cpp
endif()''')
    
    # 替换Qt5::模块
    content = content.replace('Qt5::Gui', '''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::Gui
else()
    Qt5::Gui
endif()''')
    content = content.replace('Qt5::Core', '''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::Core
else()
    Qt5::Core
endif()''')
    content = content.replace('Qt5::Widgets', '''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::Widgets
else()
    Qt5::Widgets
endif()''')
    content = content.replace('Qt5::Charts', '''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::Charts
else()
    Qt5::Charts
endif()''')
    content = content.replace('Qt5::PrintSupport', '''# 根据Qt版本选择对应的模块
if(WIN32)
    Qt6::PrintSupport
else()
    Qt5::PrintSupport
endif()''')
    
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

# 遍历所有子模块的CMakeLists.txt
for root, dirs, files in os.walk('.'):
    for file in files:
        if file == 'CMakeLists.txt':
            file_path = os.path.join(root, file)
            print(f'Fixing {file_path}...')
            fix_cmake_file(file_path)