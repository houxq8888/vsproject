@echo off
setlocal enabledelayedexpansion

REM Set Qt installation path
set QT_DIR=D:\Qt\6.9.1\mingw_64
echo Debug: QT_DIR = %QT_DIR%

REM Configure MinGW environment
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
echo Debug: MINGW_DIR = %MINGW_DIR%
set PATH=%MINGW_DIR%\bin;%PATH%

REM Create build directory
set BUILD_DIR=windows_build
if exist "%BUILD_DIR%" (
    echo Debug: Removing existing build directory...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
echo Debug: Current directory is %CD%

REM Run CMake configuration for Windows HGAppModule build
echo Debug: Running CMake configuration for Windows HGAppModule build...
"D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DHG_PROJECT_ROOT_FS="d:\virtualMachine\github\vsproject" ^
  -DPLATFORM="win32" ^
  -DBUILD_HGAPPMODULE_ONLY=ON ^
  ..\..

if %errorlevel% neq 0 (
    echo CMake configuration failed
    exit /b 1
)

REM Build the project
echo Debug: Building HGAppModule...
mingw32-make -j4

if %errorlevel% neq 0 (
    echo Build failed
    exit /b 1
)

echo ===============================================
echo HGAppModule Windows build successful!
echo ===============================================

cd ..
endlocal