---
title: 系统移植 - ARM裸机程序
date: 2023-11-13 23:00:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

基于 Exynos 4412 的系统移植记录，ARM 裸机程序。

试验环境：

* SoC：Samsung Exynos 4412（ARM Coretex-A9 based Quad CPU）
* Board：iTOP-4412 POP 1GDDR
* Compiler：arm-linux-gnueabi-gcc（gcc version 7.5.0 (Linaro GCC 7.5-2019.12)）

<!--more-->

<br/>

## 工程代码

LED 闪烁代码目录：

* CodeSign4SecureBoot_POP/（从讯为开发板资料中拷贝）
* bare_led/
  * Makefile
  * start.S
  * led.c


### Makefile

```makefile
CROSS := arm-linux-gnueabi
POP_PRE = ../CodeSign4SecureBoot_POP

%.o : %.S
	$(CROSS)-gcc -o $@ $< -c -nostdlib

%.o : %.c
	$(CROSS)-gcc -o $@ $< -c -nostdlib

led.bin: start.o led.o
	$(CROSS)-ld -Ttext 0x0 -o led.elf $^
	$(CROSS)-objcopy -O binary led.elf led.bin
	$(CROSS)-objdump -D led.elf > led_elf.dis

img: led.bin
	cat $(POP_PRE)/E4412.S.BL1.SSCR.EVT1.1.bin $(POP_PRE)/E4412.BL2.TZ.SSCR.EVT1.1.bin $(POP_PRE)/all00_padding.bin led.bin $(POP_PRE)/E4412.TZ.SSCR.EVT1.1.bin > led_img.bin

clean:
	rm *.o *.elf *.bin *.dis  -f

```

#### gcc 编译选项

* `-c`：仅执行编译操作，不进行连接操作。
* `-nostdlib`：用于告诉编译器不连接系统标准启动文件和标准库文件。此选项常用于裸机 bootloader、 linux 内核等程序，因为它们不需要启动文件、标准库文件。
* `gcc-ld -Ttext`：链接器 ld 负责将这些段合并成最终的可执行文件或动态链接库。-Ttext 选项用于指定 .text 段在可执行文件中的起始地址。`-Ttext 0x0` 即将 .text 段的起地址设置为 0x0。
* `gcc-objcopy`：用于将目标文件的内容复制到另一个目标文件中。具体而言，objcopy的作用可以分为以下几个方面：
  * 格式转换：objcopy可以将一个目标文件的格式转换为另一个格式。例如，可以将ELF格式的目标文件转换为COFF格式，或者将COFF格式的目标文件转换为ELF格式。
  * 剪切和提取objcopy可以从一个目标文件中剪切出一部分内容，并将其保存到一个新的目标文件中。这对于从一个目标文件中提取特定的节(section)或符号(symbol)非常有用。
  * 符号表操作：objcopy可以修改目标文件中的符号表，如更改符号的名称或属性。这在一些特殊的情况下非常有用，如在程序调试过程中修改符号信息。
  * 调试信息处理objcopy可以提取、剪切、合并或删除目标文件中的调试信息。这对于调试过程中的符号跟踪、代码优化或大型软件项目的构建很有帮助。

* `gcc-objcopy -O binary <input_file> <output_file>`：使用这个命令可以将目标文件（通常是可执行文件或者目标文件）转换成为一个纯粹的二进制文件，不包含任何元数据和头部信息（例如程序入口点、符号表等），只包含代码和数据的字节流。这个二进制文件通常可以用于嵌入式系统、固件更新等特应用中。

<br/>

### start.S

```
//#define CONFIG_SYS_ICACHE_OFF  

.global _start

_start:
	//disable watch dog (set 0x10060000 == 0)
	ldr r0, =0x10060000
	mov r1, #0
	str r1, [r0]

	//turn on icache
	mrc	p15, 0, r0, c1, c0, 0
	//bic r0, r0, #0x00002300	/* clear bits 13, 9:8 (--V- --RS) */
	//bic r0, r0, #0x00000087	/* clear bits 7, 2:0 (B--- -CAM) */
	//orr r0, r0, #0x00000002	/* set bit 2 (A) Align */
	//orr r0, r0, #0x00001000	/* set bit 12 (I) I-Cache */

#ifdef CONFIG_SYS_ICACHE_OFF
	// clear bit 12 (I) I-cache
	bic r0, r0, #0x00001000
#else
	// set bit 12 (I) I-cache
	orr r0, r0, #0x00001000
#endif
	mcr	p15, 0, r0, c1, c0, 0
	//mcr p15, 0, r0, c7, c5, 0	@ invalidate icache

	//set stack
	ldr sp, =0x02050000

	bl led_blink

// endless loop
halt:
	b halt

```

#### 代码分析

* `.global _start`：`.globl` 定义一个全局标志，表明包含该标志的文件链接的任何文件都可以使用该符号，否则，仅在定义标志的文件中可见。

