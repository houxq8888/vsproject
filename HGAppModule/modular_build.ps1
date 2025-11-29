# 模块化编译脚本 - PowerShell版本
# 支持选择性编译特定模块

param(
    [string[]]$Modules = @(),
    [switch]$Clean,
    [int]$Jobs = 0,
    [switch]$Debug,
    [switch]$Help,
    [switch]$ListModules
)

# 设置变量
$ProjectRoot = $PSScriptRoot
$BuildDir = "$ProjectRoot\windows_build"

# 所有可用模块列表
$AllModules = "HGBaseAppModule pluginInterface HGSharedFileModule HGAnalysisRecordModule HGSearchModule HGCurveModule HGPrintModule HGUartModule HGEBalanceModule HGLoginModule HGLogModule HGExceptionHandleModule HGScannerModule HGCameraRecognizeModule HGMethodModule HGTaskModule HGFlowModule HGChannelModule HGReagentModule HGUserAuditModule"

# 默认模块（基础模块）
$DefaultModules = "HGBaseAppModule pluginInterface HGSharedFileModule HGAnalysisRecordModule"

# 显示帮助信息
if ($Help) {
    Write-Host "模块化编译脚本使用说明" -ForegroundColor Green
    Write-Host ""
    Write-Host "用法: modular_build.ps1 [-Modules 模块列表] [-Clean] [-Jobs N] [-Debug] [-Help] [-ListModules]"
    Write-Host ""
    Write-Host "参数:"
    Write-Host "  -Modules         - 要编译的模块列表（逗号分隔或数组）"
    Write-Host "  -Clean           - 清理构建目录"
    Write-Host "  -Jobs            - 并行任务数（默认：自动检测CPU核心数）"
    Write-Host "  -Debug           - 使用Debug模式编译"
    Write-Host "  -Help            - 显示此帮助信息"
    Write-Host "  -ListModules     - 列出所有可用模块"
    Write-Host ""
    Write-Host "示例:"
    Write-Host "  modular_build.ps1 -Modules HGSearchModule,HGCurveModule"
    Write-Host "  modular_build.ps1 -Modules HGSearchModule,HGCurveModule -Clean -Jobs 8"
    Write-Host "  modular_build.ps1 -Modules all -Debug"
    Write-Host "  modular_build.ps1 -ListModules"
    Write-Host ""
    exit 0
}

# 列出所有模块
if ($ListModules) {
    Write-Host "所有可用模块:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "基础模块（默认包含）:" -ForegroundColor Cyan
    $DefaultModules.Split(" ") | ForEach-Object { Write-Host "  $_" }
    Write-Host ""
    Write-Host "功能模块:" -ForegroundColor Cyan
    $AllModules.Split(" ") | Where-Object { $DefaultModules.Split(" ") -notcontains $_ } | ForEach-Object { Write-Host "  $_" }
    Write-Host ""
    exit 0
}

# 自动检测CPU核心数
$cores = 4
try {
    $cpuInfo = Get-WmiObject -Class Win32_Processor
    $cores = $cpuInfo.NumberOfCores
    if ($cores -gt 8) { $cores = 8 }
    if ($cores -lt 2) { $cores = 2 }
} catch {
    $cores = 4
}

# 处理模块参数
if ($Modules.Count -eq 0) {
    Write-Host "未指定模块，使用默认模块" -ForegroundColor Yellow
    $selectedModules = $DefaultModules
} elseif ($Modules -contains "all") {
    Write-Host "编译所有模块" -ForegroundColor Green
    $selectedModules = $AllModules
} else {
    # 验证模块是否存在
    $validModules = @()
    foreach ($module in $Modules) {
        if ($AllModules.Split(" ") -contains $module) {
            $validModules += $module
        } else {
            Write-Warning "模块 '$module' 不存在，将被忽略"
        }
    }
    
    if ($validModules.Count -eq 0) {
        Write-Host "没有有效的模块，使用默认模块" -ForegroundColor Yellow
        $selectedModules = $DefaultModules
    } else {
        $selectedModules = $validModules -join " "
    }
}

# 设置编译选项
if ($Debug) { $BuildType = "Debug" } else { $BuildType = "Release" }
if ($Jobs -gt 0) { $JobCount = $Jobs } else { $JobCount = $cores }

# 显示编译信息
Write-Host "========================================" -ForegroundColor Green
Write-Host "   HGAppModule 模块化编译" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "编译类型: $BuildType"
Write-Host "并行任务数: $JobCount"
Write-Host "编译模块: $selectedModules"
Write-Host "构建目录: $BuildDir"
Write-Host ""

# 清理构建目录
if ($Clean) {
    Write-Host "清理构建目录..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
}

# 创建构建目录
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# 设置Qt和MinGW路径
$QtDir = "D:\Qt\6.9.1\mingw_64"
$MingwDir = "D:\Qt\Tools\mingw1310_64"
$env:PATH = "$MingwDir\bin;" + $env:PATH

# 创建临时CMakeLists.txt用于模块化编译
Write-Host "创建模块化编译配置..." -ForegroundColor Yellow

