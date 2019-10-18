---
title: C++练习 太阳系行星系统模拟
date: 2019-07-22 14:19:04
tags:
---

## Solar System Simulator Made By OpenGL

**OpenGL**（英语：Open Graphics Library，译名：开放图形库或者“开放式图形库”）是用于渲染2D、3D矢量图形的跨语言、跨平台的应用程序编程接口（API）。

**GLUT**（英文：OpenGL Utility Toolkit）是一个OpenGL API的一个具体实现及扩展的工具库，支持跨平台开发。



### 1. Set Up freeglut With MinGW

1. [Install MinGW](<http://www.mingw.org/wiki/Getting_Started>)
   1. download [mingw-get-setup.exe](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe)
   2. double click on it to start the installer（The preferred installation target directory is **C:\MinGW**）
   3. Use “MinGW Installation Manager” to install ”g++“
   4. And “C:\MinGW\bin” to "Path"
2. [Set Up GLUT](<https://www.transmissionzero.co.uk/computing/using-glut-with-mingw/>)
   1. download [freeglut MinGW package](https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW.zip)
   2. Copy the “lib\” and “include\” and "bin\\" folders from the zip archive to "C:\MinGW".(Preferred Use System32)
3. Compiling
   1. `gcc -o example.exe example.o -lfreeglut -lopengl32 -Wl,--subsystem,windows`

### 2. Code

* [GLUT API](<https://www.opengl.org/resources/libraries/glut/spec3/node1.html>)
* [OpenGL4 API](<https://www.khronos.org/registry/OpenGL-Refpages/gl4/>)

