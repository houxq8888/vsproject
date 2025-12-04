@echo off
setlocal enabledelayedexpansion

REM 读取并应用build_config.ini配置
if exist "build_config.ini" (
    for /f "usebackq tokens=1,2 delims==" %%A in (`findstr /i "^encoding= ^console_output_codepage= ^input_codepage= ^use_utf8_encoding= ^force_console_utf8=" build_config.ini`) do (
        if /i "%%A"=="encoding" set CONFIG_ENCODING=%%B
        if /i "%%A"=="console_output_codepage" set CONFIG_CONSOLE_CP=%%B
        if /i "%%A"=="input_codepage" set CONFIG_INPUT_CP=%%B
        if /i "%%A"=="use_utf8_encoding" set CONFIG_USE_UTF8=%%B
        if /i "%%A"=="force_console_utf8" set CONFIG_FORCE_UTF8=%%B
    )
)

REM 设置编码解决中文乱码问题，优先使用配置文件中的设置
if "%CONFIG_CONSOLE_CP%" neq "" (
    chcp %CONFIG_CONSOLE_CP% >nul
) else if "%CONFIG_ENCODING%"=="UTF-8" (
    chcp 65001 >nul
) else if "%CONFIG_USE_UTF8%"=="true" (
    chcp 65001 >nul
) else if "%CONFIG_FORCE_UTF8%"=="true" (
    chcp 65001 >nul
) else (
    chcp 65001 >nul
)

REM 读取应用程序名称和版本
set APP_NAME=产品编辑器
set APP_VERSION=1.0.0
if exist "build_config.ini" (
    for /f "usebackq tokens=1,2 delims==" %%A in (`findstr /i "^name= ^version=" build_config.ini`) do (
        if /i "%%A"=="name" set APP_NAME=%%B
        if /i "%%A"=="version" set APP_VERSION=%%B
    )
)

REM 设置默认编译选项
set BUILD_EXECUTABLE=ON
set BUILD_SHARED_LIBS=OFF
set BUILD_STATIC_LIBS=ON
set BUILD_TYPE=Release

REM 自动检测CPU核心数并设置并行任务数
for /f "usebackq tokens=2 delims==" %%i in (`wmic cpu get NumberOfCores /value ^| findstr NumberOfCores`) do set /a CPU_CORES=%%i
if %CPU_CORES% gtr 0 (
    set /a JOB_COUNT=%CPU_CORES%
) else (
    set JOB_COUNT=4
)
if %JOB_COUNT% gtr 8 (
    set JOB_COUNT=8
)

REM 设置单个模块编译选项
set BUILD_MODULE=ALL
set CLEAN_BUILD=OFF

REM 解析命令行参数
:parse_args
if "%~1"=="" goto :args_done

if /i "%~1"=="--exe" (
    set BUILD_EXECUTABLE=ON
    shift
    goto :parse_args
)

if /i "%~1"=="--no-exe" (
    set BUILD_EXECUTABLE=OFF
    shift
    goto :parse_args
)

if /i "%~1"=="--shared" (
    set BUILD_SHARED_LIBS=ON
    shift
    goto :parse_args
)

if /i "%~1"=="--no-shared" (
    set BUILD_SHARED_LIBS=OFF
    shift
    goto :parse_args
)

if /i "%~1"=="--static" (
    set BUILD_STATIC_LIBS=ON
    shift
    goto :parse_args
)

if /i "%~1"=="--no-static" (
    set BUILD_STATIC_LIBS=OFF
    shift
    goto :parse_args
)

if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)

if /i "%~1"=="--release" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
)

