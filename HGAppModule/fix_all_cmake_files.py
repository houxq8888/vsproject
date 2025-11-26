import os
import re

def fix_cmake_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Fix MOC command syntax
    content = re.sub(r'if\(WIN32\)\s*qt_wrap_cpp\s*else\(\)\s*qt5_wrap_cpp\s*endif\(\)\(MOC_SRCS \${MOC_HEADERS}\)', r'if(WIN32)\n    qt_wrap_cpp(MOC_SRCS ${MOC_HEADERS})\nelse()\n    qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})\nendif()', content, flags=re.DOTALL)

    # Fix Qt module dependencies
    qt_modules = ["Gui", "Core", "Widgets", "Charts", "PrintSupport"]
    for module in qt_modules:
        content = re.sub(r'if\(WIN32\)\s*Qt6::' + module + r'\s*else\(\)\s*if\(WIN32\)\s*Qt6::' + module + r'\s*else\(\)\s*if\(WIN32\)\s*Qt6::' + module + r'\s*else\(\)\s*Qt5::' + module + r'\s*endif\(\)\s*endif\(\)\s*endif\(\)', r'Qt5::' + module, content, flags=re.DOTALL)

    # Write back the fixed content
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

    print(f"Fixed: {file_path}")

# Find all CMakeLists.txt files in subdirectories
for root, dirs, files in os.walk('.'):
    if 'CMakeLists.txt' in files:
        file_path = os.path.join(root, 'CMakeLists.txt')
        fix_cmake_file(file_path)

print("All CMakeLists.txt files fixed successfully!")