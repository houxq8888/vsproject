#!/bin/bash

# 退出时遇到错误立即停止
set -e

# 设置构建目录
BUILD_DIR=build

# 清理旧构建（可选）
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# 一键配置 + 编译
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake \
  -DPLATFORM=win32

make -j$(nproc)  # 多线程编译

