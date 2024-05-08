---
title: 嵌入式工程编译模板
toc: true
categories:
  - Technology
tags:
  - 嵌入式
date: 2023-05-26 11:11:00
---

![compile.png](/resources/Cover/compile.png)

嵌入式项目中多用 makefile 或者 cmake 来编译代码，基于这两种自动化编译框架，记录一个模板，以供日后使用。

<!--more-->

<br/>

## Makefile

### 源码编译框架

源码目录结构：

```
/
	ai_lib/
		include/ai.h
		lib/libai.so
	libopencv/*
	src/
		test.cpp
	CMakeLists.txt
```

Makefile

```makefile
TOOLCHAIN_PRE:=~/aarch64-mix210-linux/bin/
CROSS   :=aarch64-mix210-linux-
CC      :=$(TOOLCHAIN_PRE)$(CROSS)gcc
CXX     :=$(TOOLCHAIN_PRE)$(CROSS)g++

.PHONY: test clean

OPENCV_INC        := -I./libopencv/include/opencv4
OPENCV_LIB_PATH   := ./libopencv/lib
OPENCV_LIB_LD     := -L${OPENCV_LIB_PATH}
OPENCV_LIBS := $(subst .so,,$(subst lib,-l,$(subst $(OPENCV_LIB_PATH)/,,$(wildcard ${OPENCV_LIB_PATH}/*.so))))
CXXFLAGS    := -fPIC

test: src/test.cpp
	$(CXX) $(OPENCV_INC) $(OPENCV_LIB_LD) $(OPENCV_LIBS) $^ -o $@ $(CXXFLAGS)
	
clean:
	@rm test
```

### 编译开源库

help：`./configure --help`

```sh
./configure --prefix=/home/user/build_out --host=arm-linux CC=arm-linux-gnueabihf-gcc --enable-shared
make -j4
make install
```

--prefix：安装目录，一般要求一个绝对路径

--host：目标板环境，需要与交叉编译工具链匹配

--build：当前环境，与主机编译工具链匹配

CC：指定交叉编译工具

--enable-shared：编译动态库

## CMake

### 源码编译框架

源码目录结构：

````
/
	ai_lib/
		include/ai.h
		lib/libai.so
	libopencv/*
	src/
		test.cpp
	CMakeLists.txt
````

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.2.0)

# Set toolchain
set(TOOLCHAIN_PRE gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PRE}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PRE}g++)

PROJECT (test)

# Import OpenCVConfig.cmake
set(OpenCV_DIR ${PROJECT_SOURCE_DIR}/libopencv/lib/cmake/opencv4/)
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add lib
include_directories(${PROJECT_SOURCE_DIR}/ai_lib/include)
link_directories(${PROJECT_SOURCE_DIR}/ai_lib/lib)
set(AI_LIBS ai)

# Add source
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${AI_LIBS} ${OpenCV_LIBS})
```

### 编译开源库

```sh
mkdir build && cd build
# help：cmake .. --help
cmake -D CMAKE_TOOLCHAIN_FILE=toolchain.cmake \
-D CMAKE_INSTALL_PREFIX=./build_out \
..
```

toolchain.cmake

```cmake
CC=arm-linux-gnueabihf-gcc
LD=arm-linux-gnueabihf-gcc –o
LDSHARED=arm-linux-gnueabihf-gcc -shared -Wl,-soname,libz.so.1,--version-script,zlib.map
AR=arm-linux-gnueabihf-ar
RANLIB=arm-linux-gnueabihf-ranlib
```



