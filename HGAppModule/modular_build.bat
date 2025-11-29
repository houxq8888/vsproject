@echo off
setlocal enabledelayedexpansion

REM Modular build script
set "PROJECT_ROOT=%~dp0"
set "BUILD_DIR=%PROJECT_ROOT%windows_build"

REM Available modules
set "AVAILABLE_MODULES=HGBaseAppModule HGChannelModule HGCurveModule HGEBalanceModule HGFlowModule HGLogModule HGLoginModule HGMethodModule HGPrintModule HGReagentModule HGScannerModule HGSearchModule HGSharedFileModule HGTaskModule HGUartModule HGUserAuditModule"

REM Default modules
set "DEFAULT_MODULES=HGBaseAppModule HGLogModule HGLoginModule HGTaskModule"

REM Parameters
set "CLEAN_BUILD=0"
set "JOBS=4"
set "DEBUG_BUILD=0"
set "SHOW_HELP=0"
set "LIST_MODULES=0"
set "MODULE_LIST=%DEFAULT_MODULES%"

REM Parse arguments
:parse_args
if "%~1"=="" goto args_parsed

if "%~1"=="--help" (
    set "SHOW_HELP=1"
    shift
    goto parse_args
)

if "%~1"=="--list-modules" (
    set "LIST_MODULES=1"
    shift
    goto parse_args
)

if "%~1"=="--clean" (
    set "CLEAN_BUILD=1"
    shift
    goto parse_args
)

if "%~1"=="--jobs" (
    if not "%~2"=="" (
        set "JOBS=%~2"
        shift
    )
    shift
    goto parse_args
)

if "%~1"=="--debug" (
    set "DEBUG_BUILD=1"
    shift
    goto parse_args
)

if "%~1"=="--modules" (
    if not "%~2"=="" (
        set "MODULE_LIST=%~2"
        shift
    )
    shift
    goto parse_args
)

shift
goto parse_args

:args_parsed

REM Show help
if "%SHOW_HELP%"=="1" (
    echo.
    echo Modular Build Script
    echo.
    echo Usage: modular_build.bat [options]
    echo.
    echo Options:
    echo   --modules ^<modules^>   Specify modules
    echo   --clean               Clean build
    echo   --jobs ^<number^>      Parallel jobs
    echo   --debug              Debug mode
    echo   --help               Show help
    echo   --list-modules       List modules
    echo.
    echo Examples:
    echo   modular_build.bat --modules HGLogModule,HGTaskModule --clean
    echo   modular_build.bat --list-modules
    echo.
    goto :eof
)

REM List modules
if "%LIST_MODULES%"=="1" (
    echo.
    echo Available modules:
    for %%m in (%AVAILABLE_MODULES%) do echo   %%m
    echo.
    echo Default modules:
    for %%m in (%DEFAULT_MODULES%) do echo   %%m
    echo.
    goto :eof
)

REM Convert comma-separated modules to space-separated
set "MODULE_LIST_SPACE=%MODULE_LIST:,= %"

REM Validate modules
set "VALID_MODULES="
set "INVALID_MODULES="

for %%m in (%MODULE_LIST_SPACE%) do (
    set "FOUND=0"
    for %%a in (%AVAILABLE_MODULES%) do (
        if "%%m"=="%%a" set "FOUND=1"
    )
    if "!FOUND!"=="1" (
        if "!VALID_MODULES!"=="" (
            set "VALID_MODULES=%%m"
        ) else (
            set "VALID_MODULES=!VALID_MODULES! %%m"
        )
    ) else (
        if "!INVALID_MODULES!"=="" (
            set "INVALID_MODULES=%%m"
        ) else (
            set "INVALID_MODULES=!INVALID_MODULES! %%m"
        )
    )
)

if not "%INVALID_MODULES%"=="" (
    echo Error: Invalid modules: %INVALID_MODULES%
    echo Available modules: %AVAILABLE_MODULES%
    goto :eof
)

REM Show info
echo.
echo ===== Modular Build =====
echo Project: %PROJECT_ROOT%
echo Build: %BUILD_DIR%
echo Modules: %VALID_MODULES%
echo Jobs: %JOBS%
if "%DEBUG_BUILD%"=="1" echo Mode: Debug
if "%CLEAN_BUILD%"=="1" echo Clean: Yes
echo.

REM Clean
if "%CLEAN_BUILD%"=="1" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
    )
    echo Clean completed
    echo.
)

