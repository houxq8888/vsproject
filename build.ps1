# PowerShell build script
$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Building HG Project (PowerShell)" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Set paths
$QT_PATH = "C:\Qt\6.9.3\mingw_64"
$MINGW_PATH = "C:\Qt\Tools\mingw1310_64"
$CMAKE_PATH = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$PROJECT_ROOT = "c:\Users\liguitao\myjob\vsproject"

Write-Host "Qt Path: $QT_PATH" -ForegroundColor Green
Write-Host "Mingw Path: $MINGW_PATH" -ForegroundColor Green
Write-Host "CMake Path: $CMAKE_PATH" -ForegroundColor Green
Write-Host "Project Root: $PROJECT_ROOT" -ForegroundColor Green
Write-Host ""

# Validate paths
if (-not (Test-Path $QT_PATH)) {
    Write-Host "ERROR: Qt path not found: $QT_PATH" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $MINGW_PATH)) {
    Write-Host "ERROR: Mingw path not found: $MINGW_PATH" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $CMAKE_PATH)) {
    Write-Host "ERROR: CMake path not found: $CMAKE_PATH" -ForegroundColor Red
    exit 1
}

# Set environment variables
$env:PATH = "$MINGW_PATH\bin;$QT_PATH\bin;$env:PATH"
Write-Host "Environment variables updated" -ForegroundColor Yellow
Write-Host ""

# Clean and create build directory
$BUILD_DIR = Join-Path $PROJECT_ROOT "build_hglog"
Write-Host "Build directory: $BUILD_DIR" -ForegroundColor Yellow

if (Test-Path $BUILD_DIR) {
    Write-Host "Cleaning existing build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BUILD_DIR -ErrorAction SilentlyContinue
}

Write-Host "Creating build directory..." -ForegroundColor Yellow
New-Item -ItemType Directory -Path $BUILD_DIR -Force | Out-Null
Set-Location $BUILD_DIR
Write-Host ""

# Configure CMake
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Configuring CMake..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$cmakeArgs = @(
    "-G", "MinGW Makefiles",
    "-DCMAKE_PREFIX_PATH=$QT_PATH",
    "-DCMAKE_C_COMPILER=$MINGW_PATH\bin\gcc.exe",
    "-DCMAKE_CXX_COMPILER=$MINGW_PATH\bin\g++.exe",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DHG_PROJECT_ROOT_FS=$PROJECT_ROOT",
    "-DPLATFORM=win32",
    $PROJECT_ROOT
)

Write-Host "Running: cmake $cmakeArgs" -ForegroundColor Gray
Write-Host ""

try {
    & $CMAKE_PATH @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: CMake configuration failed with exit code $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: CMake configuration failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "CMake configuration completed successfully!" -ForegroundColor Green
Write-Host ""

# Build project
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Building project..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$mingwMake = Join-Path $MINGW_PATH "bin\mingw32-make.exe"

try {
    & $mingwMake -j4
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: Build failed with exit code $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: Build failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# Find and list executable files
Write-Host "Generated executable files:" -ForegroundColor Yellow
Get-ChildItem -Path $BUILD_DIR -Filter "*.exe" -Recurse | ForEach-Object {
    Write-Host "  - $($_.FullName)" -ForegroundColor White
}

Write-Host ""
Write-Host "Build directory: $BUILD_DIR" -ForegroundColor Cyan
Write-Host ""
