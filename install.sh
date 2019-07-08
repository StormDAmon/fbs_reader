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
    sudo apt autoremove cmake -y
    cd pkg
    sudo tar -zxvf cmake-3.15.0-rc3-Linux-x86_64.tar.gz -C /
    sudo rm -rf /cmake
    sudo mv /cmake-3.15.0-rc3-Linux-x86_64 /cmake
    sudo ln -sf /cmake/bin/* /usr/bin/
    cd $runPath


fi

# 安装flatbuffers
#git clone https://github.com/google/flatbuffers.git
cd pkg/flatbuffers
mkdir release && cd release
cmake ..
make && sudo make install
cd .. && rm -rf release
cd $runPath

# 安装程序
sudo rm -rf build && mkdir build && cd build
cmake ..
make
sudo make install
sudo chmod +x -R /usr/local/bin/fbs_reader

