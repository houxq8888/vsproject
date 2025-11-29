#!/bin/bash

# 开发环境配置脚本
# 用于优化开发体验和编译效率

scrip_path=$(cd `dirname $0`; pwd)

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 显示系统信息
show_system_info() {
    echo -e "${BLUE}=== 系统信息 ===${NC}"
    echo "CPU核心数: $(nproc)"
    echo "内存总量: $(free -h | awk '/^Mem:/{print $2}')"
    echo "磁盘使用: $(df -h . | awk 'NR==2{print $4 " 可用 / " $2 " 总量"}')"
    echo ""
}

# 检查ccache是否安装
check_ccache() {
    if command -v ccache &> /dev/null; then
        echo -e "${GREEN}✓ ccache 已安装${NC}"
        echo "ccache统计信息:"
        ccache -s | head -10
        echo ""
    else
        echo -e "${YELLOW}⚠ ccache 未安装${NC}"
        echo "安装命令: sudo apt install ccache"
        echo "ccache可以大幅提升增量编译速度"
        echo ""
    fi
}

# 检查编译目录状态
check_build_status() {
    echo -e "${BLUE}=== 编译目录状态 ===${NC}"
    
    if [ -d "${scrip_path}/build_x64" ]; then
        echo -e "${GREEN}✓ build_x64 目录存在${NC}"
        
        # 检查CMakeCache是否存在
        if [ -f "${scrip_path}/build_x64/CMakeCache.txt" ]; then
            echo "  CMake配置: 已生成"
            
            # 检查编译目标
            cd "${scrip_path}/build_x64"
            if make -n HGOnlinePlatformApp 2>/dev/null; then
                echo -e "  ${GREEN}HGOnlinePlatformApp: 可编译${NC}"
            else
                echo -e "  ${YELLOW}HGOnlinePlatformApp: 不可编译${NC}"
            fi
        else
            echo -e "  ${YELLOW}CMake配置: 未生成${NC}"
        fi
    else
        echo -e "${YELLOW}⚠ build_x64 目录不存在${NC}"
        echo "  需要先运行: ./build.sh x64"
    fi
    echo ""
}

# 快速编译当前模块（假设你正在开发HGOnlinePlatformApp）
quick_build() {
    local module=${1:-HGOnlinePlatformApp}
    
    echo -e "${BLUE}=== 快速编译: ${module} ===${NC}"
    
    if [ ! -d "${scrip_path}/build_x64" ]; then
        echo -e "${RED}错误: 编译目录不存在${NC}"
        echo "请先运行: ./build.sh x64"
        return 1
    fi
    
    cd "${scrip_path}/build_x64"
    
    # 检查模块是否存在
    if ! make -n "$module" 2>/dev/null; then
        echo -e "${RED}错误: 模块 '$module' 不存在${NC}"
        echo "使用 './build_module.sh --list' 查看可用模块"
        return 1
    fi
    
    # 开始编译
    start_time=$(date +%s)
    echo -e "${YELLOW}开始编译...${NC}"
    
    make "$module" -j$(nproc)
    
    if [ $? -eq 0 ]; then
        end_time=$(date +%s)
        duration=$((end_time - start_time))
        echo -e "${GREEN}✓ 编译成功! 耗时: ${duration}秒${NC}"
        
        # 显示编译产物
        echo ""
        echo -e "${BLUE}编译产物:${NC}"
        find . -name "*${module}*" -type f -executable 2>/dev/null | head -5
    else
        echo -e "${RED}✗ 编译失败${NC}"
        return 1
    fi
}

# 清理编译缓存
clean_cache() {
    echo -e "${BLUE}=== 清理编译缓存 ===${NC}"
    
    if command -v ccache &> /dev/null; then
        echo "清理ccache缓存..."
        ccache -C
        echo -e "${GREEN}ccache缓存已清理${NC}"
    fi
    
    if [ -d "${scrip_path}/build_x64" ]; then
        echo "清理编译目录..."
        cd "${scrip_path}/build_x64"
        make clean 2>/dev/null || true
        echo -e "${GREEN}编译目录已清理${NC}"
    fi
    
    echo ""
}

# 性能优化建议
performance_tips() {
    echo -e "${BLUE}=== 性能优化建议 ===${NC}"
    echo ""
    echo -e "${YELLOW}1. 使用模块化编译${NC}"
    echo "   ./build_module.sh HGOnlinePlatformApp x64"
    echo ""
    echo -e "${YELLOW}2. 启用ccache缓存${NC}"
    echo "   安装: sudo apt install ccache"
    echo "   配置: export CCACHE_DIR=~/.ccache"
    echo ""
    echo -e "${YELLOW}3. 使用SSD存储${NC}"
    echo "   如果使用HDD，考虑迁移到SSD"
    echo ""
    echo -e "${YELLOW}4. 增加系统内存${NC}"
    echo "   更多内存可以减少编译时的磁盘交换"
    echo ""
    echo -e "${YELLOW}5. 定期清理缓存${NC}"
    echo "   ./dev_env.sh --clean"
    echo ""
}

# 主函数
main() {
    case "${1}" in
        "--info"|"-i")
            show_system_info
            check_ccache
            check_build_status
            ;;
        "--build"|"-b")
            quick_build "${2:-HGOnlinePlatformApp}"
            ;;
        "--clean"|"-c")
            clean_cache
            ;;
        "--tips"|"-t")
            performance_tips
            ;;
        "--help"|"-h")
            echo -e "${BLUE}开发环境管理脚本${NC}"
            echo ""
            echo "用法: ./dev_env.sh [选项]"
            echo ""
            echo "选项:"
            echo "  --info, -i     显示系统信息和编译状态"
            echo "  --build, -b    快速编译指定模块（默认: HGOnlinePlatformApp）"
            echo "  --clean, -c    清理编译缓存"
            echo "  --tips, -t     显示性能优化建议"
            echo "  --help, -h     显示此帮助信息"
            echo ""
            echo "示例:"
            echo "  ./dev_env.sh --info"
            echo "  ./dev_env.sh --build"
            echo "  ./dev_env.sh --build HGDashboardApp"
            echo "  ./dev_env.sh --clean"
            ;;
        *)
            echo -e "${BLUE}=== HGPro 开发环境 ===${NC}"
            echo ""
            show_system_info
            check_ccache
            check_build_status
            echo "使用 './dev_env.sh --help' 查看所有功能"
            ;;
    esac
}

# 执行主函数
main "$@"