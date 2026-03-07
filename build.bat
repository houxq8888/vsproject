@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: Set Qt installation path manually
echo Setting Qt installation path...
set "QT_PATH=E:\program\Qt\Qt5.14.2\5.14.2\mingw73_32"
set "MINGW_PATH=E:\program\Qt\Qt5.14.2\Tools\mingw730_32"

:found_qt
echo Found Qt path: %QT_PATH%

:: Set Mingw path manually
echo Setting Mingw path...
set "MINGW_PATH=E:\program\Qt\Qt5.14.2\Tools\mingw730_32"

echo Using Mingw path: %MINGW_PATH%

:: Set environment variables
set "PATH=%MINGW_PATH%\bin;%QT_PATH%\bin;%PATH%"
set "CMAKE_MAKE_PROGRAM=%MINGW_PATH%\bin\mingw32-make.exe"

:: Set build directory
set "BUILD_DIR=build"
set "SOURCE_DIR=."

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
echo ========================================

:: Configure CMake
echo Configuring CMake...
cmake.exe ^
    -G "MinGW Makefiles" ^
    -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
    -DCMAKE_C_COMPILER="%MINGW_PATH%\bin\gcc.exe" ^
    -DCMAKE_CXX_COMPILER="%MINGW_PATH%\bin\g++.exe" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DHG_PROJECT_ROOT_FS="d:/project/code-model-compare" ^
    -DPLATFORM="win32" ^
    "%SOURCE_DIR%\.." > build_log.txt 2>&1

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    echo Last 20 lines of error:
    powershell -Command "Get-Content build_log.txt | Select-Object -Last 20"
    exit /b %errorlevel%
)

:: Build project
echo Building project...
"%MINGW_PATH%\bin\mingw32-make.exe" -j4 LogModule >> build_log.txt 2>&1

if %errorlevel% neq 0 (
    echo Build failed!
    echo Last 20 lines of error:
    powershell -Command "Get-Content build_log.txt | Select-Object -Last 20"
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
endlocal