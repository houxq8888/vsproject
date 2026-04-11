@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo Building HG Project (Simple)
echo ========================================

:: 设置 Qt 和 MinGW 路径
set QT_PATH=C:\Qt\6.9.3\mingw_64
set MINGW_PATH=C:\Qt\Tools\mingw1310_64

echo Qt Path: %QT_PATH%
echo Mingw Path: %MINGW_PATH%

:: 设置环境变量
set PATH=%MINGW_PATH%\bin;%QT_PATH%\bin;%PATH%

:: 设置构建目录
set BUILD_DIR=build_win32
set SOURCE_DIR=.

:: 清理并创建构建目录
echo Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

:: 配置 CMake
echo Configuring CMake...
"C:\Qt\Tools\CMake_64\bin\cmake.exe" ^
    -G "MinGW Makefiles" ^
    -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
    -DCMAKE_C_COMPILER="%MINGW_PATH%\bin\gcc.exe" ^
    -DCMAKE_CXX_COMPILER="%MINGW_PATH%\bin\g++.exe" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DHG_PROJECT_ROOT_FS="c:\Users\liguitao\myjob\vsproject" ^
    -DPLATFORM="win32" ^
    "%SOURCE_DIR%"

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b %errorlevel%
)

:: 编译项目
echo Building project...
"%MINGW_PATH%\bin\mingw32-make.exe" -j4

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo Build directory: %CD%
echo.

cd ..
endlocal
