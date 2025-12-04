@echo off
setlocal enabledelayedexpansion

REM 设置编码解决中文乱码问题
chcp 65001 >nul

echo ===============================================
echo HGAppModule 部署脚本
echo ===============================================
echo.

REM 设置默认参数
set MODULE_NAME=HGUserAuditModule
set BUILD_DIR=windows_build
set DEPLOY_DIR=%MODULE_NAME%_Release
set QT_DIR=D:\Qt\6.9.1\mingw_64

REM 解析命令行参数
:parse_args
if "%~1"=="" goto :args_done

if /i "%~1"=="--module" (
    if not "%~2"=="" (
        set MODULE_NAME=%~2
        set DEPLOY_DIR=%MODULE_NAME%_Release
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--build-dir" (
    if not "%~2"=="" (
        set BUILD_DIR=%~2
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--deploy-dir" (
    if not "%~2"=="" (
        set DEPLOY_DIR=%~2
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--qt-dir" (
    if not "%~2"=="" (
        set QT_DIR=%~2
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--help" (
    goto :show_help
)

echo 错误: 未知参数 "%~1"
goto :show_help

:show_help
echo.
echo HGAppModule 部署脚本
echo.
echo 用法: deploy.bat [选项]
echo.
echo 选项:
echo   --module NAME     指定要部署的模块名 (默认: HGUserAuditModule)
echo   --build-dir DIR   指定构建目录 (默认: windows_build)
echo   --deploy-dir DIR  指定部署目录 (默认: 模块名_Release)
echo   --qt-dir DIR      指定Qt安装目录 (默认: D:\Qt\6.9.1\mingw_64)
echo   --help           显示此帮助信息
echo.
echo 示例:
echo   deploy.bat --module HGUserAuditModule
echo   deploy.bat --module HGCameraRecognizeModule --build-dir build
echo   deploy.bat --module HGAnalysisRecordModule --deploy-dir MyRelease
echo.
exit /b 1

:args_done

REM 显示部署配置
echo ===============================================
echo 部署配置
echo ===============================================
echo 模块名称: %MODULE_NAME%
echo 构建目录: %BUILD_DIR%
echo 部署目录: %DEPLOY_DIR%
echo Qt路径: %QT_DIR%
echo ===============================================
echo.

REM 检查构建目录是否存在
if not exist "%BUILD_DIR%" (
    echo 错误: 构建目录 "%BUILD_DIR%" 不存在
    echo 请先运行 windows_build.bat 编译模块
    exit /b 1
)

REM 检查模块目录是否存在
if not exist "%BUILD_DIR%\%MODULE_NAME%" (
    echo 错误: 模块目录 "%BUILD_DIR%\%MODULE_NAME%" 不存在
    echo 请确认模块名是否正确，或先编译该模块
    exit /b 1
)

REM 检查可执行文件是否存在
set EXE_FILE=%BUILD_DIR%\%MODULE_NAME%\%MODULE_NAME%Run.exe
if not exist "%EXE_FILE%" (
    echo 错误: 可执行文件 "%EXE_FILE%" 不存在
    echo 请确认模块是否已编译可执行程序
    exit /b 1
)

REM 创建部署目录
if exist "%DEPLOY_DIR%" (
    echo 删除现有部署目录...
    rmdir /s /q "%DEPLOY_DIR%"
)

echo 创建部署目录...
mkdir "%DEPLOY_DIR%"

REM 复制可执行文件
echo 复制可执行文件...
copy "%EXE_FILE%" "%DEPLOY_DIR%\" >nul

REM 使用windeployqt收集Qt依赖库
echo 收集Qt依赖库...
"%QT_DIR%\bin\windeployqt.exe" "%DEPLOY_DIR%\%MODULE_NAME%Run.exe"

if %errorlevel% neq 0 (
    echo 警告: windeployqt执行过程中可能出现问题
)

REM 创建配置文件目录
echo 创建配置文件...
mkdir "%DEPLOY_DIR%\config" >nul 2>&1

REM 创建基础配置文件
echo ^<?xml version="1.0" encoding="UTF-8"?^> > "%DEPLOY_DIR%\config\translations.xml"
echo ^<translations^> >> "%DEPLOY_DIR%\config\translations.xml"
echo     ^<!-- %MODULE_NAME% 模块翻译配置 --^> >> "%DEPLOY_DIR%\config\translations.xml"
echo     ^<language code="zh_CN"^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="app_title"^>%MODULE_NAME%^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="start_app"^>启动应用^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="stop_app"^>停止应用^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo     ^</language^> >> "%DEPLOY_DIR%\config\translations.xml"
echo     ^<language code="en_US"^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="app_title"^>%MODULE_NAME%^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="start_app"^>Start App^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo         ^<string key="stop_app"^>Stop App^</string^> >> "%DEPLOY_DIR%\config\translations.xml"
echo     ^</language^> >> "%DEPLOY_DIR%\config\translations.xml"
echo ^</translations^> >> "%DEPLOY_DIR%\config\translations.xml"

REM 创建启动脚本
echo 创建启动脚本...
echo @echo off > "%DEPLOY_DIR%\启动程序.bat"
echo chcp 65001 ^>nul >> "%DEPLOY_DIR%\启动程序.bat"
echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo echo ======================================== >> "%DEPLOY_DIR%\启动程序.bat"
echo echo    %MODULE_NAME% 模块 >> "%DEPLOY_DIR%\启动程序.bat"
echo echo ======================================== >> "%DEPLOY_DIR%\启动程序.bat"
echo echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo echo 正在启动 %MODULE_NAME%... >> "%DEPLOY_DIR%\启动程序.bat"
echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo REM 设置当前目录为脚本所在目录 >> "%DEPLOY_DIR%\启动程序.bat"
echo cd /d "%%~dp0" >> "%DEPLOY_DIR%\启动程序.bat"
echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo REM 启动模块 >> "%DEPLOY_DIR%\启动程序.bat"
echo %MODULE_NAME%Run.exe >> "%DEPLOY_DIR%\启动程序.bat"
echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo if %%errorlevel%% neq 0 ( >> "%DEPLOY_DIR%\启动程序.bat"
echo     echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo     echo 程序异常退出，错误代码: %%errorlevel%% >> "%DEPLOY_DIR%\启动程序.bat"
echo     echo 请检查依赖库是否完整 >> "%DEPLOY_DIR%\启动程序.bat"
echo     pause >> "%DEPLOY_DIR%\启动程序.bat"
echo ) else ( >> "%DEPLOY_DIR%\启动程序.bat"
echo     echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo     echo 程序已正常退出 >> "%DEPLOY_DIR%\启动程序.bat"
echo ) >> "%DEPLOY_DIR%\启动程序.bat"
echo. >> "%DEPLOY_DIR%\启动程序.bat"
echo pause >> "%DEPLOY_DIR%\启动程序.bat"

REM 创建说明文档
echo 创建说明文档...
echo # %MODULE_NAME% 模块部署包 > "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ## 项目描述 >> "%DEPLOY_DIR%\README.md"
echo %MODULE_NAME% 是一个基于Qt框架开发的模块。 >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ## 系统要求 >> "%DEPLOY_DIR%\README.md"
echo - Windows 7/10/11 操作系统 >> "%DEPLOY_DIR%\README.md"
echo - 至少2GB可用内存 >> "%DEPLOY_DIR%\README.md"
echo - 支持OpenGL的显卡 >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ## 使用方法 >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ### 方法一：双击启动（推荐） >> "%DEPLOY_DIR%\README.md"
echo 直接双击 `启动程序.bat` 文件即可运行程序。 >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ### 方法二：命令行启动 >> "%DEPLOY_DIR%\README.md"
echo 打开命令提示符，切换到程序目录，然后运行： >> "%DEPLOY_DIR%\README.md"
echo ```cmd >> "%DEPLOY_DIR%\README.md"
echo %MODULE_NAME%Run.exe >> "%DEPLOY_DIR%\README.md"
echo ``` >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo ## 部署信息 >> "%DEPLOY_DIR%\README.md"
echo - 部署时间: %date% %time% >> "%DEPLOY_DIR%\README.md"
echo - 模块名称: %MODULE_NAME% >> "%DEPLOY_DIR%\README.md"
echo - Qt版本: 6.9.1 >> "%DEPLOY_DIR%\README.md"
echo. >> "%DEPLOY_DIR%\README.md"
echo --- >> "%DEPLOY_DIR%\README.md"
echo *%MODULE_NAME% - 专业的模块解决方案* >> "%DEPLOY_DIR%\README.md"

REM 显示部署结果
echo ===============================================
echo %MODULE_NAME% 部署完成!
echo ===============================================
echo.
echo 部署目录: %DEPLOY_DIR%
echo 包含文件:
echo   - %MODULE_NAME%Run.exe (主程序)
echo   - *.dll (Qt依赖库)
echo   - iconengines/ (图标引擎插件)
echo   - imageformats/ (图像格式插件)
echo   - platforms/ (平台插件)
echo   - translations/ (多语言翻译文件)
echo   - config/ (配置文件目录)
echo   - 启动程序.bat (启动脚本)
echo   - README.md (说明文档)
echo.
echo 使用方法:
echo   1. 双击 "启动程序.bat" 启动程序
echo   2. 或运行 "%MODULE_NAME%Run.exe"
echo.

endlocal