---
title: RV1126交叉编译带FFMPEG的OpenCV
toc: true
categories:
  - null
tags:
  - null
---

编译OpenCV版本：

* OpenCV 4.7.0（with contribute）
* FFMPEG 4.2.9（with x264）
* Eigen 3.4.0

交叉编译目标：

* 芯片：SD3403
* 工具链：aarch64-mix210-linux-gcc
* Target：aarch64-linux-gnu
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
sudo cp lib/* /opt/linux/x86-arm/aarch64-mix210-linux/target/usr/lib/
```



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



### 编译 libx264

如果没有此库，编译 FFMPEG ./configure 会输出：`ERROR: libx264 not found`

Download ：http://download.videolan.org/x264/snapshots/x264-snapshot-20191217-2245.tar.bz2

```sh
tar xfj x264-snapshot-20191217-2245.tar.bz2
cd x264-snapshot-20191217-2245
./configure --prefix=../libx264_3403 --enable-shared --enable-pic --host=aarch64-linux --cross-prefix=aarch64-mix210-linux- --enable-mp4
```



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
--enable-libxvid \
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
```

报错：

```
ERROR: libdc1394-2 not found using pkg-config
ERROR: libmp3lame >= 3.98.3 not found
ERROR: libopencore_amrnb not found
ERROR: libopencore_amrwb not found
ERROR: libopenjp2 >= 2.1.0 not found using pkg-config
ERROR: libtheora not found
ERROR: libv4l2 not found using pkg-config
ERROR: vorbis not found using pkg-config
ERROR: x264 not found using pkg-config
ERROR: x265 not found using pkg-config
ERROR: libxvid not found
```



```shell
# --enable-libx264
# 解决 ERROR: libx264 not found
# 包下载地址 http://ftp.videolan.org/pub/videolan/x264/snapshots/
tar xf x264-snapshot-20191217-2245.tar.bz2 
mkdir x264_build
cd x264-snapshot-20191217-2245
source setenv_rv1126_ffmpeg.sh
./configure --enable-shared --prefix=../x264_build --host=arm-linux-gnueabihf
make && make install
cd -
rm -f x264-snapshot-20191217-2245.tar.bz2
rm -rf x264-snapshot-20191217-2245
```





```
./configure \
    --prefix=../ffmpeg-4.2.9_build \
    --enable-shared \
    --disable-static \
    --enable-avresample \
    --enable-swscale \
    --enable-avformat \
    --enable-avcodec \
    --enable-nonfree \
    --enable-gpl \
    --enable-version3 \
    --enable-cross-compile \
    --arch=arm \
    --target-os=linux \
    --cross-prefix=arm-linux-gnueabihf-


make
make install
```

### 交叉编译 Eigen

官网：[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)

Download：https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz

vim toolchain.cmake

```cmake
set(TOOLCHAIN_PRE aarch64-mix210-linux-)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm64)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PRE}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PRE}g++)
```

编译：

```shell
mkdir -p build && cd build && mkdir ../../libeigen
cmake -D CMAKE_TOOLCHAIN_FILE=toolchain.cmake -D CMAKE_INSTALL_PREFIX=../../libeigen_3403 ..
make install
```



### 编译 OpenCV

 Download：[Releases - OpenCV](https://opencv.org/releases/)

把 libffmpeg 放到 编译器的 usr/lib 下：

```sh
cp ffmpeg-4.2.9_build/lib/* /home/easy/code/RV1126/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/libc/usr/lib
```

编译：

```shell
mkdir -p build && cd build
cmake -D CMAKE_TOOLCHAIN_FILE=toolchain.cmake \
-D CMAKE_BUILD_TYPE=Release \
-D ENABLE_CXX11=ON \
-D BUILD_opencv_world=ON \
-D EIGEN_DIR=../../libeigen_1126/share/eigen3/cmake/ \
-D WITH_EIGEN=ON \
-D OPENCV_FFMPEG_USE_FIND_PACKAGE=ON \
-D FFMPEG_DIR=/home/easy/code/OpenSources/opencv_1126/libffmpeg_1126 \
-D WITH_FFMPEG=ON \
-D WITH_1394=OFF \
-D CMAKE_INSTALL_PREFIX=../../libopencv_1126 \
..

```

-D BUILD_opencv_world=ON：只生成一个统一的lib，包含所有模块，比较方便配置环境。

-D EIGEN_DIR：指定包含 Eigen3Config.cmake 的目录

-D FFMPEG_DIR：指定包含 FFMPEGConfig.cmake 的目录