if /i "%~1"=="--jobs" (
    if not "%~2"=="" (
        set JOB_COUNT=%~2
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--module" (
    if not "%~2"=="" (
        if "%BUILD_MODULE%"=="ALL" (
            set BUILD_MODULE=%~2
        ) else (
            set BUILD_MODULE=%BUILD_MODULE%,%~2
        )
        shift
        shift
        goto :parse_args
    )
)

if /i "%~1"=="--clean" (
    set CLEAN_BUILD=ON
    shift
    goto :parse_args
)

if /i "%~1"=="--clean-module" (
    set CLEAN_MODULE=ON
    shift
    goto :parse_args
)

if /i "%~1"=="--incremental" (
    set CLEAN_BUILD=OFF
    shift
    goto :parse_args
)

if /i "%~1"=="--help" (
    goto :show_help
)

REM 检查是否是模块参数的一部分（处理包含逗号的情况）
if "%BUILD_MODULE%" neq "ALL" (
    if "%BUILD_MODULE:~-1%"=="," (
        set BUILD_MODULE=%BUILD_MODULE%%~1
        shift
        goto :parse_args
    )
)

echo 错误: 未知参数 "%~1"
goto :show_help

:show_help
echo.
echo Windows HGAppModule 编译脚本
echo.
echo 用法: windows_build.bat [选项]
echo.
echo 编译选项:
echo   --exe          编译可执行程序 (默认: ON)
echo   --no-exe       不编译可执行程序
echo   --shared       编译动态库 (默认: ON)
echo   --no-shared    不编译动态库
echo   --static       编译静态库 (默认: OFF)
echo   --no-static    不编译静态库
echo   --debug        调试模式编译
echo   --release      发布模式编译 (默认)
echo   --jobs N       设置并行编译任务数 (默认: 4)
echo   --module NAME  仅编译指定模块 (默认: ALL)
echo   --clean        强制清空构建目录
echo   --clean-module 仅清理指定模块的编译文件
echo   --incremental  增量编译，不清空构建目录 (默认)
echo   --help         显示此帮助信息
echo.
echo 支持的模块:
echo   - ALL: 编译所有模块 (默认)
echo   - HGCameraRecognizeModule: 摄像头识别模块
echo   - HGAnalysisRecordModule: 分析记录模块
echo   - HGBaseAppModule: 基础应用模块
echo   - HGUserAuditModule: 用户审计模块
echo   - HGBaseAppModule,HGUserAuditModule: 同时编译基础应用和用户审计模块
echo.
echo 示例:
echo   windows_build.bat --exe --no-shared --no-static
echo   windows_build.bat --static --no-exe --no-shared
echo   windows_build.bat --debug --jobs 8
echo   windows_build.bat --module HGCameraRecognizeModule --incremental
echo   windows_build.bat --module HGAnalysisRecordModule --clean
echo   windows_build.bat --module HGBaseAppModule --module HGUserAuditModule --incremental
echo.
exit /b 1

:args_done

REM 显示编译配置
echo ===============================================
echo %APP_NAME% v%APP_VERSION% - Windows 编译配置
echo ===============================================
echo 应用程序: %APP_NAME%
echo 版本: %APP_VERSION%
echo 编译模式: %BUILD_TYPE%
echo 可执行程序: %BUILD_EXECUTABLE%
echo 动态库: %BUILD_SHARED_LIBS%
echo 静态库: %BUILD_STATIC_LIBS%
echo 编译模块: %BUILD_MODULE%
echo 清理构建: %CLEAN_BUILD%
echo 并行任务数: %JOB_COUNT%
echo ===============================================
echo.

REM Set Qt installation path
set QT_DIR=D:\Qt\6.9.1\mingw_64
echo Qt路径: %QT_DIR%

REM Configure MinGW environment
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
echo MinGW路径: %MINGW_DIR%
set PATH=%MINGW_DIR%\bin;%PATH%

REM Create build directory
set BUILD_DIR=windows_build

REM 根据清理选项处理构建目录
if "%CLEAN_BUILD%"=="ON" (
    if exist "%BUILD_DIR%" (
        echo 强制清空构建目录...
        rmdir /s /q "%BUILD_DIR%"
    )
    echo 创建新的构建目录...
    mkdir "%BUILD_DIR%"
) else if "%CLEAN_MODULE%"=="ON" (
    if exist "%BUILD_DIR%" (
        echo 清理指定模块: %BUILD_MODULE%
        if exist "%BUILD_DIR%\%BUILD_MODULE%" (
            echo 删除模块目录: %BUILD_DIR%\%BUILD_MODULE%
            rmdir /s /q "%BUILD_DIR%\%BUILD_MODULE%"
        )
        if exist "%BUILD_DIR%\deploy\%BUILD_MODULE%Run.exe" (
            echo 删除部署文件: %BUILD_DIR%\deploy\%BUILD_MODULE%Run.exe
            del /q "%BUILD_DIR%\deploy\%BUILD_MODULE%Run.exe"
        )
        echo 模块清理完成
    ) else (
        echo 构建目录不存在，无需清理
    )
    exit /b 0
) else (
    if exist "%BUILD_DIR%" (
        echo 使用现有构建目录进行增量编译...
    ) else (
        echo 创建新的构建目录...
        mkdir "%BUILD_DIR%"
    )
)

cd "%BUILD_DIR%"
echo 当前目录: %CD%

REM Run CMake configuration for Windows HGAppModule build
echo 运行CMake配置...

REM 根据模块选择设置CMake参数
if "%BUILD_MODULE%"=="ALL" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGAPPMODULE_ONLY=ON
) else if "%BUILD_MODULE%"=="HGCameraRecognizeModule" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGCAMERARECOGNIZEMODULE_ONLY=ON
) else if "%BUILD_MODULE%"=="HGAnalysisRecordModule" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGANALYSISRECORDMODULE_ONLY=ON
) else if "%BUILD_MODULE%"=="HGBaseAppModule" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGBASEAPPMODULE_ONLY=ON
) else if "%BUILD_MODULE%"=="HGUserAuditModule" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGUSERAUDITMODULE_ONLY=ON
) else if "%BUILD_MODULE%"=="HGBaseAppModule,HGUserAuditModule" (
    set CMAKE_MODULE_ARGS=-DBUILD_HGBASEAPPMODULE_ONLY=ON -DBUILD_HGUSERAUDITMODULE_ONLY=ON
) else (
    echo 错误: 不支持的模块 "%BUILD_MODULE%"
    echo 请使用 --help 查看支持的模块列表
    exit /b 1
)

