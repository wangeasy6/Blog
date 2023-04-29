---
title: jupyter-notebook
toc: true
categories:
  - Technology
tags:
  - Python
---

Jupyter Notebook是一个开源的web应用程序，允许您创建和共享包含实时代码、方程式、可视化和叙述文本的文档。用途包括：数据清理和转换、数值模拟、统计建模、数据可视化、机器学习等。 —— [Jupyter Notebook官方介绍](https://jupyter.org/)

<!--more-->

<br/>

## 安装

基于pip安装：

```shell
pip install notebook
```

基于conda安装：

```shell
conda install -c conda-forge notebook
```

基于mamba安装：

```shell
mamba install -c conda-forge notebook
```

## 使用

### 启动

```shell
jupyter notebook
```

### Notebook结构

Notebook由一系列的**单元**构成。

**单元**是一个多行文本输入框。Notebook有3种**单元**格式：代码单元（code cells）、Markdown单元（markdown cells）、原文本单元（raw cells）。

每个单元初始时均为代码单元，可以通过工具栏中的下拉框改变单元格式，也可以通过快捷键（Y/M/R）来切换单元格式（代码/MD/原生）。

可以通过`Ctrl+Enter`/工具栏的“Play”按钮/菜单栏的“Run”按钮，来执行代码或者渲染Markdown。（`Shift+Enter`代表执行并选择下一单元；`Alt+Enter`代表执行并添加下一单元）

### 绘制图像不能交互

在绘图的前面添加如下语句即可打开交互式会话绘图界面。

`%matplotlib notebook`