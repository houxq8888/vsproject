# HGUserAuditModule 用户审计模块

## 项目描述
HGUserAuditModule 是一个基于Qt框架开发的用户审计模块，用于监控和记录用户操作行为。

## 系统要求
- Windows 7/10/11 操作系统
- 至少2GB可用内存
- 支持OpenGL的显卡

## 文件结构
```
HGUserAuditModule_Release/
├── HGUserAuditModuleRun.exe     # 主程序
├── *.dll                         # Qt依赖库
├── iconengines/                  # 图标引擎插件
├── imageformats/                 # 图像格式插件
├── platforms/                    # 平台插件
├── translations/                 # 多语言翻译文件
├── config/                       # 配置文件目录
│   └── translations.xml          # 翻译配置文件
├── 启动程序.bat                  # 启动脚本
└── README.md                     # 说明文档
```

## 使用方法

### 方法一：双击启动（推荐）
直接双击 `启动程序.bat` 文件即可运行程序。

### 方法二：命令行启动
打开命令提示符，切换到程序目录，然后运行：
```cmd
HGUserAuditModuleRun.exe
```

## 注意事项
1. 请勿删除任何.dll文件或插件目录，否则程序将无法正常运行
2. 程序需要config目录下的配置文件才能正常工作
3. 如果程序无法启动，请检查是否缺少必要的系统组件

## 故障排除

### 常见问题
1. **程序无法启动**：检查是否所有依赖文件都存在
2. **界面显示异常**：检查显卡驱动是否支持OpenGL
3. **配置文件错误**：检查config/translations.xml文件是否完整

### 技术支持
如遇到问题，请联系开发团队获取技术支持。

## 版本信息
- 版本：1.0.0
- 构建时间：2025年12月
- Qt版本：6.9.1

---
*HGUserAuditModule - 专业的用户行为审计解决方案*