@echo off
setlocal enabledelayedexpansion

REM 设置编码解决中文乱码问题
chcp 65001 >nul

REM 读取应用程序名称
set APP_NAME=产品编辑器
if exist "build_config.ini" (
    for /f "usebackq tokens=1,2 delims==" %%A in (`findstr /i "^name=" build_config.ini`) do (
        if /i "%%A"=="name" set APP_NAME=%%B
    )
)

echo ===============================================
echo %APP_NAME% - 可用模块列表
echo ===============================================
echo.

set MODULE_COUNT=0

REM 检测所有包含CMakeLists.txt的模块文件夹
for /d %%d in (*) do (
    if exist "%%d\CMakeLists.txt" (
        set /a MODULE_COUNT+=1
        echo [!MODULE_COUNT!] %%d
    )
)

echo ===============================================
echo 总共发现 !MODULE_COUNT! 个模块
echo ===============================================
echo.

echo 使用示例:
echo   smart_build.bat --module HGUserAuditModule
echo   smart_build.bat --module HGBaseAppModule,HGUserAuditModule
echo   smart_build.bat --module ALL (编译所有模块)
echo.

endlocal