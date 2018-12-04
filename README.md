# zmq_sdk 开发文档

## 功能
### 功能说明
zmq_sdk 提供了一套跨平台, 快速的通讯机制.

提供了 Matrix(armhf) 与 Linux(x86) 之间的一套通信SDK.

在 Linux(x86) 平台下有ClientReceiveUI & ClientReceive 进程, 在 Matrix(amrhf) 下有ClientSend进程两这个相互通信.


### 硬件准备
Linux(x86) 连接 Matrix(armhf) 时, 需要在使用网线连接两者网口之后, 修改Linux(x86)的网络配置.

```
IPv4 Address: 192.168.1.100
Netmast: 255.255.255.0
```

Matrix(armhf)默认网络配置为(不可修改):
```
IPv4 Address: 192.168.1.10
Netmast: 255.255.255.0
```


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
sh pre.sh
mkdir build
cd build
cmake ../
make copy
make -j4
make install
```


## 测试

### 测试用例说明

#### Case 1:
**Matrix(armhf) 与 Linux(x86) 之间zmq通信测试**

Matrix(armhf)中运行, 启动数据发送端:
```
./testClient 192.168.1.100 5555
```

Linux(x86)中运行, 启动数据接收端:
```
./testServer 192.168.1.10 5555
```

## 运行

PC端运行
```
./ZMQServerUI 192.168.1.10 5555
```
Matrix端运行:
```
./sdk_sdk_zmq ../etc/global.json
```
即可从PC端接受到Matrix端发送的数据.
