# 简化的模块化编译脚本
param(
    [string]$Modules = "HGBaseAppModule,HGTaskModule"
)

# 设置路径
$ProjectRoot = $PSScriptRoot
$BuildDir = "$ProjectRoot\windows_build"

# 设置Qt和MinGW路径
$QtDir = "D:\Qt\6.9.1\mingw_64"
$MingwDir = "D:\Qt\Tools\mingw1310_64"
$env:PATH = "$MingwDir\bin;" + $env:PATH

# 解析模块
$ModuleArray = $Modules.Split(",") | ForEach-Object { $_.Trim() }

# 显示信息
Write-Host "===== 模块化编译 =====" -ForegroundColor Cyan
Write-Host "项目: $ProjectRoot"
Write-Host "构建目录: $BuildDir"
Write-Host "模块: $($ModuleArray -join ' ')"
Write-Host ""

# 清理构建目录
if (Test-Path $BuildDir) {
    Write-Host "清理构建目录..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
}

# 创建构建目录
New-Item -ItemType Directory -Path $BuildDir | Out-Null

# 创建简单的CMakeLists.txt
Write-Host "创建模块化编译配置..." -ForegroundColor Yellow

# 使用简单的字符串构建方法
$CmakeContent = "cmake_minimum_required(VERSION 3.16)`n"
$CmakeContent += "project(HGAppModule_Modular)`n`n"
$CmakeContent += "set(HG_PROJECT_ROOT_FS $ProjectRoot\\..)`n"
$CmakeContent += "set(PLATFORM win32)`n`n"
$CmakeContent += "set(CMAKE_CXX_STANDARD 17)`n"
$CmakeContent += "set(CMAKE_CXX_STANDARD_REQUIRED ON)`n`n"
$CmakeContent += "include_directories(`n"
$CmakeContent += "    $ProjectRoot`n"
$CmakeContent += "    $ProjectRoot\\..\\3rdparty\\include`n"
$CmakeContent += "    $ProjectRoot\\..\\3rdparty\\include\\opencv4`n"
$CmakeContent += "    $ProjectRoot\\HGBaseAppModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\pluginInterface\\inc`n"
$CmakeContent += ")`n`n"
$CmakeContent += "link_directories($ProjectRoot\\..\\3rdparty\\lib)`n`n"
$CmakeContent += "find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)`n`n"
$CmakeContent += "link_libraries(`n"
$CmakeContent += "    Qt6::Core`n"
$CmakeContent += "    Qt6::Widgets`n"
$CmakeContent += "    Qt6::Gui`n"
$CmakeContent += ")`n`n"
$CmakeContent += "message(STATUS Building modules: $($ModuleArray -join ' '))`n`n"

# 添加模块
foreach ($module in $ModuleArray) {
    $CmakeContent += "add_subdirectory($module)`n"
}

$CmakeContent | Out-File -FilePath "$BuildDir\CMakeLists.txt" -Encoding ASCII

# 配置CMake
Write-Host "配置CMake..." -ForegroundColor Yellow
Set-Location $BuildDir

$CmakeArgs = @(
    "-G", "MinGW Makefiles",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=`"$QtDir`"",
    "."
)

$CmakeResult = & cmake @CmakeArgs 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake配置失败!" -ForegroundColor Red
    Write-Host $CmakeResult
    exit 1
}

# 编译
Write-Host "开始编译..." -ForegroundColor Yellow
$MakeResult = & mingw32-make -j4 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败!" -ForegroundColor Red
    Write-Host $MakeResult
    exit 1
}

Write-Host ""
Write-Host "===== 编译完成 =====" -ForegroundColor Green
Write-Host "模块: $($ModuleArray -join ' ')"
Write-Host ""
Write-Host "构建成功!" -ForegroundColor Green