REM Create directory
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

REM Create a temporary CMakeLists.txt for modular build
echo Creating modular build configuration...
(
echo # Temporary modular build configuration
echo cmake_minimum_required(VERSION 3.16)
echo project(HGAppModule_Modular)
echo.
echo # Set project root
echo set(HG_PROJECT_ROOT_FS %PROJECT_ROOT%../)
echo set(PLATFORM win32)
echo.
echo # Set C++ standard
echo set(CMAKE_CXX_STANDARD 17)
echo set(CMAKE_CXX_STANDARD_REQUIRED ON)
echo.
echo # Include project macros
echo include(%PROJECT_ROOT%../cmake/project_macro.cmake)
echo.
echo # Include directories
echo include_directories(
echo     %PROJECT_ROOT%
echo     %PROJECT_ROOT%../3rdparty/include
echo     %PROJECT_ROOT%../3rdparty/include/opencv4
echo     %PROJECT_ROOT%../services/HGAtomService/HGSaveService/inc
echo     %PROJECT_ROOT%../services/HGAtomService/HGFrameService/inc
echo     %PROJECT_ROOT%../services/HGAtomService/HGSecurityService/inc
echo     %PROJECT_ROOT%../services/HGAtomService/HGCommunicateService/inc
echo     %PROJECT_ROOT%../services/HGAtomService/HGAuthorityService/inc
echo     %PROJECT_ROOT%HGBaseAppModule/inc
echo     %PROJECT_ROOT%pluginInterface/inc
echo     %PROJECT_ROOT%HGSearchModule/inc
echo     %PROJECT_ROOT%HGUartModule/inc
echo     %PROJECT_ROOT%HGPrintModule/inc
echo     %PROJECT_ROOT%HGCurveModule/inc
echo     %PROJECT_ROOT%../services/HGHardware/HGLowerPC/inc
echo     %PROJECT_ROOT%../services/HGCommon/inc
echo     %PROJECT_ROOT%../services/HGConfig/inc
echo     %PROJECT_ROOT%../pluginInterface/inc
echo )
echo.
echo # Link directories
echo link_directories(%PROJECT_ROOT%../3rdparty/lib)
echo.
echo # Find Qt6
echo find_package(Qt6 COMPONENTS Core Widgets Gui Charts LinguistTools PrintSupport Designer UiPlugin Qml REQUIRED)
echo.
echo # Link Qt libraries
echo link_libraries(
echo     Qt6::Core
echo     Qt6::Widgets
echo     Qt6::Gui
echo     Qt6::Charts
echo     Qt6::PrintSupport
echo     Qt6::Designer
echo     Qt6::UiPlugin
echo     Qt6::Qml
echo )
echo.
echo # Add selected modules
echo message(STATUS "Building modules: %MODULE_LIST_SPACE%")
) > "%BUILD_DIR%\CMakeLists.txt"

REM Add each selected module to the temporary CMakeLists.txt
for %%m in (%MODULE_LIST_SPACE%) do (
    echo add_subdirectory(%%m) >> "%BUILD_DIR%\CMakeLists.txt"
)

REM Set Qt installation path
set QT_DIR=D:\Qt\6.9.1\mingw_64
echo Qt路径: %QT_DIR%

REM Configure MinGW environment
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
echo MinGW路径: %MINGW_DIR%
set PATH=%MINGW_DIR%\bin;%PATH%

REM CMake configuration
echo Configuring CMake...
cd /d "%BUILD_DIR%"

if "%DEBUG_BUILD%"=="1" (
    cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%" .
) else (
    cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%" .
)

if errorlevel 1 (
    echo CMake configuration failed
    goto :eof
)

REM Build
echo.
echo Starting build...
if "%DEBUG_BUILD%"=="1" (
    cmake --build . --config Debug --parallel %JOBS%
) else (
    cmake --build . --config Release --parallel %JOBS%
)

if errorlevel 1 (
    echo Build failed
    goto :eof
)

REM Complete
echo.
echo ===== Build Complete =====
echo Modules: %VALID_MODULES%
if "%DEBUG_BUILD%"=="1" (
    echo Type: Debug
) else (
    echo Type: Release
)
echo.

echo Generated files:
for /r "%BUILD_DIR%" %%f in (*.exe) do (
    echo   %%~nxf
)

echo.
echo Build successful!

endlocal