#!/bin/bash
set -e
sysName=$(cat /etc/*-release)
if [[ "$sysName" =~ CentOS ]]
then
    {
        yum install sudo gcc-c++ -y
    } ||
    {
        echo not by root
    }
fi
sudo echo begin
runPath=$(pwd)
sysOS=$(uname -s)


# 安装依赖
if [ $sysOS == "Darwin" ]
then
    brew install git cmake gcc make
elif [ $sysOS == "Linux" ]
then
    sysName=$(cat /etc/*-release)
    if [[ "$sysName" =~ Ubuntu ]]
    then
        sudo apt install git build-essential gcc make -y
        sudo apt autoremove cmake -y
    elif [[ "$sysName" =~ CentOS ]]
    then
        sudo yum install git make gcc unzip -y
        
    fi

    if [ ! -d "/reckless/opt/" ]
    then
        sudo mkdir -p /reckless/opt/
    fi
    # 安装cmake3.15
    cd $runPath
    cmakePath="/reckless/opt/cmake_3.15"
    if [ ! -d $cmakePath ]
    then
        cd pkg
        sudo tar -zxvf cmake-3.15.0-rc4.tar.gz -C /reckless/
        sudo mv /reckless/cmake-3.15.0-rc4 $cmakePath
        cd $cmakePath
        sudo ./bootstrap
        sudo make 
        sudo make install
        sudo ln -sf $cmakePath/bin/*  /usr/bin/ 
        cd $runPath
    fi

    # 安装flatbuffers
    #git clone https://github.com/google/flatbuffers.git
    flatcVersion="notFoundFlatc"
    if type flatc >/dev/null 2>&1
    then
        flatcVersion=$(flatc --version)
    fi
    echo $flatcVersion
    if [ "$flatcVersion" != "flatc version 1.11.0" ]
    then
        cd $runPath
        cd pkg/
        rm -rf flatbuffers-master
        unzip flatbuffers-master.zip
        cd flatbuffers-master && mkdir release && cd release
        cmake ..
        make && sudo make install
        cd $runPath
        rm -rf pkg/flatbuffers-master
    fi
fi



# 安装程序
cd $runPath
rm -rf build
if [ ! -d "build" ]
then
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



