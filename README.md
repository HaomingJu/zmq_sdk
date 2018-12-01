# zmq_sdk 开发文档

## 功能
zmq_sdk 提供了一套跨平台, 快速的通讯机制.

## 编译
### 组织文件部署
```
cp build.properties.local.example build.properties.local
cp build.properties.example.matrix build.properties
```

### 指定编译器
```
vim build.properties.local
```

```
architecture armhf
platform linux
vendor matrix
cmake_c_compiler /home/root/software/ToolChain/Matrix/linaro-arm-linux-gnueabihf-4.9/bin/arm-linux-gnueabihf-gcc
cmake_cxx_compiler /home/root/software/ToolChain/Matrix/linaro-arm-linux-gnueabihf-4.9/bin/arm-linux-gnueabihf-g++
```

Matrix Compile Path: `smb://bjnas/horizonrobotics/CarNet/Haoming.Ju/Matrix交叉工具链/linaro-arm-linux-gnueabihf-4.9.tar.gz`

### 编译操作

```
mkdir build
cd build
cmake ../
make copy
make -j4
make install
```
