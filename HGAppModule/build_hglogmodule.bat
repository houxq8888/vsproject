@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ===============================================
echo HGLogModule 构建脚本
echo ===============================================
echo.

echo 1. 检查构建环境...

REM 检查Qt路径
set QT_DIR=C:\Qt\6.9.3\mingw_64
if not exist "%QT_DIR%" (
    echo 错误: Qt路径不存在: %QT_DIR%
    echo 请检查Qt安装路径
    pause
    exit /b 1
)

echo ✓ Qt路径: %QT_DIR%

REM 检查MinGW路径
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
if not exist "%MINGW_DIR%" (
    echo 错误: MinGW路径不存在: %MINGW_DIR%
    echo 请检查MinGW安装路径
    pause
    exit /b 1
)

echo ✓ MinGW路径: %MINGW_DIR%

REM 设置环境变量
set PATH=%MINGW_DIR%\bin;%QT_DIR%\bin;%PATH%

echo.
echo 2. 开始构建...

REM 进入HGAppModule目录
cd /d "c:\Users\liguitao\myjob\vsproject\HGAppModule"

REM 使用windows_build.bat编译
echo 执行: windows_build.bat --release --module HGLogModule --incremental
echo.
call windows_build.bat --release --module HGLogModule --incremental

if %ERRORLEVEL% neq 0 (
    echo.
    echo 编译失败! 错误代码: %ERRORLEVEL%
    echo 请检查 build_log.txt 获取详细信息
    pause
    exit /b 1
)

echo.
echo ✓ HGLogModule 编译成功!
echo.
echo 生成的文件位于: windows_build\deploy\
echo.

if exist "windows_build\deploy\HGLogModuleRun.exe" (
    echo 可执行文件: windows_build\deploy\HGLogModuleRun.exe
    echo.
    echo 按任意键启动程序...
    pause >nul
    cd windows_build\deploy
    start HGLogModuleRun.exe
)

pause
