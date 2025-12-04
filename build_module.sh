#!/bin/bash

# 模块化编译脚本
# 用法: ./build_module.sh [模块名] [平台] [选项]

scrip_path=$(cd `dirname $0`; pwd)

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 显示帮助信息
show_help() {
    echo -e "${BLUE}模块化编译脚本 - 快速编译特定模块${NC}"
    echo ""
    echo "用法: ./build_module.sh [模块名] [平台] [选项]"
    echo ""
    echo -e "${YELLOW}可用模块:${NC}"
    echo "  HGOnlinePlatformApp - 在线平台应用 (推荐使用)"
    echo "  HGDashboardApp      - 仪表板应用"
    echo "  ProductFlow         - 产品流程应用"
    echo "  HGBaseAppModule     - 基础应用模块"
    echo "  HGLoginModule       - 登录模块"
    echo "  all                 - 编译所有模块"
    echo ""
    echo -e "${YELLOW}平台:${NC}"
    echo "  x64        - 64位Linux (默认)"
    echo "  win32      - 32位Windows"
    echo "  win32_x86  - 32位Windows x86"
    echo ""
    echo -e "${YELLOW}选项:${NC}"
    echo "  --clean    - 清理后重新编译"
    echo "  --install  - 编译后安装"
    echo "  --list     - 列出所有可用模块"
    echo ""
    echo -e "${GREEN}示例:${NC}"
    echo "  ./build_module.sh HGOnlinePlatformApp x64"
    echo "  ./build_module.sh HGOnlinePlatformApp x64 --clean"
    echo "  ./build_module.sh --list"
}

# 列出所有可用模块
list_modules() {
    echo -e "${BLUE}=== 可用模块列表 ===${NC}"
    echo ""
    echo -e "${YELLOW}主要应用:${NC}"
    echo "  HGOnlinePlatformApp - 在线平台应用"
    echo "  HGDashboardApp      - 仪表板应用"
    echo "  ProductFlow         - 产品流程应用"
    echo ""
    echo -e "${YELLOW}应用模块:${NC}"
    echo "  HGBaseAppModule     - 基础应用模块"
    echo "  HGUserAuditModule   - 用户审计模块"
    echo "  HGPrintModule       - 打印模块"
    echo "  HGCurveModule       - 曲线模块"
    echo "  HGLoginModule       - 登录模块"
    echo "  HGSearchModule      - 搜索模块"
    echo "  HGLogModule         - 日志模块"
    echo "  HGMethodModule      - 方法模块"
    echo "  HGFlowModule        - 流程模块"
    echo "  HGTaskModule        - 任务模块"
    echo ""
    echo -e "${YELLOW}服务模块:${NC}"
    echo "  HGCommon            - 通用服务"
    echo "  HGConfig            - 配置服务"
    echo "  HGLowerPC           - 下位机服务"
    echo "  HGSaveService       - 保存服务"
    echo "  HGFrameService      - 框架服务"
    echo ""
    echo -e "${GREEN}使用 'all' 编译所有模块${NC}"
}

# 参数解析
MODULE_NAME=""
PLATFORM="x64"
CLEAN_BUILD=false
INSTALL_AFTER=false
LIST_MODULES=false

for arg in "$@"; do
    case $arg in
        --help|-h)
            show_help
            exit 0
            ;;
        --list)
            LIST_MODULES=true
            ;;
        --clean)
            CLEAN_BUILD=true
            ;;
        --install)
            INSTALL_AFTER=true
            ;;
        x64|win32|win32_x86)
            PLATFORM=$arg
            ;;
        -*)
            echo -e "${RED}未知选项: $arg${NC}"
            show_help
            exit 1
            ;;
        *)
            if [ -z "$MODULE_NAME" ]; then
                MODULE_NAME=$arg
            fi
            ;;
    esac
done

if [ "$LIST_MODULES" = true ]; then
    list_modules
    exit 0
fi

