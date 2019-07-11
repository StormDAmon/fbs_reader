#!/bin/bash
set -e
sudo echo begin
runPath=$(pwd)


# 安装依赖
sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    brew install git cmake gcc make
elif [ $sysOS == "Linux" ];then
    sudo apt install git build-essential cmake gcc make -y
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
    make -j4 && sudo make install
    cd $runPath
fi

# 安装程序
cd $runPath
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
{
    cmake ..
} ||
{
    sudo rm -rf *
    cmake ..
}
make -j4
sudo make install
sudo chmod -R +x /usr/local/bin/fbs_reader
cd $runPath





