#!/bin/bash
set -e
sudo echo begin
runPath=$(pwd)

# 安装依赖
sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    brew install git cmake
elif [ $sysOS == "Linux" ];then
    sudo apt install git build-essential -y

    # 安装cmake
    #wget https://github.com/Kitware/CMake/releases/download/v3.15.0-rc3/cmake-3.15.0-rc3-Linux-x86_64.tar.gz
    cmakePath="/cmake_3.15.0"
    if [ ! -d $cmakePath ]; then
        sudo apt autoremove cmake -y
        cd pkg
        sudo tar -zxvf cmake-3.15.0-rc3-Linux-x86_64.tar.gz -C /
        sudo rm -rf $cmakePath
        sudo mv /cmake-3.15.0-rc3-Linux-x86_64 $cmakePath
        sudo ln -sf $cmakePath/bin/* /usr/bin/
        cd $runPath
    fi
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
    rm -rf release
    mkdir release && cd release
    cmake ..
    make && sudo make install
    cd .. && rm -rf release
    cd $runPath
fi

# 安装程序
cd $runPath
if [ ! -d "release" ]; then
    mkdir release
fi
cd release
cmake ..
make
sudo make install
sudo chmod +x -R /usr/local/bin/fbs_reader
cd $runPath

