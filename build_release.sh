#!/bin/bash

# HGPro 项目发布版本编译脚本
# 功能: 编译优化后的发布版本，提升程序运行性能

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 脚本路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"

# 加载build.config配置文件
if [ -f "$PROJECT_ROOT/build.config" ]; then
    source "$PROJECT_ROOT/build.config"
    echo -e "${GREEN}✓ 已加载build.config配置文件${NC}"
else
    echo -e "${YELLOW}⚠ 未找到build.config配置文件，使用默认配置${NC}"
fi

# 函数: 显示帮助信息
show_help() {
    echo -e "${CYAN}HGPro 发布版本编译脚本${NC}"
    echo "用法: $0 [选项] [平台]"
    echo ""
    echo "选项:"
    echo "  --help, -h         显示此帮助信息"
    echo "  --clean, -c        清理构建目录后重新编译"
    echo "  --install, -i      编译后安装到系统目录"
    echo "  --list, -l         列出可用的平台配置"
    echo "  --module MODULE    仅编译指定模块"
    echo ""
    echo "平台:"
    echo "  x64                64位Linux系统 (默认)"
    echo "  zone              特定区域版本"
    echo ""
    echo "示例:"
    echo "  $0 x64             编译x64发布版本"
    echo "  $0 --clean x64     清理后编译x64发布版本"
    echo "  $0 --module HGOnlinePlatformApp x64  仅编译HGOnlinePlatformApp模块"
}

# 函数: 列出可用平台
list_platforms() {
    echo -e "${CYAN}可用的发布版本平台配置:${NC}"
    echo "  x64   - 64位Linux系统 (cmake/x64_release.cmake)"
    echo "  zone  - 特定区域版本 (需要配置)"
}

# 函数: 检查平台配置是否存在
check_platform_config() {
    local platform=$1
    case $platform in
        x64)
            if [ -f "cmake/x64_release.cmake" ]; then
                return 0
            else
                echo -e "${RED}错误: x64发布版本配置文件不存在${NC}"
                return 1
            fi
            ;;
        zone)
            echo -e "${YELLOW}警告: zone平台发布版本配置需要手动设置${NC}"
            return 0
            ;;
        *)
            echo -e "${RED}错误: 不支持的平台 '$platform'${NC}"
            return 1
            ;;
    esac
}

# 函数: 获取CPU核心数
get_cpu_cores() {
    if command -v nproc >/dev/null 2>&1; then
        nproc
    else
        echo 4  # 默认值
    fi
}

