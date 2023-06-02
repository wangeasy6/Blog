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

* 芯片：RV1126
* 工具链：gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf

编译环境：

* Ubuntu 18.04.6 LTS
* x86_64-linux-gnu

<!--more-->

<br/>

### 编译 FFMPEG

 Download FFmpeg：https://ffmpeg.org/releases/ffmpeg-4.2.9.tar.xz

编译 ffmpeg：

```shell
tar xvfJ ffmpeg-4.2.9.tar.xz
cd ffmpeg-4.2.9
mkdir ../ffmpeg-4.2.9_build
./configure \
    --prefix=../ffmpeg-4.2.9_build \
    --enable-shared \
    --disable-static \
    --enable-libmp3lame \
    --enable-libopencore-amrnb \
    --enable-libopencore-amrwb \
    --enable-libtheora \
    --enable-libvorbis \
    --enable-libxvid \
    --enable-libx264 \
    --enable-libx265 \
    --enable-libv4l2 \
    --enable-avresample \
    --enable-swscale \
    --enable-avformat \
    --enable-avcodec \
    --enable-libopenjpeg \
    --enable-libdc1394 \
    --enable-nonfree \
    --enable-gpl \
    --enable-version3 \
    --enable-cross-compile \
    --arch=arm \
    --target-os=linux \
    --cross-prefix=/home/easy/code/RV1126/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf- \
    --shlibdir=/home/easy/code/RV1126/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/lib

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

### 编译 OpenCV

官方下载地址：

[Releases · opencv/opencv (github.com)](https://github.com/opencv/opencv/releases)

[Release 4.7.0 · opencv/opencv_contrib (github.com)](https://github.com/opencv/opencv_contrib/releases/tag/4.7.0)

镜像下载：

https://www.raoyunsoft.com/opencv/opencv-4.7.0/opencv-4.7.0.tar.gz

https://www.raoyunsoft.com/opencv/opencv_contrib/opencv_contrib-4.7.0.zip

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
-D FFMPEG_DIR=../../libffmpeg_1126 \
-D WITH_FFMPEG=ON \
-D WITH_1394=OFF \
-D CMAKE_INSTALL_PREFIX=../../libopencv_1126 \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.7.0/modules \
..

```

-D BUILD_opencv_world=ON：只生成一个统一的lib，包含所有模块，比较方便配置环境。

-D EIGEN_DIR：指定包含 Eigen3Config.cmake 的目录

-D FFMPEG_DIR：指定包含 FFMPEGConfig.cmake 的目录

### 交叉编译 Eigen

官网：[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)

下载：https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz

vim toolchain.cmake

```cmake
set(TOOLCHAIN_PRE ~/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PRE}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PRE}g++)
```

编译：

```
mkdir -p build && cd build && mkdir ../../libeigen
cmake -D CMAKE_TOOLCHAIN_FILE=toolchain.cmake -D CMAKE_INSTALL_PREFIX=../../libeigen ..
make install
```

