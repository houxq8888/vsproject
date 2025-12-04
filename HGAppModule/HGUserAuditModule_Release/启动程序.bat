@echo off
chcp 65001 >nul
echo ========================================
echo    HGUserAuditModule 用户审计模块
echo ========================================
echo.
echo 正在启动用户审计模块...

REM 设置当前目录为脚本所在目录
cd /d "%~dp0"

REM 启动用户审计模块
HGUserAuditModuleRun.exe

if %errorlevel% neq 0 (
    echo.
    echo 程序异常退出，错误代码: %errorlevel%
    echo 请检查依赖库是否完整
    pause
) else (
    echo.
    echo 程序已正常退出
)

pause