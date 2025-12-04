#!/bin/bash

# HGPro 发布版本性能对比测试脚本
# 功能: 比较调试版本和发布版本的性能差异

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 函数: 显示帮助信息
show_help() {
    echo -e "${CYAN}HGPro 发布版本性能对比测试脚本${NC}"
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --help, -h         显示此帮助信息"
    echo "  --app APP_NAME     指定测试的应用名称 (默认: HGOnlinePlatformApp)"
    echo "  --iterations N     测试迭代次数 (默认: 5)"
    echo "  --quick            快速测试 (仅1次迭代)"
    echo ""
    echo "示例:"
    echo "  $0                   测试HGOnlinePlatformApp性能"
    echo "  $0 --app HGDashboardApp --iterations 10  测试HGDashboardApp性能"
}

# 函数: 检查应用是否存在
check_app_exists() {
    local app_name=$1
    local debug_app="build_x64/bin/$app_name"
    local release_app="build_x64_release/bin/$app_name"
    
    if [ ! -f "$debug_app" ]; then
        echo -e "${RED}错误: 调试版本应用 '$debug_app' 不存在${NC}"
        echo "请先运行 ./build.sh x64 编译调试版本"
        return 1
    fi
    
    if [ ! -f "$release_app" ]; then
        echo -e "${RED}错误: 发布版本应用 '$release_app' 不存在${NC}"
        echo "请先运行 ./build_release.sh x64 编译发布版本"
        return 1
    fi
    
    return 0
}

# 函数: 获取文件大小信息
get_file_info() {
    local file_path=$1
    if [ -f "$file_path" ]; then
        local size=$(du -h "$file_path" | cut -f1)
        local details=$(file "$file_path" 2>/dev/null | cut -d: -f2- | sed 's/^ *//')
        echo "大小: $size, 类型: $details"
    else
        echo "文件不存在"
    fi
}

# 函数: 测试应用启动时间
test_startup_time() {
    local app_path=$1
    local app_name=$2
    local version=$3
    
    echo -e "${CYAN}测试 $version 版本启动时间...${NC}"
    
    # 检查应用是否存在
    if [ ! -f "$app_path" ]; then
        echo -e "${RED}错误: 应用文件不存在: $app_path${NC}"
        return
    fi
    
    # 检查是否可执行
    if [ ! -x "$app_path" ]; then
        chmod +x "$app_path"
    fi
    
    local total_time=0
    local min_time=999999
    local max_time=0
    
    for i in $(seq 1 $iterations); do
        echo -n "  第 $i 次测试... "
        
        # 使用time命令测量启动时间（仅启动到退出）
        local start_time=$(date +%s%N)
        
        # 尝试启动应用并立即退出（如果应用支持--help或--version）
        if timeout 5s "$app_path" --help >/dev/null 2>&1; then
            local end_time=$(date +%s%N)
        elif timeout 5s "$app_path" --version >/dev/null 2>&1; then
            local end_time=$(date +%s%N)
        else
            # 如果应用不支持快速退出参数，使用更短的超时
            timeout 1s "$app_path" >/dev/null 2>&1
            local end_time=$(date +%s%N)
        fi
        
        local duration=$(( (end_time - start_time) / 1000000 ))  # 转换为毫秒
        
        # 更新统计信息
        total_time=$((total_time + duration))
        if [ $duration -lt $min_time ]; then
            min_time=$duration
        fi
        if [ $duration -gt $max_time ]; then
            max_time=$duration
        fi
        
        echo -e "${GREEN}${duration}ms${NC}"
    done
    
    local avg_time=$((total_time / iterations))
    
    echo -e "${BLUE}统计结果 ($version):${NC}"
    echo "  平均时间: ${avg_time}ms"
    echo "  最短时间: ${min_time}ms"
    echo "  最长时间: ${max_time}ms"
    echo ""
    
    # 返回平均时间用于比较
    echo $avg_time
}

# 函数: 比较文件大小
compare_file_sizes() {
    local debug_app="build_x64/bin/$app_name"
    local release_app="build_x64_release/bin/$app_name"
    
    echo -e "${CYAN}文件大小比较:${NC}"
    
    if [ -f "$debug_app" ]; then
        local debug_size=$(du -b "$debug_app" | cut -f1)
        echo -e "调试版本: $(get_file_info "$debug_app")"
    fi
    
    if [ -f "$release_app" ]; then
        local release_size=$(du -b "$release_app" | cut -f1)
        echo -e "发布版本: $(get_file_info "$release_app")"
    fi
    
    if [ -f "$debug_app" ] && [ -f "$release_app" ]; then
        local size_diff=$((release_size - debug_size))
        local size_ratio=$(echo "scale=2; $release_size / $debug_size * 100" | bc)
        
        echo -e "${BLUE}大小差异:${NC}"
        if [ $size_diff -lt 0 ]; then
            echo -e "  发布版本比调试版本小: $((debug_size - release_size)) 字节"
            echo -e "  发布版本是调试版本的: ${GREEN}${size_ratio}%${NC}"
        else
            echo -e "  发布版本比调试版本大: $size_diff 字节"
            echo -e "  发布版本是调试版本的: ${YELLOW}${size_ratio}%${NC}"
        fi
    fi
    echo ""
}

