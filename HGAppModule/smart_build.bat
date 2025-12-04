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

REM 设置模块编译选项
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
echo 智能模块编译脚本
echo.
echo 用法: smart_build.bat [选项]
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
echo   - 输入任意模块文件夹名称即可编译
echo   - 例如: --module HGUserAuditModule
echo   - 例如: --module HGBaseAppModule
echo   - 例如: --module HGBaseAppModule,HGUserAuditModule
echo.
echo 示例:
echo   smart_build.bat --exe --no-shared --no-static
echo   smart_build.bat --static --no-exe --no-shared
echo   smart_build.bat --debug --jobs 8
echo   smart_build.bat --module HGUserAuditModule --incremental
echo   smart_build.bat --module HGBaseAppModule --clean
echo   smart_build.bat --module HGBaseAppModule --module HGUserAuditModule --incremental
echo.
exit /b 1

:args_done

REM 显示编译配置
echo ===============================================
echo 智能模块编译配置
echo ===============================================
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
set BUILD_DIR=smart_build

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

REM 智能模块检测和编译函数
call :detect_and_build_modules
if %errorlevel% neq 0 (
    echo 模块检测和编译失败
    exit /b 1
)

echo.
echo ===============================================
echo 智能编译完成!
echo ===============================================
echo.

cd ..
endlocal
exit /b 0

REM 智能模块检测和编译函数
:detect_and_build_modules
    echo 开始检测和编译模块...
    echo.
    
    REM 检查是否编译所有模块
    if "%BUILD_MODULE%"=="ALL" (
        echo 编译所有模块...
        set CMAKE_MODULE_ARGS=-DBUILD_HGAPPMODULE_ONLY=ON
        set MAKE_TARGET=all
        set MODULE_NAME=所有HGAppModule模块
    ) else (
        REM 处理多个模块的情况
        set MODULE_LIST=%BUILD_MODULE%
        set CMAKE_MODULE_ARGS=
        set MAKE_TARGET=
        set MODULE_NAME=
        
        REM 分割模块列表
        set "MODULE_LIST=%MODULE_LIST:,=,%"
        
        :parse_module_list
        for /f "tokens=1* delims=," %%a in ("%MODULE_LIST%") do (
            set CURRENT_MODULE=%%a
            set MODULE_LIST=%%b
            
            REM 检查模块文件夹是否存在
            if exist "..\%CURRENT_MODULE%" (
                if exist "..\%CURRENT_MODULE%\CMakeLists.txt" (
                    echo 检测到模块: %CURRENT_MODULE%
                    
                    REM 构建CMake参数
                    if "%CMAKE_MODULE_ARGS%"=="" (
                        set CMAKE_MODULE_ARGS=-DBUILD_%CURRENT_MODULE%_ONLY=ON
                    ) else (
                        set CMAKE_MODULE_ARGS=%CMAKE_MODULE_ARGS% -DBUILD_%CURRENT_MODULE%_ONLY=ON
                    )
                    
                    REM 构建make目标
                    if "%MAKE_TARGET%"=="" (
                        set MAKE_TARGET=%CURRENT_MODULE%
                    ) else (
                        set MAKE_TARGET=%MAKE_TARGET% %CURRENT_MODULE%
                    )
                    
                    REM 构建模块名称显示
                    if "%MODULE_NAME%"=="" (
                        set MODULE_NAME=%CURRENT_MODULE%
                    ) else (
                        set MODULE_NAME=%MODULE_NAME%, %CURRENT_MODULE%
                    )
                ) else (
                    echo 警告: 模块 %CURRENT_MODULE% 不存在CMakeLists.txt文件，跳过
                )
            ) else (
                echo 错误: 模块文件夹 %CURRENT_MODULE% 不存在
                exit /b 1
            )
            
            if not "!MODULE_LIST!"=="" (
                set MODULE_LIST=!MODULE_LIST!
                goto :parse_module_list
            )
        )
    )
    
    if "%CMAKE_MODULE_ARGS%"=="" (
        echo 错误: 没有找到有效的模块进行编译
        exit /b 1
    )
    
    echo.
    echo 编译模块: %MODULE_NAME%
    echo CMake参数: %CMAKE_MODULE_ARGS%
    echo.
    
    REM Run CMake configuration
    echo 运行CMake配置...
    "D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" ^
      -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
      -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
      -DHG_PROJECT_ROOT_FS="d:\virtualMachine\github\vsproject" ^
      -DPLATFORM="win32" ^
      %CMAKE_MODULE_ARGS% ^
      -DBUILD_EXECUTABLE=%BUILD_EXECUTABLE% ^
      -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS% ^
      -DBUILD_STATIC_LIBS=%BUILD_STATIC_LIBS% ^
      ..
    
    if %errorlevel% neq 0 (
        echo CMake配置失败
        exit /b 1
    )
    
    REM Build the project
    echo 开始编译...
    echo 编译日志将保存到: build_log.txt
    
    mingw32-make -j%JOB_COUNT% > build_log.txt 2>&1
    if %errorlevel% neq 0 (
        echo 编译失败，请查看 build_log.txt 文件获取详细错误信息
        echo 最后10行错误信息:
        powershell -Command "Get-Content 'build_log.txt' | Select-Object -Last 10"
        exit /b 1
    )
    
    echo 编译成功，完整日志已保存到 build_log.txt
    
    REM 显示编译结果
    echo.
    echo ===============================================
    echo %MODULE_NAME% 编译成功!
    echo ===============================================
    echo.
    
    REM 显示生成的目标文件
    echo 生成的目标文件:
    
    REM 智能检测生成的文件
    for /f "delims=" %%f in ('dir /b /s *.dll *.a *.exe 2^>nul') do (
        echo   - %%f
    )
    
    echo.
    
    goto :eof