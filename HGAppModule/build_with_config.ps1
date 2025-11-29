# PowerShell构建脚本 - 使用配置文件解决中文乱码问题
# 读取配置文件设置

# 检查配置文件是否存在
if (Test-Path "build_config.ini") {
    Write-Host "读取配置文件: build_config.ini" -ForegroundColor Green
    
    # 应用编码设置
    $OutputEncoding = [System.Text.Encoding]::UTF8
    [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
    [Console]::InputEncoding = [System.Text.Encoding]::UTF8
    
    Write-Host "已应用UTF-8编码设置" -ForegroundColor Yellow
} else {
    Write-Host "配置文件不存在，使用默认设置" -ForegroundColor Yellow
}

# 显示应用信息
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "产品编辑器 - 版本 1.0.0" -ForegroundColor Cyan
Write-Host "使用UTF-8编码解决中文乱码问题" -ForegroundColor Cyan
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

# 运行批处理文件
Write-Host "正在启动构建过程..." -ForegroundColor Green
Write-Host ""

# 使用PowerShell运行批处理文件
$arguments = $args -join " "
$command = "cmd /c 'chcp 65001 >nul && windows_build.bat $arguments'"

Write-Host "执行命令: $command" -ForegroundColor Gray
Write-Host ""

# 执行构建命令
Invoke-Expression $command

# 检查退出代码
if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "构建成功完成!" -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "构建失败!" -ForegroundColor Red
    Write-Host "退出代码: $LASTEXITCODE" -ForegroundColor Red
}

Write-Host ""
Write-Host "构建过程结束" -ForegroundColor Cyan