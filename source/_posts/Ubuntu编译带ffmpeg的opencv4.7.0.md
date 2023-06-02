---
title: Ubuntu编译带ffmpeg的opencv4.7.0
toc: true
categories:
  - Technology
tags:
  - 嵌入式
date: 2023-05-26 10:00:00
---

## 系统环境

* Ubuntu 18.04.6 LTS
    * gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
    * Target: x86_64-linux-gnu
* opencv-4.7.0
* ffmpeg-4.2.9

<!--more-->

<br/>

## 编译FFMPEG

安装依赖：

```shell
sudo apt install libx264-dev
# Setting up libx264-dev:amd64 (2:0.152.2854+gite9a5903-2)
sudo apt install libx265-dev
# Setting up libx265-dev:amd64 (2.6-3)
```



 Download FFmpeg：https://ffmpeg.org/releases/ffmpeg-4.2.9.tar.xz

解压后编译：

```shell
./configure --prefix=../libffmpeg_x86 \
--disable-asm \
--disable-x86asm \
--enable-shared \
--enable-gpl \
--enable-avresample \
--enable-libx264 \
--enable-libx265 \
--enable-swscale

make -j4
make install
```

vim ../libffmpeg_x86/FFMPEGConfig.cmake

```cmake
set(ffmpeg_path ${CMAKE_CURRENT_LIST_DIR})
get_filename_component(FFMPEG_INSTALL_PATH ffmpeg_path REALPATH)

set(ENV{PKG_CONFIG_PATH} ${ffmpeg_path}/lib/pkgconfig)
find_package(PkgConfig)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavresample libavcodec libavdevice libavfilter libavformat libavutil libpostproc libswresample libswscale)

set(FFMPEG_INCLUDE_DIRS ${ffmpeg_path}/include)
set(FFMPEG_LIBRARY_DIRS  ${ffmpeg_path}/lib)
set(FFMPEG_LIBRARIES ${ffmpeg_path}/lib/libavresample.so
    ${ffmpeg_path}/lib/libavcodec.so
    ${ffmpeg_path}/lib/libavdevice.so
    ${ffmpeg_path}/lib/libavfilter.so
    ${ffmpeg_path}/lib/libavformat.so
    ${ffmpeg_path}/lib/libavutil.so
    ${ffmpeg_path}/lib/libpostproc.so
    ${ffmpeg_path}/lib/libswresample.so
    ${ffmpeg_path}/lib/libswscale.so)
```



## 编译 OpenCV

下载地址：[Releases - OpenCV](https://opencv.org/releases/)

解压后修改 `modules/videoio/cmake/detect_ffmpeg.cmake`

```cmake

      CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${FFMPEG_INCLUDE_DIRS}"
                  "-DLINK_DIRECTORIES:STRING=${FFMPEG_LIBRARY_DIRS}"
                  "-DLINK_LIBRARIES:STRING=${FFMPEG_LIBRARIES}"
```



编译：

```sh
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release \
-D ENABLE_CXX11=ON \
-D FFMPEG_DIR=/home/easy/code/OpenSources/libffmpeg_x86 \
-D OPENCV_FFMPEG_USE_FIND_PACKAGE=ON \
-D WITH_FFMPEG=ON \
-D CMAKE_INSTALL_PREFIX=../../libopencv_x86 \
..

make -j4
make install
```



## Demo编译

cmake 示例：

```cmake
cmake_minimum_required(VERSION 3.2.0)
PROJECT (open-cv47-test)

set(CMAKE_SYSTEM_NAME linux)
set(CMAKE_HOST_SYSTEM_PROCESSOR x86_64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(ffmpeg_path /home/easy/code/OpenSources/libffmpeg_x86)
set(FFMPEG_LIBRARY_DIRS  ${ffmpeg_path}/lib)
set(CMAKE_CXX_FLAGS -L{FFMPEG_LIBRARY_DIRS})

set(opencv_path ${PROJECT_SOURCE_DIR}/libopencv_x86)
set(OpenCV_DIR ${opencv_path}/lib/cmake/opencv4/)
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(get_version demo/get_version.cpp)
target_link_libraries(get_version ${OpenCV_LIBS})
```

get_version.cpp

```c++
#include <opencv2/opencv.hpp>
#include <stdio.h>

int main(void)
{
	printf("OpenCV Version:[%d.%d.%d]\n", 
           CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION);
	return 0;
}
```

