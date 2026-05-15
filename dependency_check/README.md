# 依赖检查系统

本目录包含了项目的依赖检查系统，用于验证模块间的依赖关系，确保符合依赖层次设计。

## 文件说明

- `generated_cmake_code.cmake` - 自动生成的CMake代码，包含对项目中所有模块的依赖检查调用
- `validation_result.json` - 依赖关系验证结果，包含所有模块及其依赖关系
- `依赖检查模块实现总结.md` - 详细的实现说明和使用指南
- `README.md` - 本文件，使用说明

## 如何使用

### 1. 运行依赖检查

从项目根目录执行以下命令：

```bash
python tools/cmake_dependency_validator.py --project-root . --output-json dependency_check/validation_result.json --output-cmake dependency_check/generated_cmake_code.cmake
```

### 2. 集成到构建系统

将 `generated_cmake_code.cmake` 中的代码添加到主CMakeLists.txt文件中，或者直接包含该文件：

```cmake
# 在主CMakeLists.txt中添加
include(dependency_check/generated_cmake_code.cmake)
```

### 3. 查看验证结果

打开 `validation_result.json` 文件查看详细的依赖关系分析结果。

### 4. 生成依赖关系图

使用Graphviz将生成的依赖关系图转换为图片：

```bash
dot -Tpng ${CMAKE_BINARY_DIR}/dependency_graph.dot -o dependency_graph.png
```

## 注意事项

1. 每次修改项目中的依赖关系后，建议重新运行依赖检查脚本
2. `generated_cmake_code.cmake` 是自动生成的，不应手动修改
3. 依赖检查模块位于 `cmake/dependency_check.cmake`，包含所有依赖检查的核心功能

## 目录结构

```
dependency_check/
├── README.md                           # 本文件
├── generated_cmake_code.cmake           # 自动生成的CMake代码
├── validation_result.json               # 依赖关系验证结果
└── 依赖检查模块实现总结.md                # 详细实现说明
```