# vsproject 项目技术架构展示

## 概述

本项目提供了一个交互式的前端页面，用于展示 vsproject 项目的完整技术架构、模块功能和开发方案。

## 文件说明

- `project_architecture.html` - 主要的前端展示页面
- `view_architecture.bat` - Windows环境下的启动脚本
- `ARCHITECTURE_README.md` - 本说明文档

## 快速开始

### 方法一：使用启动脚本（推荐）

在 Windows 环境下，双击运行 `view_architecture.bat` 脚本：

```bash
.\view_architecture.bat
```

脚本会自动检测文件并提供三种打开方式：
1. 使用默认浏览器直接打开
2. 复制文件路径
3. 查看文件内容

### 方法二：手动打开

直接在浏览器中打开 `project_architecture.html` 文件：

```bash
# 在文件管理器中双击打开
# 或使用浏览器文件菜单打开
```

## 页面功能

### 1. 技术架构概览
- 展示项目的整体技术栈
- 包括编程语言、框架、工具等

### 2. 模块分类展示
页面将项目模块分为四大类：

#### 应用模块 (Applications)
- HGDashboardApp - 仪表盘应用
- HGComDebugApp - 通信调试应用  
- HGCupDetApp - 杯检测应用
- HGOnlinePlatformApp - 在线平台应用
- ProductFlow - 产品流程应用

#### 核心模块 (Core Modules)
- HGBaseAppModule - 基础应用模块
- HGUserAuditModule - 用户审计模块
- HGLogModule - 日志模块
- HGLoginModule - 登录模块
- HGSearchModule - 搜索模块

#### 功能模块 (Functional Modules)
- HGChannelModule - 通道模块
- HGCurveModule - 曲线模块
- HGFlowModule - 流程模块
- HGMethodModule - 方法模块
- HGReagentModule - 试剂模块

#### 服务模块 (Service Modules)
- HGCommon - 通用服务
- HGConfig - 配置服务
- HGHardware - 硬件服务
- HGImageAlgorithmService - 图像算法服务
- HGCommunicateService - 通信服务

### 3. 交互功能
- 点击任意模块查看详细信息和依赖关系
- 响应式设计，支持移动端查看
- 动画效果和友好的用户界面

### 4. 构建和部署信息
- 提供项目的构建命令
- 部署和测试相关信息

## 技术特点

### 项目架构
- **模块化设计**: 采用插件化架构，支持功能扩展
- **跨平台支持**: 支持 Linux 和 Windows 平台
- **标准化构建**: 使用 CMake 进行项目构建管理

### 技术栈
- **编程语言**: C++ 17/20
- **GUI框架**: Qt 5.14+
- **图像处理**: OpenCV 4.6+
- **网络通信**: MQTT/WebSocket
- **数据库**: SQLCipher
- **构建系统**: CMake 3.22+

### 开发工具
- **编译器**: GCC/Clang (Linux), MSVC (Windows)
- **调试工具**: GDB, Visual Studio Debugger
- **版本控制**: Git
- **持续集成**: 支持自动化构建和测试

## 构建和运行

### 编译项目
```bash
# 使用根目录下的 build.bat
.\build.bat
```

### 运行程序
```bash
# 编译完成后运行可执行文件
.\build\software-editor.exe
```

### 测试脚本
```bash
# UI自动化测试脚本位置
.\test\ui_automation
```

## 项目结构

```
vsproject/
├── 3rdparty/          # 第三方库
├── HGAppModule/       # 应用模块
├── modules/          # 功能模块
├── services/         # 服务模块
├── interfaces/       # 接口定义
├── processes/        # 进程管理
├── projects/         # 应用项目
└── cmake/           # CMake配置
```

## 开发指南

### 添加新模块
1. 在相应目录下创建模块文件夹
2. 编写 CMakeLists.txt 文件
3. 在主 CMakeLists.txt 中添加子项目
4. 实现模块功能并测试

### 模块依赖管理
- 使用 CMake 的 target_link_libraries 管理依赖
- 遵循模块化设计原则，减少循环依赖
- 通过接口定义实现模块间通信

## 贡献指南

欢迎提交 Issue 和 Pull Request 来改进这个架构展示页面。

## 许可证

本项目遵循 vsproject 项目的许可证协议。

## 联系信息

如有问题或建议，请通过项目维护渠道联系。