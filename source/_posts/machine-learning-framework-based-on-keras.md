---
title: 基于Keras的机器学习框架
toc: true
categories:
  - tech
tags:
  - AI
date: 2021-09-22 14:03:08
---

<img src="/resources/KerasFlow/Keras.jpeg" width=278 height=278 />

作为非专业学习人员，我的计划是先做脚本小子，然后再从具体的模块伸下去研究理论。

目前选定的是使用Keras做前端，TensorFlow做后端，偶尔使用sk-learn来研究原理。

那么，首先基于Keras，从大的框架上来认识一下机器学习，先俯瞰这个森林，再去找具体的树木。

<!--more-->

<br/>

## 机器学习的一般流程

![机器学习的一般流程](/resources/KerasFlow/机器学习的一般流程.svg)

## Keras流程图

Keras以模型（Model）为中心展开工作，定义模型，训练模型，使用模型。模型提供的其实是一个输入到输出的映射。

所以Keras主要处理以上的后4步，每一步都有对应的对象/函数与之对应：

1. 选择模型 —— model
2. 设置模型参数 —— add layers，compile
3. 训练 —— fit
4. 评估 —— evaluate
5. 预测 —— predict、model.save、models.load_model

![Keras流程图](/resources/KerasFlow/Keras流程图.svg)

上图中表达了以下内容：

1. 一个模型（Model）由一个或多个层（Layer）组成
2. 一个层由一个张量输入 - 张量输出计算函数（激活函数-activation）和一些状态组成，这些状态保存在张量流变量（层的权重W）中。
3. 模型的输入（Input_X） = Layer_1的输入
4. 上一层的输出（Out）即为下一层的输入（In）
5. 模型的输出（Output） = Layer_last的输出
6. 模型训练（Fit）过程为根据输出（Output）和标准输出（Input_Y）之间的Loss、Metrics，再根据优化器优化层（Layer）的权重，Loss、Metrics、Optimizer通过Compile函数指定。

## Keras数据使用场景

根据使用的不同，可以把数据通过模型的情况分为3个不同的场景：

![Keras数据流](/resources/KerasFlow/Keras数据流.svg)