* `ldr{<size>}  Rx,=<immediate|symbol>`：Load register，从内存拷贝 指定大小 到寄存器。同样用法的另一个指令 `str` 用于从寄存器拷贝到内存。指定大小：

  * b - unsigned byte
  * h - unsigned half-word
  * sb - signed byte
  * sh - signed half-word
  * sw - signed word

* `#0`：立即数寻址方式，代表数字0

* `[r0]`：以 r0 为地址的内存地址

* `mrc <pnum>, <opcode1>, <Rd>, <CRn>, <CRm>, <opcode2>`：Move to Register from Coprocessor，用于从协处理器寄存器（如CP15）中读取一个32位的数据。反向操作使用指令 `mcr`。其中：

  - `<pnum>`：协处理器的编号

  - `<opc1>`：指定操作类型

  - `<Rd>`：目标寄存器，用于接收来自协处理器的数据

  - `<Cp>`：协处理器地址

  - `<Cn>`：寄存器地址，用于选择协处理器的寄存器组

  - `<Cm>`：寄存器地址，于选择协处理器解码的寄存器

    `mrc	p15, 0, r0, c1, c0, 0` 将CP15的控制寄存器（c1）的值读入ARM寄存器R0

* `bic {S} Rd, Rn, Operand2 `：（Bit Clear）指令用于将寄存器中的指定位清零。将 `Rn` 中的指定位清零，并将结果存储在 `Rd` 中。操作数 `Operand2` 中指定了要清零的位。其中：

  * `{S}`：可选项，指定是否更新条件码。
  * `Rd`：目标寄存器，存储清零结果。
  * `Rn`：源寄存器，存储待清零的值。
  * `Operand2`：操作数，可以是立即数、寄存器或寄存器移位操作数。

* `orr Rd, Rn, Operand2`：该指令的作用是将 `Rn` 寄存器的值与 `Operand2` 的值进行逻辑或操作，结果存储在 `Rd` 寄存器中。这可以用来设置特定的位，或者将一组位的某些位置为1。

* 所有逻辑操作指令：

  * and - Bitwise AND
  * bic - Bitwise Bit Clear
  * eor - Bitwise Exclusive OR
  * eon - Bitwise Exclusive OR NOT
  * orr - Bitwise OR
  * orn - Bitwise OR NOT, and
  * mvn - Bitwise NOT

* `b/bl`：b（branch）指令用于无条件地跳转到一个指定的地址或标签。bl（branch with link）指令也是用于跳转，但不同于b指令的是，bl指令在跳转之前会先将当前指令的下一条指令的地址保存到链接寄存器（LR）中，这样在跳转完成后，程序可以使用bx（或者mov + bx）指令将链接寄存器（LR）的值加载到程序计数器（PC）中，返回到原来的位置。bl指令通常用于实现函数调用。

#### 相关寄存器

![WTCON_register](/resources/porting/WTCON_register.png)



<br/>

### led.c

```c
#define	GPL2CON	(*(volatile unsigned long *) 0x11000100)
#define	GPL2DAT	(*(volatile unsigned long *) 0x11000104)

#define	GPK1CON	(*(volatile unsigned long *) 0x11000060)
#define	GPK1DAT	(*(volatile unsigned long *) 0x11000064)

//GPL2_0, GPK1_1

void delay(int r0)
{
    volatile int count = r0;
    while (count--)
        ;
}

void led_blink()
{
	GPL2CON = 0x00000001;
	GPK1CON = 0x00000010;
	
	while(1)
	{
		GPL2DAT = 1;
		GPK1DAT = 0;
		delay(0x80000);
		GPL2DAT = 0;
		GPK1DAT = 0x2;
		delay(0x80000);
	}
}

```

#### 相关寄存器

![GPL2CON](/resources/porting/GPL2CON.png)

![GPL2CON0](/resources/porting/GPL2CON0.png)

![GPL2DAT](/resources/porting/GPL2DAT.png)

![GPK1CON](/resources/porting/GPK1CON.png)

![GPK1CON1](/resources/porting/GPK1CON1.png)

![GPK1DAT](/resources/porting/GPK1DAT.png)

<br/>

### 编译

```sh
make img
```

## 汇编直接点灯

修改 Makefile

```makefile
led.bin: start.o# led.o
```

修改 start.S（仅点亮 LED）

```

	b	led2_light


#define GPL2CON 0x11000100
#define GPL2DAT 0x11000104

led2_light:
	ldr r0, =GPL2CON
	ldr r1, =0x00000001
	str r1, [r0]

	ldr r0, =GPL2DAT
	ldr r1, =0x1
	str r1, [r0]

halt:
	b halt
```

<br/>

## 烧写

```sh
# 插入 SD 卡，显示为 /dev/sdb
sudo dd iflag=dsync oflag=dsync if=led_img.bin of=/dev/sdb seek=1
```

