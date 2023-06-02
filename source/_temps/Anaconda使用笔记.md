---
title: Anaconda使用笔记
toc: true
categories:
  - null
tags:
  - null
---

[Anaconda](https://www.anaconda.com/) 是一个包含250+的科学包及其依赖项的发行版本，可以用于包管理器和环境管理。其包含的科学包包括：python, conda, numpy, scipy, ipython notebook等。

[conda](https://conda.io/en/latest) 是包及其依赖项和环境的管理工具。

<!--more-->

<br/>

## Anaconda 安装


### 换中科大源地址

```sh
conda config --add channels https://mirrors.ustc.edu.cn/anaconda/pkgs/main/
conda config --add channels https://mirrors.ustc.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes

conda config --remove channels url

conda config --show-channels

conda config --show-sources
```

## conda 常用命令

| 说明                         | 命令                                         |
| ---------------------------- | -------------------------------------------- |
| 列出环境                     | conda env list                               |
| 创建环境                     | conda create -n "env_name"                   |
| 创建环境并指定一个Python版本 | conda create -n "env_name" python==3.7       |
| 删除环境                     | conda remove -n "env_name" --all             |
| 克隆环境                     | conda create -n "env_name" --clone ”env_org" |
| 进入环境                     | conda activate "env_name"                    |
| 退出环境                     | conda deactivate                             |
|                              |                                              |
|                              |                                              |
|                              |                                              |


