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



## conda 常用命令

```bash
conda env list

conda deactivate
```





| 说明     | 命令                                         |
| -------- | -------------------------------------------- |
| 列出环境 | conda env list                               |
| 创建环境 | conda create -n "env_name"                   |
| 删除环境 | conda remove -n "env_name" --all             |
| 克隆环境 | conda create -n "env_name" --clone ”env_org" |
| 进入环境 | conda activate "env_name"                    |
| 退出环境 | conda deactivate                             |
|          |                                              |
|          |                                              |
|          |                                              |