if [ -z "$MODULE_NAME" ]; then
    echo -e "${RED}错误: 请指定要编译的模块${NC}"
    show_help
    exit 1
fi

if [ -f "build.config" ]; then
    source build.config
fi

# 配置构建环境
config_build(){
    if [ ${PLATFORM} = "x64" ];then
        FAKE_ROOT=${FAKE_ROOT_X64}
        HG_TOOLCHAIN_FILE="cmake/x64_dbg.cmake"
        build_dir=build_x64
    elif [ ${PLATFORM} = "win32" ];then
        FAKE_ROOT=${FAKE_ROOT_WIN32}
        HG_TOOLCHAIN_FILE="cmake/win32.cmake"
        build_dir=build_win32
    elif [ ${PLATFORM} = "win32_x86" ];then
        FAKE_ROOT=${FAKE_ROOT_WIN32_X86}
        HG_TOOLCHAIN_FILE="cmake/win32_x86.cmake"
        build_dir=build_win32_x86
    else
        echo -e "${RED}不支持的平台: ${PLATFORM}${NC}"
        exit 1
    fi

    if [ ! -d "${scrip_path}/${build_dir}" ]; then
        echo -e "${YELLOW}构建目录不存在，请先运行完整构建${NC}"
        echo -e "运行: ${GREEN}./build.sh ${PLATFORM}${NC}"
        exit 1
    fi
}

# 清理构建
clean_build() {
    if [ "$CLEAN_BUILD" = true ]; then
        echo -e "${YELLOW}清理构建目录...${NC}"
        cd ${scrip_path}/${build_dir}
        
        if [ "$MODULE_NAME" = "all" ]; then
            make clean
        else
            # 清理特定模块
            if make -n "$MODULE_NAME" 2>/dev/null | grep -q "clean"; then
                make "$MODULE_NAME"-clean 2>/dev/null || true
            fi
        fi
        echo -e "${GREEN}清理完成${NC}"
    fi
}

# 编译特定模块
build_module(){
    cd ${scrip_path}/${build_dir}
    
    # 获取CPU核心数
    CORES=$(nproc)
    echo -e "${BLUE}使用 ${CORES} 个CPU核心进行编译${NC}"
    
    if [ "$MODULE_NAME" = "all" ]; then
        echo -e "${YELLOW}编译所有模块...${NC}"
        make -j${CORES} install
    else
        echo -e "${BLUE}编译模块: ${MODULE_NAME}${NC}"
        
        # 检查模块是否存在
        if ! make -n "$MODULE_NAME" 2>/dev/null; then
            echo -e "${RED}错误: 模块 '${MODULE_NAME}' 不存在${NC}"
            echo -e "${YELLOW}使用 --list 查看可用模块${NC}"
            exit 1
        fi
        
        # 编译模块
        start_time=$(date +%s)
        make "$MODULE_NAME" -j${CORES}
        
        if [ $? -eq 0 ]; then
            end_time=$(date +%s)
            duration=$((end_time - start_time))
            echo -e "${GREEN}模块编译成功! 耗时: ${duration}秒${NC}"
            
            # 如果需要安装
            if [ "$INSTALL_AFTER" = true ]; then
                echo -e "${YELLOW}安装模块...${NC}"
                make install
                echo -e "${GREEN}安装完成${NC}"
            fi
        else
            echo -e "${RED}模块编译失败${NC}"
            exit 1
        fi
    fi
}

# 主流程
echo -e "${BLUE}=== 模块化编译 ===${NC}"
echo -e "模块: ${GREEN}${MODULE_NAME}${NC}"
echo -e "平台: ${YELLOW}${PLATFORM}${NC}"
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "模式: ${YELLOW}清理后编译${NC}"
fi
if [ "$INSTALL_AFTER" = true ]; then
    echo -e "模式: ${YELLOW}编译后安装${NC}"
fi
echo -e "${BLUE}==================${NC}"

config_build
clean_build
build_module