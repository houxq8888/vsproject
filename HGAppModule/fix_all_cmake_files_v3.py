import os
import re

def fix_cmake_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Add Qt library definition at the top of the file
    qt_libraries_def = '''# Define Qt libraries based on OS
if(WIN32)
    set(QT_LIBRARIES
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
    )
else()
    set(QT_LIBRARIES
        Qt5::Gui
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::PrintSupport
    )
endif()
'''

    # Add the Qt libraries definition after the project command
    if 'project(' in content:
        content = re.sub(r'(project\([^)]*\))', r'\1\n\n' + qt_libraries_def, content)
    else:
        # If no project command, add at the beginning
        content = qt_libraries_def + '\n' + content

    # Replace all Qt module references with ${QT_LIBRARIES}
    content = re.sub(r'Qt[56]::Gui|Qt[56]::Core|Qt[56]::Widgets|Qt[56]::Charts|Qt[56]::PrintSupport', '', content)
    content = re.sub(r'if\(WIN32\).*?endif\(\)', '', content, flags=re.DOTALL)
    content = re.sub(r'# 根据Qt版本选择对应的模块', '', content)

    # Add ${QT_LIBRARIES} to the dependency lists
    # Look for HG_CMAKE_MODULE_DEPEND_PUB or HG_CMAKE_MODULE_DEPEND calls
    content = re.sub(r'(HG_CMAKE_MODULE_DEPEND(PUB)?\([^)]*?)\)', r'\1\n    ${QT_LIBRARIES}\n)', content, flags=re.DOTALL)

    # Fix MOC command syntax if still present
    content = re.sub(r'if\(WIN32\)\s*qt_wrap_cpp\s*else\(\)\s*qt5_wrap_cpp\s*endif\(\)\(MOC_SRCS \${MOC_HEADERS}\)', r'if(WIN32)\n    qt_wrap_cpp(MOC_SRCS ${MOC_HEADERS})\nelse()\n    qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})\nendif()', content, flags=re.DOTALL)

    # Write back the fixed content
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

    print(f"Fixed: {file_path}")

# Find all CMakeLists.txt files in subdirectories
for root, dirs, files in os.walk('.'):
    if 'CMakeLists.txt' in files and not root.endswith('build_windows'):
        file_path = os.path.join(root, 'CMakeLists.txt')
        fix_cmake_file(file_path)

print("All CMakeLists.txt files fixed successfully!")