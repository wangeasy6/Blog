---
title: STM32库函数开发Keil工程搭建
toc: true
categories:
  - Technology
tags:
  - 嵌入式
date: 2021-09-27 21:28:12
---

之前用STM32做过一两个小项目，但是那时还没有记笔记的习惯，现在也快忘光了。

趁着最近又要用STM32开发，记一下笔记。

<!--more-->

<br/>

## 软硬件环境

**硬件：**STM103CBT6

**IDE：**Keil 5.36

<br/>

## 库函数开发简介

基于STM32的软件开发分为3代：

第一代：寄存器开发

第二代：标准库开发（Standard Peripheral Library：简写SPL）

第三代：HAL（Hardware Abstraction Layer，硬件抽象层库）\LL（Low-Layer，底层库）库 + STM32 CubeMX工具开发  

标准库和 HAL\LL 都是遵循了 CMSIS 标准的软件抽象层。标准外设库是在寄存器的基础上进行了一次简单封装，主要是面向过程的嵌入式系统开发人员（因为现在ST官方主推STM32CubeMX，所以停止了对SPL的更新）。HAL针对的是具有一定嵌入式基础的开发人员，HAL具有很好的移植性。LL库相对HAL，具有简单的结构，针对之前从事SPL（标准外设库），或寄存器开发的人员。

此示例基于标准库。

首先需要下载 STM32F10x 标准固件库[快速下载地址](/resources/STM32/en.stsw-stm32054_v3.5.0.zip)/[官网地址](https://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html#design-scroll)。在官网地址上，在“More from the product line”这一栏下，有STM32其它系列（F0、F1、F3、F2、F4、L1）的固件下载地址。

在实际的软件开发中，可以把工程中的文件分为3层，如下所示：

<img src="/resources/STM32/stm32F1固件库文件间的关系.png" width="500" height="688"/>

其中和软件相关的就是CMSIS层和用户层。

### 什么是CMSIS？

CMSIS：Cortex Microcontroller Software Interface Standard，ARM Cortex™ 微控制器软件接口标准。

ARM 公司本身并不靠自有的设计来制造或出售 CPU，而是将处理器架构授权给有兴趣的厂家。

ST 公司生产的STM32采用的是Cortex-M内核，由于基于 Cortex 核的芯片厂商很多，不只是 ST 公司，为了解决不同厂家的 Cortex 核芯片软件兼容的问题，ARM 和这些厂家就建立了这套 CMSIS 标准。

ST公司或其他芯片生产厂商如TI，负责设计的是在内核之外的部件，如芯片内部的模数转换外设ADC、串口UART、定时器TIM等。



CMSIS 实际是建立了一个软件抽象层，通过固定函数名或者命名规范来达到软件操作的一致性。CMSIS核心层包括：

1. 内核函数层：其中包含用于访问内核寄存器的名称、地址定义，主要由ARM公司提供。
2. 设备外设访问层：包含片上的核外外设的地址和中断定义，主要由芯片生产商提供。

所以 CMSIS 层由厂商提供后，不用修改，直接包含进工程即可。

我们需要修改的主要在用户区。

<br/>

### 从0配置一个工程模板

拿到STM32固件库之后，我们就可以配置一个基于库函数的[工程模板](/resources/STM32/LedTest.zip)了。

#### 基础文件拷贝

1. 首先新建文件夹如下：

```
├─Template
│  ├─CMSIS
│  ├─FWLib
│  ├─Output
│  ├─Project
│  ├─Startup
│  └─User
```

**CMSIS** —— 存放CMSIS核心层文件。包含：stm32f10x.h、core_cm3.c、core_cm3.h、system_stm32f10x.c、system_stm32f10x.h。

**FWLib** —— 存放固件外设驱动源文件。“STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\STM32F10x_StdPeriph_Driver\”目录下包含src、inc两个目录，这两个目录中存放了固件外设源文件。向FWLib中添加需要用的外设文件。以点亮LED为例，则需要添加stm32f10x_rcc.c、stm32f10x_rcc.h、stm32f10x_gpio.c、stm32f10x_gpio.h。

**Output** —— 存放输出文件。

**Project** —— 存放工程文件。

**Startup** —— 存放启动文件。启动文件需要选择，存放在“STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm”目录下。STM103CBT6应使用：“startup_stm32f10x_md.s”

其中文件名的英文缩写如下：

1. cl：互联型产品，F105/107系列
2. vl：超值型产品，F100系列
3. xl：超高容量产品，F101/103系列
4. ld：低容量产品，Flash小于64KB
5. md：中容量产品，Flash等于64KB或128KB
6. hd：高容量产品，Flash大于128KB

**User** —— 存放用户代码。复制stm32f10x_conf.h、stm32f10x_it.c、stm32f10x_it.h到此文件夹下。

#### 建立Keil工程

1.新建工程

  * “Project” - “New uVision Project”
  * 选择Project文件夹
  * 选择芯片——STM32F103CB

2.编辑工程文件夹

  * 在”Target 1“右键，Manage Project Items，打开管理窗口
  * 在 Groups 中，新建User、CMSIS、FWLib、Startup文件夹
  * 依次添加User、CMSIS、FWLib、Startup中的文件到对应文件夹，Ok保存退出

3.修改配置

  * “Project” - “Options for Target”，打开配置窗口
  * 在 ”Target" - “ARM Compiler”中，选择“Use default compiler version 5”
  * 在 ”Output“ 这个选项下，Select Folder for Objects，选择Output文件夹；选择 ”Create Executable“，勾选 “Create HEX File”
  * 在 ”C/C++（AC6）“ 这个选项下，在Define中填入：USE_STDPERIPH_DRIVER；在Include Paths这一行，编辑为：..\CMSIS;..\User;..\FWLib

4.编写用户代码

  * 编辑 ”stm32f10x_conf.h“，将Includes下我们未使用的外设注释
  * 在 ”User“ 右键，Add New Item to Group，选择 .c 文件，命名为main
  * 在main.c中，开始编写我们的主程序（以闪烁LED为例）：
   ```c
   #include "stm32f10x.h"
   #include "stm32f10x_conf.h"
   
   static void delay(u16 n)
   {
   	int i = 0;
   	while(n--)
   	{
   		i = 10000;
   		while(i--);
   	}
   }
   
   int main(void)
   {
   
     /* GPIOD Periph clock enable */
   	GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   
     /* Configure PD0 and PD2 in output pushpull mode */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
   
     while (1)
     {
   		GPIO_SetBits( GPIOA, GPIO_Pin_0 );
   		delay( 100 );
   		GPIO_ResetBits( GPIOA, GPIO_Pin_0 );
   		delay( 100 );
     }
   }
   ```
  * 打开main.c，点击预编译，没有Errors的话再点击构建即可生成hex文件。

<br/>

## 参考

《STM32库开发实战指南》
