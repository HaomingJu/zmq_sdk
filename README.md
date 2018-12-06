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

## Q & A

Question 1:

**Ubuntu 16.0.4 编译时出现错误, 如下:**

```
/usr/bin/ld: warning: libicui18n.so.60, need by ../libQt5Core.so not found...
/usr/bin/ld: warning: libicuuc.so.60 need by ../libQt5Core.so not found...
```

Answer 1:

往 `/etc/apt/sources.list` 文件中添加一行

```
deb http://cz.archive.ubuntu.com/ubuntu bionic main
```

然后执行:

```
sudo apt-get update
sudo apt-get install libicu60
```

参考网址: https://packages.ubuntu.com/bionic/amd64/libicu60/download

Question 2:

**Ubuntu 16.0.4 编译出现错误, 如下:**

```
libQt5Core.so: undefined reference to `getentropy@GLIBC_2.25`
libQt5Gui.so: undefined reference to `hb_font_funcs_set_nominal_glyph_func`
libQt5Gui.so: undefined reference to `hb_font_funcs_set_font_h_extents_func`
libQt5Gui.so: undefined reference to `hb_font_funcs_set_variation_glyph_func`
```

Answer 2:

```
sudo apt-get install libc6
sudo apt-get install libharfbuzz-dev
```

Question 3:

**Ubuntu 16.0.4 运行ZMQServerUI 出错, 如下:**

```
./ZMQServerUi
qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
This applicatio failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix thsi problem.

Aborted(core dumped)
```


```
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/
```
