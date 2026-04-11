# HGLogModule Build Script
$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "HGLogModule Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Set paths
$QT_DIR = "C:\Qt\6.9.3\mingw_64"
$MINGW_DIR = "C:\Qt\Tools\mingw1310_64"
$CMAKE_PATH = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$PROJECT_ROOT = "c:\Users\liguitao\myjob\vsproject"
$HGAPP_DIR = Join-Path $PROJECT_ROOT "HGAppModule"
$BUILD_DIR = Join-Path $HGAPP_DIR "windows_build"

Write-Host "Qt Path: $QT_DIR" -ForegroundColor Green
Write-Host "MinGW Path: $MINGW_DIR" -ForegroundColor Green
Write-Host "CMake Path: $CMAKE_PATH" -ForegroundColor Green
Write-Host "Project Root: $PROJECT_ROOT" -ForegroundColor Green
Write-Host "HGAppModule Dir: $HGAPP_DIR" -ForegroundColor Green
Write-Host "Build Dir: $BUILD_DIR" -ForegroundColor Green
Write-Host ""

# Validate paths
if (-not (Test-Path $QT_DIR)) {
    Write-Host "ERROR: Qt path not found: $QT_DIR" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $MINGW_DIR)) {
    Write-Host "ERROR: MinGW path not found: $MINGW_DIR" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $CMAKE_PATH)) {
    Write-Host "ERROR: CMake path not found: $CMAKE_PATH" -ForegroundColor Red
    exit 1
}

# Set environment variables
$env:PATH = "$MINGW_DIR\bin;$QT_DIR\bin;$env:PATH"
Write-Host "Environment variables updated" -ForegroundColor Yellow
Write-Host ""

# Clean and create build directory
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
    "-DCMAKE_PREFIX_PATH=$QT_DIR",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DHG_PROJECT_ROOT_FS=$PROJECT_ROOT",
    "-DPLATFORM=win32",
    "-DBUILD_HGLOGMODULE_ONLY=ON",
    "-DBUILD_EXECUTABLE=ON",
    "-DBUILD_SHARED_LIBS=OFF",
    "-DBUILD_STATIC_LIBS=ON",
    "-DBUILD_HGBASEAPP_EXECUTABLE=OFF",
    $HGAPP_DIR
)

Write-Host "Running: cmake $cmakeArgs" -ForegroundColor Gray
Write-Host ""

try {
    & $CMAKE_PATH @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: CMake configuration failed, exit code: $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: CMake configuration failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "CMake configuration complete!" -ForegroundColor Green
Write-Host ""

# Build project
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Building project..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$mingwMake = Join-Path $MINGW_DIR "bin\mingw32-make.exe"

try {
    Write-Host "Building targets: HGLogModuleStatic HGLogModuleRun" -ForegroundColor Yellow
    Write-Host ""
    & $mingwMake -j4 HGLogModuleStatic HGLogModuleRun
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: Build failed, exit code: $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: Build failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "HGLogModule build successful!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# Find and list generated files
Write-Host "Generated files:" -ForegroundColor Yellow
$exePath = Join-Path $BUILD_DIR "deploy\HGLogModuleRun.exe"
if (Test-Path $exePath) {
    Write-Host "  - Executable: $exePath" -ForegroundColor White
}

$staticLibPath = Join-Path $BUILD_DIR "HGLogModule\libHGLogModuleStatic.a"
if (Test-Path $staticLibPath) {
    Write-Host "  - Static library: $staticLibPath" -ForegroundColor White
}

Write-Host ""
Write-Host "Build directory: $BUILD_DIR" -ForegroundColor Cyan
Write-Host ""
