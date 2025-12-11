---
title: STM32控制8位数码管（74HC595）
date: 2025-10-08 23:10:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

74HC595 驱动原理 + STM32 控制代码。

试验环境：

* 8 位数码管（3641BS x 2 + 74HC595 x 2）
* 主控芯片：STM32F103C6T6A

<!--more-->

<br/>

## 74HC595 驱动原理

### 电路分析

#### 3461BS

![3641BS](/resources/hardware-circuit/3641BS.png)

3461BS 是一个 4 位数码管，有 4 位数字，每个数字由 8 个 LED（发光二极管） 的亮灭控制显示。

由于发光二极管具有单向导电性，所以多位数码管分为 共阳（3461BS） 和 共阴（3461AS）2 种类型。

共阳则设置 6/8/9/12 为高电平，对应 LED 为低电平时，LED 发光。共阴则高低电平相反。

数字示例：数字 0：只有 g、h 为 1，其余全为 0；数字 1：只有 b、c 为 0，其余全为 1。

Tips：本篇电路图示中的 `h=DF=DP`。

<br/>

#### 74HC595

74HC595 连接 3461xS 的电路：

![74HC595](/resources/hardware-circuit/74HC595-3461.png)

如上所示，如果想要第一位数码管显示数字0，则需要U14 的输出为 0xC0（从高位到低位扫描：1100 0000），设置 U13 的输出为：0x01。

74HC595 是一个由 16 个 D型触发器 组成的 CMOS 移位寄存器。内部电路如下：

![74HC595-inner](/resources/hardware-circuit/74HC595-inner.jpg)

- D触发器是指一种具有数据（D）输入端、时钟（CLK）输入端和输出端（Q）的电路。它在时钟信号的上升沿到来时，将数据端的值锁存到输出端，其他时间保持输出状态不变。
- $Q_0 - Q_7$ 对应 $QA - QH$。

由于多位触发器按位存入，先入者在高位，所以按照时序将 0xC0 存入 U14 的时序如下：

![74HC595-led0](/resources/hardware-circuit/74HC595-led0.png)

按时序将 0xC0 存入 U14 动画如下：

![74HC595-led0](/resources/hardware-circuit/74HC595-set-led0.gif)

<br/>

## 物理连接

| STM32 | 数码管 |
| ----- | ------ |
| 3.3V  | VCC    |
| GND   | GND    |
| PA7   | DIO    |
| PA4   | RCLK   |
| PA5   | SCLK   |

<br/>

## STM32 驱动

```c
// 关键函数
#define MOSIO GPIO_PIN_7  //=PA7
#define S_CLK GPIO_PIN_5  //=PA5
#define R_CLK GPIO_PIN_4  //=PA4

// [0 - 9] + [A - F]
unsigned char LED7Code[] = { ~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D,
		~0x07, ~0x7F, ~0x6F, ~0x77, ~0x7C, ~0x39, ~0x5E, ~0x79, ~0x71 };

// Mask 中使能的位置显示为 SendVal
void HC595SendData8(unsigned char SendVal, unsigned char Mask) {
	unsigned char i;

	for (i = 0; i < 8; i++) {
		if ((SendVal << i) & 0x80)
			GPIO_SetBits(GPIOA, MOSIO);
		else
			GPIO_ResetBits(GPIOA, MOSIO);

		GPIO_ResetBits(GPIOA, S_CLK);
		NOP();
		NOP();
		GPIO_SetBits(GPIOA, S_CLK);
	}

	for (i = 0; i < 8; i++) {
		if ((Mask << i) & 0x80)
			GPIO_SetBits(GPIOA, MOSIO);
		else
			GPIO_ResetBits(GPIOA, MOSIO);

		GPIO_ResetBits(GPIOA, S_CLK);
		NOP();
		NOP();
		GPIO_SetBits(GPIOA, S_CLK);
	}

	GPIO_ResetBits(GPIOA, R_CLK);
	NOP();
	NOP();
	GPIO_SetBits(GPIOA, R_CLK);   //片选
}

void HC595SendData1(unsigned char SendVal, unsigned char n) {
	HC595SendData8(SendVal, 0x80>>n);
}

int main(){
	unsigned char Mask;
	unsigned short n;
    while (1) {
        // 依次显示 1-8 个 1-8
        Mask = 0;
		for (unsigned char i = 0; i < 8; i++) {
			Mask += 0x80>>i;
			HC595SendVal = LED7Code[i+1];
			HC595SendData8(HC595SendVal, Mask);
			Delay(0x3FFFFF);
		}

        // 在对应的位置显示对应的数字
		n = 0x7FF;
		while(n--){
			for (unsigned char i = 0; i < 8; i++) {
				HC595SendVal = LED7Code[i+1];
				HC595SendData1(HC595SendVal, i);
			}
		}
    }
}
```

实际运行效果：

![8bit-led-loop](/resources/hardware-circuit/8bit-led-loop.gif)

<br/>

## 参考

* [基于FPGA的74HC595数码管驱动 - 知乎](https://zhuanlan.zhihu.com/p/679237455)
* [第13节 国产安路FPGA 74HC595驱动控制，数码管显示控制代码介绍和板上验证_74hc595d spi-CSDN博客](https://blog.csdn.net/qq_42189951/article/details/133751496)
* [74HC595 芯片原理和 Arduino 使用实例 – Arduino 实验室](https://arduino.nxez.com/2016/12/20/74hc595-chip-principle-and-arduino-use-example.html)
* [74HC595的使用_74hc595芯片手册-CSDN博客](https://blog.csdn.net/fengidea/article/details/146426052)
* [FPGA_数码管显示（74HC595芯片）_74hc595芯片手册-CSDN博客](https://blog.csdn.net/weixin_46897065/article/details/135743823)
* 时序图绘制工具：[WaveDrom - Digital timing diagram everywhere](https://wavedrom.com/)
