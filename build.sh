#!/bin/bash

scrip_path=$(cd `dirname $0`; pwd)

if [ -f "build.config" ]; then
	source build.config
fi

pwd_git_info(){
	echo ----------------------------------
	br=$(git rev-parse --abbrev-ref HEAD)
	cid=$(git rev-parse --short HEAD)
	echo "Branch:" ${br} " Commit id:" ${cid}
	git submodule status
	echo ----------------------------------
}

config(){
	build_ver="x64"
	if [ $# -eq 1 ]; then
		build_ver=$1
	fi

	if [ ${build_ver} = "zone" ];then
		FAKE_ROOT=${FAKE_ROOT_ZONE}
		HG_TOOLCHAIN_FILE="cmake/zone.cmake"
		build_dir=build_zone
		#如果宿主机是arm64,本机编译则不设置TOOLCHAIN_DIR
		arch=$(uname -m)
		if [ "$arch" = "aarch64" ]; then
			HG_TOOLCHAIN_DIR=""
		else 
			HG_TOOLCHAIN_DIR=${TOOLCHAIN_DIR_ZONE}
		fi
	elif [ ${build_ver} = "x64" ];then
        HG_USE_PCH_OPTION="-DHG_USE_PCH=ON"
		FAKE_ROOT=${FAKE_ROOT_X64}
		HG_TOOLCHAIN_FILE="cmake/x64_dbg.cmake"
		build_dir=build_x64
	elif [ ${build_ver} = "x64_release" ];then
        HG_USE_PCH_OPTION="-DHG_USE_PCH=ON"
		FAKE_ROOT=${FAKE_ROOT_X64}
		HG_TOOLCHAIN_FILE="cmake/x64_release.cmake"
		build_dir=build_x64_release
	elif [ ${build_ver} = "win32" ];then
	    HG_USE_PCH_OPTION="-DHG_USE_PCH=OFF"
        echo "Windows交叉编译：禁用预编译头文件"
		FAKE_ROOT=${FAKE_ROOT_WIN32}
		HG_TOOLCHAIN_FILE="cmake/win32.cmake"
		HG_TOOLCHAIN_DIR=${TOOLCHAIN_DIR_WIN32}
		build_dir=build_win32
	elif [ ${build_ver} = "win32_x86" ];then
	    HG_USE_PCH_OPTION="-DHG_USE_PCH=OFF"
        echo "Windows交叉编译：禁用预编译头文件"
		FAKE_ROOT=${FAKE_ROOT_WIN32_X86}
		HG_TOOLCHAIN_FILE="cmake/win32_x86.cmake"
		HG_TOOLCHAIN_DIR=${TOOLCHAIN_DIR_WIN32_X86}
		build_dir=build_win32_x86
	elif [ ${build_ver} = "stm32" ];then
		FAKE_ROOT=${FAKE_ROOT_STM32}
		HG_TOOLCHAIN_FILE="cmake/stm32.cmake"
		HG_TOOLCHAIN_DIR=${TOOLCHAIN_DIR_STM32}
		build_dir=build_stm32
	fi

	if [ ! -d $FAKE_ROOT ]; then
		echo "fake foot path is invaild"
		exit -1;
	fi
	
	echo -----------ver: ${build_ver}-------------------
	echo toolchain config path: $HG_TOOLCHAIN_FILE
	echo toolchain dir: $HG_TOOLCHAIN_DIR
	echo build scrip path: $scrip_path
	echo build dir path: $scrip_path/$build_dir
	echo fake root path: $FAKE_ROOT
	echo ----------------------------------

	if [ $# -eq 0 ]; then
		read -p "Pls confirm toolchain path and fake root path:[y/n] " confirm
		if [ $confirm != y ];then
			exit -1;
		fi
	fi
	# git submodule update --init --recursive
	mkdir -p ${scrip_path}/${build_dir} && cd ${scrip_path}/${build_dir} && pwd
	cmake ../ -DTOOLCHAIN_DIR=${HG_TOOLCHAIN_DIR} -DCMAKE_TOOLCHAIN_FILE=${HG_TOOLCHAIN_FILE} -DHG_PROJECT_ROOT_FS=${FAKE_ROOT} -DPLATFORM=${build_ver}
	pwd_git_info
	if [ $? -ne 0 ]; then echo "congiure failed" && exit -1; fi
}

build(){
	cd ${scrip_path}/${build_dir} && pwd
	# make -j`nproc` install
	make -j$(nproc) install
	if [ $? -ne 0 ]; then echo "make failed" && exit -1; fi
}

usage(){
	echo "usage:"
	echo "build.sh [ver] [c/b]"
	echo "\tver: x64 x64_release stm32 win32 win32_x86"
	echo "\tc:only configure"
	echo "\tb:build with configure"
}

if [ $# -eq 0 ]; then 
	config; build
elif [ $# -eq 1 ]; then 
	config $1; build
elif [ $# -eq 2 ]; then 
	if [ $2 = "c" ]; then
		config $1
	elif [ $2 = "b" ]; then
		config $1; build
	else
		echo -e "can't support cmd:[ $2 ]"
		usage
	fi
else
	usage
fi