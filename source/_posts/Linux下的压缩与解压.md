---
title: Linux下的压缩与解压
date: 2023-01-04 15:35:12
toc: true
categories:
  - Technology
tag:
  - Tools
---

Linux中常见压缩格式的压缩与解压方法。

### 测试环境

* Ubuntu 22.04.1 LTS
* x86_64

<!--more-->

### 主要格式简介

* .gz：GNU自由软件的文件压缩程序，Gzip的基础是DEFLATE，DEFLATE是LZ77与哈夫曼编码的一个组合体。

* .bz：Bzip压缩算法合并了块排序压缩算法（Burrows-Wheeler）和行程长度编码（RLE），尽管压缩速度不如Gzip（.gz文件）快，但压缩大小比Gzip小。

* .xz：采用LZMA SDK压缩，目标文件较.gz文件小30%，较.bz2小15%。

* .zip：ZIP也使用DEFLATE算法，而且可移植性更好，不需要一个外部的归档工具就可以包容多个文件。但是，由于ZIP对每个文件进行单独压缩而没有利用文件间的冗余信息（即固实压缩），所以ZIP的压缩率会稍逊于gz压缩包。Windows系统内置对zip格式的支持。

* .rar：RAR是一种专利文件格式，比ZIP格式压缩率高。

### .tar

打包： tar cvf FileName.tar DirName
解包： tar xvf FileName.tar
（注：tar只打包，不带压缩！）

### .gz

压缩：gzip FileName
解压1：gunzip FileName.gz
解压2：gzip -d FileName.gz
（注：gzip 只能用来压缩文件，不能压缩目录）

### .tar.gz/.tgz/.tar.tgz

压缩：tar cvfz FileName.tar.gz DirName
解压：tar xvfz FileName.tar.gz
解压（指定目录）：tar xvfz FileName.tar.gz -C DirName

### .bz2

压缩： bzip2 -z FileName
解压1：bzip2 -d FileName.bz2
解压2：bunzip2 FileName.bz2
（注：bzip2 只能用来压缩文件，不能压缩目录）

### .tar.bz2

压缩：tar cvfj FileName.tar.bz2 DirName
解压：tar xvfj FileName.tar.bz2

### .tar.xz

压缩：tar cvfJ FileName.tar.xz DirName
解压：tar xvfJ FileName.tar.xz

### .Z

安装：sudo apt-get install ncompress
卸载：sudo apt-get remove ncompress

压缩：compress FileName
解压：uncompress FileName.Z

### .tar.Z

压缩：tar cvfZ FileName.tar.Z DirName
解压：tar xvfZ FileName.tar.Z
（注：要安装 ncompress）

### .zip

压缩：zip FileName.zip DirName/*
解压：unzip -O CP936 FileName.zip [-d DirName]

-O 用于指定编码，CP936 支持中文编码

### .rar

安装：sudo apt-get install rar unrar
卸载：sudo apt-get remove rar unrar

压缩：rar a FileName.rar DirName
解压：rar x -y FileName.rar [DirName]

Tips：解压到指定的目录，要先创建目录


### 参考

* [tar 打包压缩目录](https://www.iteye.com/blog/verytiny-165653)