# 使用字符串拼接而不是here-string来避免转义问题
$CmakeContent = ""
$CmakeContent += "# 临时模块化编译配置`n"
$CmakeContent += "cmake_minimum_required(VERSION 3.16)`n"
$CmakeContent += "project(HGAppModule_Modular)`n"
$CmakeContent += "`n"
$CmakeContent += "# 设置项目根目录`n"
$CmakeContent += "set(HG_PROJECT_ROOT_FS $ProjectRoot\\..)`n"
$CmakeContent += "set(PLATFORM win32)`n"
$CmakeContent += "`n"
$CmakeContent += "# 设置C++标准`n"
$CmakeContent += "set(CMAKE_CXX_STANDARD 17)`n"
$CmakeContent += "set(CMAKE_CXX_STANDARD_REQUIRED ON)`n"
$CmakeContent += "`n"
$CmakeContent += "# 包含项目宏`n"
$CmakeContent += "include($ProjectRoot\\..\\cmake\\project_macro.cmake)`n"
$CmakeContent += "`n"
$CmakeContent += "# 包含目录`n"
$CmakeContent += "include_directories(`n"
$CmakeContent += "    $ProjectRoot`n"
$CmakeContent += "    $ProjectRoot\\..\\3rdparty\\include`n"
$CmakeContent += "    $ProjectRoot\\..\\3rdparty\\include\\opencv4`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGAtomService\\HGSaveService\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGAtomService\\HGFrameService\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGAtomService\\HGSecurityService\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGAtomService\\HGCommunicateService\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGAtomService\\HGAuthorityService\\inc`n"
$CmakeContent += "    $ProjectRoot\\HGBaseAppModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\pluginInterface\\inc`n"
$CmakeContent += "    $ProjectRoot\\HGSearchModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\HGUartModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\HGPrintModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\HGCurveModule\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGHardware\\HGLowerPC\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGCommon\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\services\\HGConfig\\inc`n"
$CmakeContent += "    $ProjectRoot\\..\\pluginInterface\\inc`n"
$CmakeContent += ")`n"
$CmakeContent += "`n"
$CmakeContent += "# 链接目录`n"
$CmakeContent += "link_directories($ProjectRoot\\..\\3rdparty\\lib)`n"
$CmakeContent += "`n"
$CmakeContent += "# 查找Qt6`n"
$CmakeContent += "find_package(Qt6 COMPONENTS Core Widgets Gui Charts LinguistTools PrintSupport Designer UiPlugin Qml REQUIRED)`n"
$CmakeContent += "`n"
$CmakeContent += "# 链接Qt库`n"
$CmakeContent += "link_libraries(`n"
$CmakeContent += "    Qt6::Core`n"
$CmakeContent += "    Qt6::Widgets`n"
$CmakeContent += "    Qt6::Gui`n"
$CmakeContent += "    Qt6::Charts`n"
$CmakeContent += "    Qt6::PrintSupport`n"
$CmakeContent += "    Qt6::Designer`n"
$CmakeContent += "    Qt6::UiPlugin`n"
$CmakeContent += "    Qt6::Qml`n"
$CmakeContent += ")`n"
$CmakeContent += "`n"
$CmakeContent += "# 添加选中的模块`n"
$CmakeContent += "message(STATUS \"Building modules: $selectedModules\")`n"
$CmakeContent += "`n"

# 为每个选中的模块添加add_subdirectory
foreach ($module in $selectedModules.Split(" ")) {
    $CmakeContent += "add_subdirectory($module)`n"
}

$CmakeContent | Out-File -FilePath (Join-Path $BuildDir "CMakeLists.txt") -Encoding UTF8

# 配置CMake
Write-Host "配置CMake..." -ForegroundColor Yellow
Set-Location $BuildDir

$cmakeArgs = @("-G", "MinGW Makefiles", "-DCMAKE_BUILD_TYPE=$BuildType", "-DCMAKE_PREFIX_PATH=`"$QtDir`"", ".")

$cmakeResult = & cmake @cmakeArgs 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake配置失败!" -ForegroundColor Red
    Write-Host $cmakeResult
    exit 1
}

# 编译
Write-Host "开始编译..." -ForegroundColor Yellow
$makeResult = & mingw32-make -j$JobCount 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败!" -ForegroundColor Red
    Write-Host $makeResult
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "   编译完成!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "编译模块: $selectedModules"
Write-Host "输出目录: $BuildDir"
Write-Host ""

# 显示编译统计
$libFiles = Get-ChildItem "$BuildDir\*.a" -ErrorAction SilentlyContinue
$exeFiles = Get-ChildItem "$BuildDir\*.exe" -ErrorAction SilentlyContinue

if ($libFiles) {
    Write-Host "生成的库文件:" -ForegroundColor Cyan
    foreach ($file in $libFiles) {
        Write-Host "  $($file.Name)"
    }
}

if ($exeFiles) {
    Write-Host "生成的可执行文件:" -ForegroundColor Cyan
    foreach ($file in $exeFiles) {
        Write-Host "  $($file.Name)"
    }
}

Write-Host ""