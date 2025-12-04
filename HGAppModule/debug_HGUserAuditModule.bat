@echo off
chcp 65001 >nul

echo ===============================================
echo HGUserAuditModule 单步调试配置脚本
echo ===============================================
echo.

echo 1. 检查调试环境...

REM 检查Qt路径
set QT_DIR=D:\Qt\6.9.1\mingw_64
if not exist "%QT_DIR%" (
    echo 错误: Qt路径不存在: %QT_DIR%
    echo 请检查Qt安装路径
    pause
    exit /b 1
)

echo ✓ Qt路径: %QT_DIR%

REM 检查MinGW路径
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
if not exist "%MINGW_DIR%" (
    echo 错误: MinGW路径不存在: %MINGW_DIR%
    echo 请检查MinGW安装路径
    pause
    exit /b 1
)

echo ✓ MinGW路径: %MINGW_DIR%

REM 设置环境变量
set PATH=%MINGW_DIR%\bin;%PATH%

echo.
echo 2. 编译调试版本...

REM 进入HGAppModule目录
cd /d "d:\virtualMachine\github\vsproject\HGAppModule"

REM 使用windows_build.bat编译调试版本
echo 执行: windows_build.bat --debug --module HGUserAuditModule --clean
echo.
call windows_build.bat --debug --module HGUserAuditModule --clean

if %ERRORLEVEL% neq 0 (
    echo.
    echo 编译失败! 错误代码: %ERRORLEVEL%
    echo 请检查编译错误信息
    pause
    exit /b 1
)

echo.
echo ✓ HGUserAuditModule调试版本编译成功

echo.
echo 3. 生成调试配置...

REM 创建调试配置文件
cd /d "d:\virtualMachine\github\vsproject"

set DEBUG_CONFIG_FILE=HGUserAuditModule_debug.vscode\launch.json

if not exist "HGUserAuditModule_debug" mkdir "HGUserAuditModule_debug"
if not exist "HGUserAuditModule_debug\.vscode" mkdir "HGUserAuditModule_debug\.vscode"

echo {
echo   "version": "0.2.0",
echo   "configurations": [
echo     {
echo       "name": "调试 HGUserAuditModule",
echo       "type": "cppdbg",
echo       "request": "launch",
echo       "program": "${workspaceFolder}/HGAppModule/windows_build/deploy/HGUserAuditModuleRun.exe",
echo       "args": [],
echo       "stopAtEntry": false,
echo       "cwd": "${workspaceFolder}/HGAppModule/windows_build/deploy",
echo       "environment": [],
echo       "externalConsole": true,
echo       "MIMode": "gdb",
echo       "miDebuggerPath": "D:\\Qt\\Tools\\mingw1310_64\\bin\\gdb.exe",
echo       "setupCommands": [
echo         {
echo           "description": "为 gdb 启用整齐打印",
echo           "text": "-enable-pretty-printing",
echo           "ignoreFailures": true
echo         }
echo       ],
echo       "preLaunchTask": "build-debug",
echo       "logging": {
echo         "moduleLoad": false,
echo         "engineLogging": false
echo       }
echo     }
echo   ]
echo } > "%DEBUG_CONFIG_FILE%"

set TASKS_CONFIG_FILE=HGUserAuditModule_debug.vscode\tasks.json

echo {
echo   "version": "2.0.0",
echo   "tasks": [
echo     {
echo       "label": "build-debug",
echo       "type": "shell",
echo       "command": "cmd",
echo       "args": [
echo         "/c",
echo         "cd /d d:\\virtualMachine\\github\\vsproject\\HGAppModule && windows_build.bat --debug --module HGUserAuditModule --incremental"
echo       ],
echo       "group": "build",
echo       "presentation": {
echo         "echo": true,
echo         "reveal": "always",
echo         "focus": false,
echo         "panel": "shared"
echo       },
echo       "problemMatcher": []
echo     }
echo   ]
echo } > "%TASKS_CONFIG_FILE%"

echo.
echo 4. 调试说明...

echo ===============================================
echo 单步调试HGUserAuditModule的步骤:
echo ===============================================
echo.
echo 方法一: 使用Visual Studio Code调试
echo   1. 打开VS Code
echo   2. 打开文件夹: %CD%\HGUserAuditModule_debug
echo   3. 按F5或选择"运行和调试"开始调试
echo   4. 在authoritywidget.cpp等文件中设置断点
echo   5. 单步执行(F10)或进入函数(F11)进行调试

echo.
echo 方法二: 使用GDB命令行调试
echo   1. 打开命令提示符
echo   2. cd /d "%CD%\HGAppModule\windows_build\deploy"
echo   3. gdb HGUserAuditModuleRun.exe
echo   4. 在GDB中使用以下命令:
echo      - break authoritywidget.cpp:1050 (在1050行设置断点)
echo      - run (运行程序)
echo      - step (单步进入)
echo      - next (单步跳过)
echo      - print 变量名 (查看变量值)

echo.
echo 方法三: 使用Qt Creator调试
echo   1. 打开Qt Creator
echo   2. 打开项目: %CD%\HGAppModule\CMakeLists.txt
echo   3. 配置Kit为MinGW Debug
echo   4. 构建并调试HGUserAuditModuleRun

echo.
echo ===============================================
echo 重要调试断点建议:
echo ===============================================
echo.
echo 1. AuthorityWidget::setAuthority() - 权限设置入口
echo 2. AuthorityWidget::fnWriteDB() - 数据库保存逻辑
echo 3. UserWidget::fnWriteDB() - 用户数据保存
echo 4. GlobalSingleton::commitAuthorityEdit() - 权限编辑提交
echo 5. authoritywidget.cpp第1050行附近 - 你关注的代码区域

echo.
echo 调试版本特点:
echo - 包含完整的调试符号
echo - 关闭了优化(-O0)
echo - 启用了调试宏定义
echo - 可以单步执行和查看变量

echo.
echo ✓ 调试环境配置完成!
echo.

pause