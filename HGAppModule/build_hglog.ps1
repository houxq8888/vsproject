# HGLogModule 编译脚本
$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "HGLogModule 编译脚本" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 设置路径
$QT_DIR = "C:\Qt\6.9.3\mingw_64"
$MINGW_DIR = "C:\Qt\Tools\mingw1310_64"
$CMAKE_PATH = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$PROJECT_ROOT = "c:\Users\liguitao\myjob\vsproject"
$HGAPP_DIR = Join-Path $PROJECT_ROOT "HGAppModule"
$BUILD_DIR = Join-Path $HGAPP_DIR "windows_build"

Write-Host "Qt 路径: $QT_DIR" -ForegroundColor Green
Write-Host "MinGW 路径: $MINGW_DIR" -ForegroundColor Green
Write-Host "CMake 路径: $CMAKE_PATH" -ForegroundColor Green
Write-Host "项目根目录: $PROJECT_ROOT" -ForegroundColor Green
Write-Host "HGAppModule 目录: $HGAPP_DIR" -ForegroundColor Green
Write-Host "构建目录: $BUILD_DIR" -ForegroundColor Green
Write-Host ""

# 验证路径
if (-not (Test-Path $QT_DIR)) {
    Write-Host "ERROR: Qt 路径不存在: $QT_DIR" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $MINGW_DIR)) {
    Write-Host "ERROR: MinGW 路径不存在: $MINGW_DIR" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $CMAKE_PATH)) {
    Write-Host "ERROR: CMake 路径不存在: $CMAKE_PATH" -ForegroundColor Red
    exit 1
}

# 设置环境变量
$env:PATH = "$MINGW_DIR\bin;$QT_DIR\bin;$env:PATH"
Write-Host "环境变量已更新" -ForegroundColor Yellow
Write-Host ""

# 清理并创建构建目录
if (Test-Path $BUILD_DIR) {
    Write-Host "清理现有的构建目录..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BUILD_DIR -ErrorAction SilentlyContinue
}

Write-Host "创建构建目录..." -ForegroundColor Yellow
New-Item -ItemType Directory -Path $BUILD_DIR -Force | Out-Null
Set-Location $BUILD_DIR
Write-Host ""

# 配置 CMake
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "配置 CMake..." -ForegroundColor Cyan
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

Write-Host "运行: cmake $cmakeArgs" -ForegroundColor Gray
Write-Host ""

try {
    & $CMAKE_PATH @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: CMake 配置失败，退出码: $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: CMake 配置失败: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "CMake 配置完成!" -ForegroundColor Green
Write-Host ""

# 编译项目
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "编译项目..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$mingwMake = Join-Path $MINGW_DIR "bin\mingw32-make.exe"

try {
    Write-Host "编译目标: HGLogModuleStatic HGLogModuleRun" -ForegroundColor Yellow
    Write-Host ""
    & $mingwMake -j4 HGLogModuleStatic HGLogModuleRun
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "ERROR: 编译失败，退出码: $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "ERROR: 编译失败: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "HGLogModule 编译成功!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# 查找并列出生成的文件
Write-Host "生成的文件:" -ForegroundColor Yellow
$exePath = Join-Path $BUILD_DIR "deploy\HGLogModuleRun.exe"
if (Test-Path $exePath) {
    Write-Host "  - 可执行文件: $exePath" -ForegroundColor White
}

$staticLibPath = Join-Path $BUILD_DIR "HGLogModule\libHGLogModuleStatic.a"
if (Test-Path $staticLibPath) {
    Write-Host "  - 静态库: $staticLibPath" -ForegroundColor White
}

Write-Host ""
Write-Host "构建目录: $BUILD_DIR" -ForegroundColor Cyan
Write-Host ""