# 函数: 显示优化信息
show_optimization_info() {
    echo -e "${PURPLE}=== 发布版本优化特性 ===${NC}"
    echo ""
    echo -e "${CYAN}编译优化:${NC}"
    echo "  • O2级别优化 - 平衡性能和编译时间"
    echo "  • 链接时优化(LTO) - 跨模块优化"
    echo "  • 本地架构优化 - 针对当前CPU优化"
    echo ""
    echo -e "${CYAN}代码优化:${NC}"
    echo "  • 调试代码禁用(NDEBUG) - 移除断言和调试输出"
    echo "  • 函数内联优化"
    echo "  • 死代码消除"
    echo ""
    echo -e "${CYAN}预期性能提升:${NC}"
    echo "  • 启动速度: 提升 30-50%"
    echo "  • 运行性能: 提升 20-40%"
    echo "  • 内存使用: 优化 10-20%"
    echo ""
}

# 主函数
main() {
    local app_name="HGOnlinePlatformApp"
    local iterations=5
    local quick_test=false
    
    # 解析参数
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help
                return 0
                ;;
            --app)
                if [ -n "$2" ]; then
                    app_name="$2"
                    shift 2
                else
                    echo -e "${RED}错误: --app 需要指定应用名称${NC}"
                    return 1
                fi
                ;;
            --iterations)
                if [ -n "$2" ] && [[ "$2" =~ ^[0-9]+$ ]]; then
                    iterations="$2"
                    shift 2
                else
                    echo -e "${RED}错误: --iterations 需要指定数字${NC}"
                    return 1
                fi
                ;;
            --quick)
                quick_test=true
                iterations=1
                shift
                ;;
            *)
                echo -e "${RED}错误: 未知参数 '$1'${NC}"
                show_help
                return 1
                ;;
        esac
    done
    
    # 检查应用是否存在
    if ! check_app_exists "$app_name"; then
        return 1
    fi
    
    echo -e "${PURPLE}=== HGPro 发布版本性能对比测试 ===${NC}"
    echo -e "测试应用: ${GREEN}$app_name${NC}"
    echo -e "测试迭代: ${GREEN}$iterations${NC} 次"
    echo ""
    
    # 显示优化信息
    show_optimization_info
    
    # 比较文件大小
    compare_file_sizes
    
    # 测试调试版本
    local debug_avg=$(test_startup_time "build_x64/bin/$app_name" "$app_name" "调试")
    
    # 测试发布版本
    local release_avg=$(test_startup_time "build_x64_release/bin/$app_name" "$app_name" "发布")
    
    # 性能对比
    if [ -n "$debug_avg" ] && [ -n "$release_avg" ]; then
        echo -e "${PURPLE}=== 性能对比结果 ===${NC}"
        
        local speedup=$(echo "scale=2; $debug_avg / $release_avg" | bc)
        local improvement=$(echo "scale=1; ($debug_avg - $release_avg) / $debug_avg * 100" | bc)
        
        echo -e "调试版本平均启动时间: ${YELLOW}${debug_avg}ms${NC}"
        echo -e "发布版本平均启动时间: ${GREEN}${release_avg}ms${NC}"
        echo ""
        
        if (( $(echo "$speedup > 1" | bc -l) )); then
            echo -e "${GREEN}✅ 性能提升: ${improvement}%${NC}"
            echo -e "${GREEN}✅ 速度提升倍数: ${speedup}x${NC}"
        else
            echo -e "${YELLOW}⚠️  性能变化: ${improvement}%${NC}"
            echo -e "${YELLOW}⚠️  速度变化倍数: ${speedup}x${NC}"
        fi
        
        echo ""
        echo -e "${CYAN}建议:${NC}"
        if (( $(echo "$improvement > 20" | bc -l) )); then
            echo "  • 发布版本性能提升显著，推荐用于生产环境"
        elif (( $(echo "$improvement > 10" | bc -l) )); then
            echo "  • 发布版本有一定性能提升，可用于测试环境"
        else
            echo "  • 性能提升不明显，建议检查编译配置"
        fi
    fi
    
    echo ""
    echo -e "${GREEN}测试完成!${NC}"
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    # 检查bc命令是否可用（用于浮点计算）
    if ! command -v bc >/dev/null 2>&1; then
        echo -e "${RED}错误: 需要安装bc命令 (sudo apt install bc)${NC}"
        exit 1
    fi
    
    main "$@"
fi