"D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DHG_PROJECT_ROOT_FS="d:\virtualMachine\github\vsproject" ^
  -DPLATFORM="win32" ^
  %CMAKE_MODULE_ARGS% ^
  -DBUILD_EXECUTABLE=%BUILD_EXECUTABLE% ^
  -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS% ^
  -DBUILD_STATIC_LIBS=%BUILD_STATIC_LIBS% ^
  -DBUILD_HGBASEAPP_EXECUTABLE=OFF ^
  ..

if %errorlevel% neq 0 (
    echo CMake配置失败
    exit /b 1
)

REM Build the project
echo 编译模块: %BUILD_MODULE%
echo 编译日志将保存到: build_log.txt

REM 根据模块选择编译目标
if "%BUILD_MODULE%"=="ALL" (
    set MAKE_TARGET=all
    set MODULE_NAME=HGAppModule
) else if "%BUILD_MODULE%"=="HGCameraRecognizeModule" (
    if "%BUILD_EXECUTABLE%"=="ON" (
        set MAKE_TARGET=HGCameraRecognizeModuleRun
    ) else (
        set MAKE_TARGET=HGCameraRecognizeModule
    )
    set MODULE_NAME=HGCameraRecognizeModule
) else if "%BUILD_MODULE%"=="HGAnalysisRecordModule" (
    set MAKE_TARGET=HGAnalysisRecordModule
    set MODULE_NAME=HGAnalysisRecordModule
) else if "%BUILD_MODULE%"=="HGBaseAppModule" (
    set MAKE_TARGET=HGBaseAppModule
    set MODULE_NAME=HGBaseAppModule
) else if "%BUILD_MODULE%"=="HGUserAuditModule" (
    set MAKE_TARGET=HGUserAuditModuleStatic HGUserAuditModuleRun
    set MODULE_NAME=HGUserAuditModule
) else if "%BUILD_MODULE%"=="HGBaseAppModule,HGUserAuditModule" (
    set MAKE_TARGET=HGBaseAppModuleStatic HGUserAuditModuleStatic HGUserAuditModuleRun
    set MODULE_NAME=HGBaseAppModule和HGUserAuditModule
)

REM 编译并将输出重定向到文件
if "%BUILD_MODULE%"=="HGBaseAppModule,HGUserAuditModule" (
    echo 编译HGBaseAppModuleStatic...
    mingw32-make -j%JOB_COUNT% HGBaseAppModuleStatic >> build_log.txt 2>&1
    if %errorlevel% neq 0 (
        echo HGBaseAppModuleStatic编译失败
        goto :compile_failed
    )
    echo 编译HGUserAuditModuleStatic...
    mingw32-make -j%JOB_COUNT% HGUserAuditModuleStatic >> build_log.txt 2>&1
    if %errorlevel% neq 0 (
        echo HGUserAuditModuleStatic编译失败
        goto :compile_failed
    )
    echo 编译HGUserAuditModuleRun...
    mingw32-make -j%JOB_COUNT% HGUserAuditModuleRun >> build_log.txt 2>&1
    if %errorlevel% neq 0 (
        echo HGUserAuditModuleRun编译失败
        goto :compile_failed
    )
) else (
    mingw32-make -j%JOB_COUNT% %MAKE_TARGET% > build_log.txt 2>&1
    if %errorlevel% neq 0 goto :compile_failed
)

echo 编译成功，完整日志已保存到 build_log.txt
goto :compile_success

:compile_failed
echo 编译失败，请查看 build_log.txt 文件获取详细错误信息
echo 最后10行错误信息:
powershell -Command "Get-Content 'build_log.txt' | Select-Object -Last 10"
exit /b 1

:compile_success

REM 显示编译结果
echo ===============================================
echo %MODULE_NAME% Windows编译成功!
echo ===============================================
echo.

REM 根据模块显示生成的目标文件
if "%BUILD_MODULE%"=="ALL" (
    echo 生成的目标文件:
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGBaseAppModule.dll
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGCameraRecognizeModule.dll
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGAnalysisRecordModule.dll
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGBaseAppModuleStatic.a
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGCameraRecognizeModuleStatic.a
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGAnalysisRecordModuleStatic.a
    echo.
    echo 编译模块:
    echo   - HGBaseAppModule
echo   - HGCameraRecognizeModule
echo   - HGAnalysisRecordModule
) else if "%BUILD_MODULE%"=="HGBaseAppModule,HGUserAuditModule" (
    echo 生成的目标文件:
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGBaseAppModule.dll
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGUserAuditModule.dll
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGBaseAppModuleStatic.a
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGUserAuditModuleStatic.a
    echo.
    echo 编译模块:
    echo   - HGBaseAppModule
    echo   - HGUserAuditModule
) else (
    echo 生成的目标文件:
    if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: %MODULE_NAME%.dll
    if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: %MODULE_NAME%Static.a
    echo.
    echo 编译模块:
    echo   - %MODULE_NAME%
)
echo.

cd ..
endlocal