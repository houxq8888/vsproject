@echo off
setlocal enabledelayedexpansion

REM Set Qt installation path
set QT_DIR=D:\Qt\6.9.1\mingw_64
echo Debug: QT_DIR = %QT_DIR%

REM Configure MinGW environment
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
echo Debug: MINGW_DIR = %MINGW_DIR%
set PATH=%MINGW_DIR%\bin;%PATH%

REM Create test build directory
set BUILD_DIR=test_build
if exist "%BUILD_DIR%" (
    echo Debug: Removing existing test build directory...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
echo Debug: Current directory is %CD%

REM Run CMake configuration for test program
echo Debug: Running CMake configuration for test program...
"D:\Qt\Tools\CMake_64\bin\cmake.exe" -G "MinGW Makefiles" ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
  -DCMAKE_BUILD_TYPE=Release ^
  ..\test_qt6

if %errorlevel% neq 0 (
    echo CMake configuration failed
    exit /b 1
)

REM Build test program
echo Debug: Building test program...
mingw32-make

if %errorlevel% neq 0 (
    echo Build failed
    exit /b 1
)

echo ===============================================
echo Test build successful! Qt6 environment is working.
echo ===============================================

REM Run test program
echo Debug: Running test program...
HGAppModuleTest.exe

echo Debug: Test program returned %errorlevel%

cd ..
endlocal