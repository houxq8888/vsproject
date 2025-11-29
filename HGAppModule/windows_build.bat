@echo off
setlocal enabledelayedexpansion

REM 设置编码解决中文乱码问题
chcp 65001 >nul

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

if /i "%~1"=="--help" (
    goto :show_help
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
echo   --help         显示此帮助信息
echo.
echo 示例:
echo   windows_build.bat --exe --no-shared --no-static
echo   windows_build.bat --static --no-exe --no-shared
echo   windows_build.bat --debug --jobs 8
echo.
exit /b 1

:args_done

REM 显示编译配置
echo ===============================================
echo Windows HGAppModule 编译配置
echo ===============================================
echo 编译模式: %BUILD_TYPE%
echo 可执行程序: %BUILD_EXECUTABLE%
echo 动态库: %BUILD_SHARED_LIBS%
echo 静态库: %BUILD_STATIC_LIBS%
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
if exist "%BUILD_DIR%" (
    echo 删除现有构建目录...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
echo 当前目录: %CD%

REM Run CMake configuration for Windows HGAppModule build
echo 运行CMake配置...
"D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DHG_PROJECT_ROOT_FS="d:\virtualMachine\github\vsproject" ^
  -DPLATFORM="win32" ^
  -DBUILD_HGAPPMODULE_ONLY=ON ^
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
echo 编译HGAppModule...
echo 编译日志将保存到: build_log.txt

REM 编译并将输出重定向到文件
mingw32-make -j%JOB_COUNT% > build_log.txt 2>&1

REM 检查编译结果
if %errorlevel% neq 0 (
    echo 编译失败，请查看 build_log.txt 文件获取详细错误信息
    echo 最后10行错误信息:
    powershell -Command "Get-Content 'build_log.txt' | Select-Object -Last 10"
    exit /b 1
) else (
    echo 编译成功，完整日志已保存到 build_log.txt
)

echo ===============================================
echo HGBaseAppModule Windows编译成功!
echo ===============================================
echo.
echo 生成的目标文件:
if "%BUILD_SHARED_LIBS%"=="ON" echo   - 动态库: HGBaseAppModule.dll
if "%BUILD_STATIC_LIBS%"=="ON" echo   - 静态库: HGBaseAppModuleStatic.a
echo.
echo 编译模块:
echo   - HGBaseAppModule (仅静态库/动态库)
echo.

cd ..
endlocal