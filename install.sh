#!/bin/bash
set -e
sudo echo begin
runPath=$(pwd)


# 安装依赖
sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    brew install git cmake gcc make
elif [ $sysOS == "Linux" ];then
    sudo apt install git build-essential cmake gcc make clang-6.0 llvm-6.0 llvm-6.0-tools llvm-6.0-dev -y

    sudo rm -rf /usr/bin/clang
    sudo ln -s /usr/bin/clang-6.0 /usr/bin/clang
    sudo rm -rf /usr/bin/clang++
    sudo ln -s /usr/bin/clang++-6.0 /usr/bin/clang++
    sudo rm -rf /usr/bin/llvm-ar
    sudo ln -s /usr/bin/llvm-ar-6.0 /usr/bin/llvm-ar
    sudo rm -rf /usr/bin/llvm-ld
    sudo ln -s /usr/bin/llvm-ld-6.0 /usr/bin/llvm-ld
    sudo rm -rf /usr/bin/llvm-nm
    sudo ln -s /usr/bin/llvm-nm-6.0 /usr/bin/llvm-nm
    sudo rm -rf /usr/bin/llvm-objdump
    sudo ln -s /usr/bin/llvm-objdump-6.0 /usr/bin/llvm-objdump
    sudo rm -rf /usr/bin/llvm-ranlib
    sudo ln -s /usr/bin/llvm-ranlib-6.0 /usr/bin/llvm-ranlib
fi

# 安装flatbuffers
#git clone https://github.com/google/flatbuffers.git
fbsVersion="null"
if type flatc >/dev/null 2>&1; then 
    fbsVersion=$(flatc --version)
fi
echo $fbsVersion
if [ "$fbsVersion" != "flatc version 1.11.0" ]; then
    cd pkg/flatbuffers
    rm -rf build
    mkdir build && cd build
    {
        cmake ..
    } ||
    {
        sudo rm -rf *
        cmake ..
    }
    make -j && sudo make install
    cd $runPath
fi

# 安装程序
cd $runPath
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
{
    cmake -DCMAKE_BUILD_TYPE=Release ..
} ||
{
    sudo rm -rf *
    cmake -DCMAKE_BUILD_TYPE=Release ..
}
make -j
sudo make install
sudo chmod -R +x /usr/local/bin/fbs_reader
cd $runPath


echo 完成



