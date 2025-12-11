---
title: 交叉编译带FFMPEG的OpenCV
toc: true
categories:
  - tech
tags:
  - 嵌入式
date: 2023-06-05 13:50:00
---

编译OpenCV版本：

* OpenCV 4.7.0（with contribute）
* FFMPEG 4.2.9（with x264）
* Eigen 3.4.0

交叉编译目标：

* 芯片：SD3403
* 工具链：aarch64-mix210-linux-gcc
* 版本：gcc version 7.3.0 (HC&C V1R3C00SPC200B042_20221123)

编译环境：

* Ubuntu 18.04.6 LTS
* x86_64-linux-gnu

<!--more-->

<br/>

### 编译 zlib

如果没有此库，编译 GPAC ./configure 会输出：`error: zlib not found on system or in local libs`

Download ：[Index of /fossils (zlib.net)](http://www.zlib.net/fossils/)

https://www.zlib.net/fossils/zlib-1.2.8.tar.gz

```sh
cd zlib-1.2.8
export CC=aarch64-mix210-linux-gcc
./configure --prefix=../libz_3403

make
make install

# 拷贝库到交叉编译器的 usr 目录下
cd ../libz_3403
sudo cp -r sudo cp -r include/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/include/
sudo cp -r lib/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/lib/
```

<br/>

### 编译 GPAC

如果没有此库，编译 libx264 ./configure 会输出：`mp4:           no`

Download ：[Downloads | GPAC (imt.fr)](https://gpac.wp.imt.fr/downloads/)

`git clone https://github.com/gpac/gpac.git`

```shell
cd gpac
./configure --enable-pic --prefix=../libgpac_3403 --cross-prefix=aarch64-mix210-linux-

# 解决 relocation 问题（见后文）
vim config.mak
# 添加 CFLAGS=-fPIC

make
make install

# 拷贝库到交叉编译器的 usr 目录下
cd ../libgpac_3403
sudo cp -r include/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/include/
sudo cp -r lib/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/lib/
```

问题详情：

```
make[1]: Entering directory '/home/easy/code/OpenSources/3403/gpac/src'
/opt/linux/x86-arm/aarch64-mix210-linux/host_bin/../lib/gcc/aarch64-linux-gnu/7.3.0/../../../../aarch64-linux-gnu/bin/ld: media_tools/mpd.o: relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `stdout@@GLIBC_2.17' which may bind externally can not be used when making a shared object; recompile with -fPIC
/opt/linux/x86-arm/aarch64-mix210-linux/host_bin/../lib/gcc/aarch64-linux-gnu/7.3.0/../../../../aarch64-linux-gnu/bin/ld: media_tools/mpd.o(.text+0xe6c0): unresolvable R_AARCH64_ADR_PREL_PG_HI21 relocation against symbol `stdout@@GLIBC_2.17'
/opt/linux/x86-arm/aarch64-mix210-linux/host_bin/../lib/gcc/aarch64-linux-gnu/7.3.0/../../../../aarch64-linux-gnu/bin/ld: final link failed: bad value
collect2: error: ld returned 1 exit status
Makefile:382: recipe for target '../bin/gcc/libgpac.so' failed
```

<br/>

### 编译 libx264

如果没有此库，编译 FFMPEG ./configure 会输出：`ERROR: libx264 not found`

Download ：http://download.videolan.org/x264/snapshots/x264-snapshot-20191217-2245.tar.bz2

```sh
tar xfj x264-snapshot-20191217-2245.tar.bz2
cd x264-snapshot-20191217-2245
# 修改 configure，打开 mp4 支持
vim configure
# mp4="yes"

./configure --prefix=../libx264_3403 --enable-shared --enable-pic --host=aarch64-linux --cross-prefix=aarch64-mix210-linux-

make
make install

# 拷贝库到交叉编译器的 usr 目录下
cd ../libx264_3403
sudo cp -r include/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/include/
sudo cp -r lib/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/lib/
```

<br/>

### 编译 FFMPEG

 Download：https://ffmpeg.org/releases/ffmpeg-4.2.9.tar.xz

编译 ffmpeg：

```shell
tar xvfJ ffmpeg-4.2.9.tar.xz
cd ffmpeg-4.2.9
mkdir ../libffmpeg_3403
./configure \
--prefix=../libffmpeg_3403 \
--enable-shared \
--enable-static \
--enable-protocol=tcp \
--enable-network \
--enable-protocol=udp \
--enable-demuxer=rtsp \
--enable-demuxer=rtp \
--enable-libx264 \
--enable-avresample \
--enable-swscale \
--enable-avformat \
--enable-avcodec \
--enable-nonfree \
--enable-gpl \
--enable-version3 \
--enable-cross-compile \
--arch=arm64 \
--target-os=linux \
--extra-cflags=-fPIC \
--cross-prefix=aarch64-mix210-linux-

make -j4
make install

# 拷贝库到交叉编译器的 usr 目录下
cd ../libffmpeg_3403
sudo cp -r include/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/include/
sudo cp -r lib/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/lib/
```

<br/>

### 交叉编译 Eigen

官网：[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)

Download：https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz

vim toolchain_3403.cmake

```cmake
set(TOOLCHAIN_PRE aarch64-mix210-linux-)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm64)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PRE}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PRE}g++)
```

编译：

```shell
mkdir -p build && cd build && mkdir ../../libeigen_3403
cmake -D CMAKE_TOOLCHAIN_FILE=../../toolchain_3403.cmake -D CMAKE_INSTALL_PREFIX=../../libeigen_3403 ..
make install
```

<br/>

### 编译 OpenCV

 Download：[Releases - OpenCV](https://opencv.org/releases/)

官方下载地址：

[Releases · opencv/opencv (github.com)](https://github.com/opencv/opencv/releases)

镜像下载：

https://www.raoyunsoft.com/opencv/opencv-4.7.0/opencv-4.7.0.zip

https://www.raoyunsoft.com/opencv/opencv_contrib/opencv_contrib-4.7.0.zip

编译：

```shell
mkdir -p build && cd build
#FFMPEG 库使用了 pkgpackge
export PKG_CONFIG_LIBDIR=$PWD/../../libffmpeg_3403/lib/pkgconfig/
cmake -D CMAKE_TOOLCHAIN_FILE=../../toolchain_3403.cmake \
-D CMAKE_BUILD_TYPE=Release \
-D ENABLE_CXX11=ON \
-D BUILD_opencv_world=ON \
-D WITH_EIGEN=ON \
-D OPENCV_FFMPEG_USE_FIND_PACKAGE=ON \
-D WITH_FFMPEG=ON \
-D WITH_1394=OFF \
-D CMAKE_INSTALL_PREFIX=../../libopencv_3403 \
-D BUILD_opencv_gapi=OFF \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.7.0/modules \
..

make
make install
```

-D BUILD_opencv_world=ON：只生成一个统一的lib，包含所有模块，比较方便配置环境。

-D BUILD_opencv_gapi=OFF：`ade/util/assert.hpp: No such file or directory`，tool-chain doesn't support ADE，so disabled。

<br/>

### 测试

```c++
# read_video_test.cpp
#include <iostream>
#include "opencv2/videoio.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void)
{
    VideoCapture cap("test.mp4");
    if(!cap.isOpened())
    {
        cout << "read: test.mp4 failed." << endl;
        return 0;
    }
    cout << "read: test.mp4" << endl;

    Mat frame;
    cap >> frame;
    imwrite("test_frame_0.png", frame);
    cout << "output: test_frame_0.png" << endl;

    cap.release();
    return 0;
}
```

Makefile

```makefile
OPENCV_INC        := -I./libopencv_3403/include/opencv4
OPENCV_LIB_LD     := -L./libopencv_3403/lib
OPENCV_LIBS       := -lstdc++ -lm -lopencv_world

read_video_test: read_video_test.cpp
	$(CC) $(OPENCV_INC) $(OPENCV_LIB_LD) $(OPENCV_LIBS) $^ -o $@
```

<br/>



