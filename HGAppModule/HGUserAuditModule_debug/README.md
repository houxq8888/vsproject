# HGUserAuditModule 单步调试指南

## 调试环境已成功配置完成！

### 编译状态
✅ HGUserAuditModule调试版本已成功编译
✅ 调试配置文件已生成
✅ 调试环境已准备就绪

## 三种调试方法

### 方法一：使用Visual Studio Code调试（推荐）

1. **打开VS Code**
2. **打开文件夹**: `d:\virtualMachine\github\vsproject\HGUserAuditModule_debug`
3. **设置断点**: 在`authoritywidget.cpp`第1050行或其他需要调试的代码行设置断点
4. **开始调试**: 按`F5`或选择"运行和调试"开始调试
5. **单步调试**:
   - `F10`: 单步跳过
   - `F11`: 单步进入函数
   - `Shift+F11`: 单步跳出函数
   - `F5`: 继续执行

### 方法二：使用GDB命令行调试

1. **打开命令提示符**
2. **进入部署目录**:
   ```cmd
   cd /d "d:\virtualMachine\github\vsproject\HGAppModule\windows_build\deploy"
   ```
3. **启动GDB调试**:
   ```cmd
   "D:\Qt\Tools\mingw1310_64\bin\gdb.exe" HGUserAuditModuleRun.exe
   ```
4. **常用GDB命令**:
   ```gdb
   # 设置断点
   break authoritywidget.cpp:1050
   break AuthorityWidget::setAuthority
   break AuthorityWidget::fnWriteDB
   
   # 运行程序
   run
   
   # 单步调试
   step      # 进入函数
   next      # 跳过函数
   continue  # 继续执行
   
   # 查看变量
   print variable_name
   
   # 查看堆栈
   backtrace
   
   # 退出调试
   quit
   ```

### 方法三：使用Qt Creator调试

1. **打开Qt Creator**
2. **打开项目**: `d:\virtualMachine\github\vsproject\HGAppModule\CMakeLists.txt`
3. **配置Kit**: 选择MinGW Debug配置
4. **构建项目**: 构建HGUserAuditModuleRun目标
5. **开始调试**: 点击调试按钮开始单步调试

## 重要调试断点建议

### 权限设置相关断点
1. **AuthorityWidget::setAuthority()** - 权限设置入口函数
2. **AuthorityWidget::fnWriteDB()** - 数据库保存逻辑
3. **GlobalSingleton::commitAuthorityEdit()** - 权限编辑提交
4. **authoritywidget.cpp第1050行** - 你关注的代码区域

### 用户管理相关断点
1. **UserWidget::fnWriteDB()** - 用户数据保存
2. **UserWidget::fnAddUsrsGroupUser()** - 添加用户到权限组
3. **UserWidget::fnRemoveUsrsGroupUser()** - 从权限组移除用户

## 调试版本特点

- ✅ **完整调试符号**: 包含所有调试信息
- ✅ **优化关闭**: 使用`-O0`优化级别，便于单步调试
- ✅ **调试宏定义**: 启用了调试相关的宏定义
- ✅ **变量查看**: 可以查看所有局部变量和成员变量

## 重新编译调试版本

如果需要重新编译调试版本，可以使用以下命令：

```cmd
cd /d "d:\virtualMachine\github\vsproject\HGAppModule"
windows_build.bat --debug --module HGUserAuditModule --clean
```

## 调试技巧

1. **条件断点**: 在VS Code中可以设置条件断点，只在特定条件下触发
2. **观察窗口**: 使用观察窗口监控重要变量的值变化
3. **调用堆栈**: 查看函数调用堆栈了解程序执行流程
4. **内存查看**: 在GDB中可以使用`x`命令查看内存内容

## 常见问题解决

### 调试器无法启动
- 检查GDB路径是否正确：`D:\Qt\Tools\mingw1310_64\bin\gdb.exe`
- 确保调试版本已正确编译

### 断点不生效
- 确认源代码与编译版本一致
- 检查断点是否设置在有效代码行

### 变量值显示不正确
- 确保使用调试版本编译
- 检查优化级别是否为`-O0`

## 调试目标

通过单步调试，你可以：

1. **验证权限保存逻辑**: 确认权限修改后是否正确保存到数据库
2. **检查函数调用流程**: 了解权限设置和用户管理的完整流程
3. **调试修复效果**: 验证之前修复的权限保存功能是否正常工作
4. **发现潜在问题**: 通过调试发现代码中可能存在的其他问题

---

**开始调试吧！** 使用你喜欢的调试方法，单步执行HGUserAuditModule代码，深入了解权限设置和用户管理的实现细节。