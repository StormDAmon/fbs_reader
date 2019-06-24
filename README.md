## FBS Reader

封装 Flatbuffers 操作函数，简化数据使用过程，提高数据安全性

**支持语言：**
- C++
- Python3


## 安装使用

### 安装依赖 - ubuntu
``` shell
    sudo apt install git cmake
```
### 安装依赖 - macOS
``` shell
    brew install git cmake
```
### 安装依赖
``` shellß
    git clone https://github.com/google/flatbuffers.git && cd flatbuffers
    mkdir release && cd release
    cmake ..
    make && sudo make install
    cd ../.. && rm -rf flatbuffers
```

### 安装
``` shell
    rm -rf build && mkdir build && cd build && cmake ..
    make
    sudo make install
```







