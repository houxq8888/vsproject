@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: 防止窗口自动关闭，在脚本结束时暂停
if not defined RUN_FROM_CMD (
    set "RUN_FROM_CMD=1"
    cmd /k "%~dpnx0"
    exit /b
)

:: Auto detect Qt installation path
echo Detecting Qt installation path...
set "QT_PATH="
set "MINGW_PATH="

:: User specified Qt path
set "USER_QT_PATH=C:\Qt\6.10.1\mingw_64"

:: Check user specified path first
if exist "%USER_QT_PATH%\bin\qmake.exe" (
    set "QT_PATH=%USER_QT_PATH%"
    goto :found_qt
)

:: Check common Qt installation paths
for %%d in (C D E F) do (
    if exist "%%d:\Qt\6.10.1\mingw_64\bin\qmake.exe" (
        set "QT_PATH=%%d:\Qt\6.10.1\mingw_64"
        goto :found_qt
    )
    if exist "%%d:\Qt\6.9.1\mingw_64\bin\qmake.exe" (
        set "QT_PATH=%%d:\Qt\6.9.1\mingw_64"
        goto :found_qt
    )
    if exist "%%d:\Qt\6.7.0\mingw_64\bin\qmake.exe" (
        set "QT_PATH=%%d:\Qt\6.7.0\mingw_64"
        goto :found_qt
    )
    if exist "%%d:\Qt\6.6.0\mingw_64\bin\qmake.exe" (
        set "QT_PATH=%%d:\Qt\6.6.0\mingw_64"
        goto :found_qt
    )
)

:: If auto detection fails, prompt user to set manually
:not_found
echo Error: Qt installation path not found
echo.
echo Please set Qt path manually:
echo 1. Edit this file and modify QT_PATH variable
echo 2. Or set system environment variable QT_DIR
echo.
echo Example: set QT_PATH=D:\Qt\6.9.1\mingw_64
echo.
pause
exit /b 1

:found_qt
echo Found Qt path: %QT_PATH%

:: Detect Mingw path
if exist "%QT_PATH%\..\Tools\mingw1310_64\bin\gcc.exe" (
    set "MINGW_PATH=%QT_PATH%\..\Tools\mingw1310_64"
) else if exist "%QT_PATH%\..\Tools\mingw1120_64\bin\gcc.exe" (
    set "MINGW_PATH=%QT_PATH%\..\Tools\mingw1120_64"
) else if exist "%QT_PATH%\..\Tools\mingw810_64\bin\gcc.exe" (
    set "MINGW_PATH=%QT_PATH%\..\Tools\mingw810_64"
) else (
    echo Warning: Mingw toolchain not found, using default path
    set "MINGW_PATH=%QT_PATH%"
)

echo Using Mingw path: %MINGW_PATH%

:: Set environment variables
set "PATH=%MINGW_PATH%\bin;%QT_PATH%\bin;%PATH%"

:: Set build directory
set "BUILD_DIR=build"
set "SOURCE_DIR=%~dp0"

:: Clean build directory
echo Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%" 2>nul
)

:: Create build directory
mkdir "%BUILD_DIR%" 2>nul
cd "%BUILD_DIR%"

:: Show build information
echo ========================================
echo Building HG Project
echo Qt Path: %QT_PATH%
echo Mingw Path: %MINGW_PATH%
echo Build Directory: %BUILD_DIR%
echo Source Directory: %SOURCE_DIR%
echo ========================================

:: Get current directory as project root
set "PROJECT_ROOT=%SOURCE_DIR%"

:: Try to find CMake in common locations
set "CMAKE_PATH="
if exist "%QT_PATH%\..\Tools\CMake_64\bin\cmake.exe" (
    set "CMAKE_PATH=%QT_PATH%\..\Tools\CMake_64\bin\cmake.exe"
) else if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "CMAKE_PATH=C:\Program Files\CMake\bin\cmake.exe"
) else if exist "C:\Program Files (x86)\CMake\bin\cmake.exe" (
    set "CMAKE_PATH=C:\Program Files (x86)\CMake\bin\cmake.exe"
) else (
    where cmake >nul 2>&1
    if %errorlevel% equ 0 (
        set "CMAKE_PATH=cmake"
    )
)

if not defined CMAKE_PATH (
    echo Error: CMake not found! Please install CMake or add it to PATH.
    pause
    exit /b 1
)

echo Using CMake: %CMAKE_PATH%
echo Project Root: %PROJECT_ROOT%

:: Configure CMake
echo Configuring CMake...
"%CMAKE_PATH%" ^
    -G "MinGW Makefiles" ^
    -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
    -DCMAKE_C_COMPILER="%MINGW_PATH%\bin\gcc.exe" ^
    -DCMAKE_CXX_COMPILER="%MINGW_PATH%\bin\g++.exe" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DHG_PROJECT_ROOT_FS="%PROJECT_ROOT%" ^
    -DPLATFORM="win32" ^
    ".." > build_log.txt 2>&1

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    echo Last 20 lines of error:
    powershell -Command "Get-Content build_log.txt | Select-Object -Last 20"
    pause
    exit /b %errorlevel%
)

:: Build project
echo Building project...
"%MINGW_PATH%\bin\mingw32-make.exe" -j4 >> build_log.txt 2>&1

if %errorlevel% neq 0 (
    echo Build failed!
    echo Last 20 lines of error:
    powershell -Command "Get-Content build_log.txt | Select-Object -Last 20"
    pause
    exit /b %errorlevel%
)

:: Show build result
echo.
echo ========================================
echo Build completed successfully!
echo ========================================

:: Show generated executable files
echo Generated executable files:
dir *.exe /b

echo.
echo Build directory: %BUILD_DIR%
echo Build log: %BUILD_DIR%\build_log.txt
echo.

cd ..
pause
endlocal