# 函数: 编译发布版本
build_release() {
    local platform=$1
    local clean_build=$2
    local install_flag=$3
    local module_name=$4
    
    local build_dir="build_${platform}_release"
    local toolchain_file=""
    
    # 设置平台特定的变量（基于原始build.sh的逻辑）
    local FAKE_ROOT=""
    local HG_TOOLCHAIN_DIR=""
    
    case $platform in
        x64)
            toolchain_file="cmake/x64_release.cmake"
            FAKE_ROOT="${FAKE_ROOT_X64:-~/code/install/x64}"
            ;;
        zone)
            toolchain_file="cmake/zone.cmake"  # 需要手动配置
            FAKE_ROOT="${FAKE_ROOT_ZONE:-~/code/install/zone}"
            # 如果宿主机是arm64,本机编译则不设置TOOLCHAIN_DIR
            local arch=$(uname -m)
            if [ "$arch" != "aarch64" ]; then
                HG_TOOLCHAIN_DIR="${TOOLCHAIN_DIR_ZONE:-/usr/bin}"
            fi
            ;;
    esac
    
    # 检查工具链文件
    if [ ! -f "$toolchain_file" ] && [ "$platform" != "zone" ]; then
        echo -e "${RED}错误: 工具链文件 '$toolchain_file' 不存在${NC}"
        return 1
    fi
    
    # 检查FAKE_ROOT目录是否存在
    if [ ! -d "$FAKE_ROOT" ]; then
        echo -e "${RED}错误: fake root路径无效或不存在: $FAKE_ROOT${NC}"
        echo -e "${YELLOW}请检查build.config文件中的配置或创建相应目录${NC}"
        return 1
    fi
    
    # 清理构建目录
    if [ "$clean_build" = "true" ]; then
        echo -e "${YELLOW}清理构建目录 '$build_dir'...${NC}"
        rm -rf "$build_dir"
    fi
    
    # 创建构建目录
    mkdir -p "$build_dir"
    cd "$build_dir" || {
        echo -e "${RED}错误: 无法进入构建目录${NC}"
        return 1
    }
    
    # 配置CMake
    echo -e "${CYAN}配置CMake ($platform 发布版本)...${NC}"
    local cmake_cmd="cmake .."
    if [ -f "../$toolchain_file" ]; then
        cmake_cmd="$cmake_cmd -DCMAKE_TOOLCHAIN_FILE=../$toolchain_file"
    fi
    
    # 添加发布版本特定选项和平台变量
    cmake_cmd="$cmake_cmd -DCMAKE_BUILD_TYPE=Release -DTOOLCHAIN_DIR=\"$HG_TOOLCHAIN_DIR\" -DHG_PROJECT_ROOT_FS=\"$FAKE_ROOT\" -DPLATFORM=\"$platform\""
    
    echo -e "${BLUE}执行: $cmake_cmd${NC}"
    if ! eval "$cmake_cmd"; then
        echo -e "${RED}CMake配置失败${NC}"
        cd ..
        return 1
    fi
    
    # 编译
    local cpu_cores=$(get_cpu_cores)
    echo -e "${CYAN}开始编译 ($cpu_cores 线程)...${NC}"
    
    local start_time=$(date +%s)
    
    if [ -n "$module_name" ]; then
        # 编译指定模块
        echo -e "${GREEN}编译模块: $module_name${NC}"
        if ! make "$module_name" -j"$cpu_cores"; then
            echo -e "${RED}模块编译失败${NC}"
            cd ..
            return 1
        fi
    else
        # 编译全部
        if ! make -j"$cpu_cores"; then
            echo -e "${RED}编译失败${NC}"
            cd ..
            return 1
        fi
    fi
    
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    
    # 安装
    if [ "$install_flag" = "true" ]; then
        echo -e "${CYAN}安装到系统目录...${NC}"
        if ! make install; then
            echo -e "${YELLOW}安装失败，但编译成功${NC}"
        fi
    fi
    
    echo -e "${GREEN}✅ 发布版本编译完成!${NC}"
    echo -e "${BLUE}编译时间: ${duration}秒${NC}"
    echo -e "${BLUE}构建目录: $build_dir${NC}"
    
    # 显示优化信息
    echo -e "${CYAN}优化特性:${NC}"
    echo "  • O2级别优化"
    echo "  • 链接时优化(LTO)"
    echo "  • 本地架构优化"
    echo "  • 调试代码禁用"
    
    cd ..
}

# 主函数
main() {
    local platform="x64"
    local clean_build="false"
    local install_flag="false"
    local module_name=""
    
    # 解析参数
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help
                return 0
                ;;
            --clean|-c)
                clean_build="true"
                shift
                ;;
            --install|-i)
                install_flag="true"
                shift
                ;;
            --list|-l)
                list_platforms
                return 0
                ;;
            --module)
                if [ -n "$2" ]; then
                    module_name="$2"
                    shift 2
                else
                    echo -e "${RED}错误: --module 需要指定模块名称${NC}"
                    return 1
                fi
                ;;
            x64|zone)
                platform="$1"
                shift
                ;;
            *)
                echo -e "${RED}错误: 未知参数 '$1'${NC}"
                show_help
                return 1
                ;;
        esac
    done
    
    # 检查平台配置
    if ! check_platform_config "$platform"; then
        return 1
    fi
    
    # 检查模块是否存在
    if [ -n "$module_name" ]; then
        if [ ! -d "projects/$module_name" ] && [ ! -d "modules/$module_name" ]; then
            echo -e "${RED}错误: 模块 '$module_name' 不存在${NC}"
            return 1
        fi
    fi
    
    echo -e "${PURPLE}=== HGPro 发布版本编译 ===${NC}"
    echo -e "平台: ${GREEN}$platform${NC}"
    if [ -n "$module_name" ]; then
        echo -e "模块: ${GREEN}$module_name${NC}"
    fi
    echo -e "清理构建: ${GREEN}$clean_build${NC}"
    echo -e "安装: ${GREEN}$install_flag${NC}"
    echo ""
    
    # 执行编译
    build_release "$platform" "$clean_build" "$install_flag" "$module_name"
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi