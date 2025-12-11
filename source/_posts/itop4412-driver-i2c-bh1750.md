---
title: 驱动移植 iTOP-4412 BH1750(I2C)
date: 2024-11-07 15:52:00
toc: true
categories:
  - tech
tags:
  - 嵌入式
---

I2C 总线仅仅使用 SCL、SDA 这两根信号线就实现了设备之间的数据交互，极大地简化了对硬件资源和 PCB 板布线空间的占用。因此，I2C 总线非常广泛地应用在 EEPROM、实时钟、小型 LCD 等设备与 CPU 的接口中。

此篇通过 I2C 设备驱动移植来学习一下 Linux 设备驱动的编写。

试验环境：

* **Board:** iTOP-4412 POP 1G Elite（精英板）
* **Compiler:** arm-linux-gnueabi-gcc version 4.4.1 (Sourcery G++ Lite 2009q3-67)
* **Kernel:** linux-3.0.15
* **Root FS:** busybox-1.22.1
* **I2C Device**: BH1750

<!--more-->

<br/>

## I2C 传输

### 简介

I2C（Inter-Integrated Circuit，内部集成电路总线）是一种同步的、多主机、多从机、打包的串行通信协议，用于连接低速率外部设备到微控制器和类似的系统中。

以下是I2C总线的一些关键特点和介绍：

1. **同步通信**：I2C总线数据传输是由时钟信号同步的。

2. **多主机和多从机**：I2C总线支持多个主机（可以发送数据的设备）和多个从机（接收数据的设备）。

3. **串行通信**：I2C使用两条线进行通信：数据线（SDA）和时钟线（SCL）。所有设备共享这两条线。

4. **地址空间**：I2C协议允许每个从机设备有一个唯一的地址，这样主机就可以通过发送正确的地址来选择特定的从机进行通信。

5. **数据包传输**：I2C通信是通过数据包进行的，每个数据包包括起始条件、设备地址、读写位、数据字节和停止条件。

6. **速度**：I2C有不同的速度模式，包括标准模式（最高 100 kbit/s）、快速模式（最高 400 kbit/s）、快速模式Plus（最高 1 Mbit/s）和高速模式（最高 3.4 Mbit/s），更高速率的器件兼容低速模式。

7. **电源和地线**：除了SDA和SCL线外，I2C总线设备还需要连接电源（VCC）和地线（GND）。

8. **上拉电阻**：在SDA和SCL线上通常需要外部的上拉电阻，以确保在没有设备驱动这些线时，它们能够保持高电平。

9. **仲裁和冲突检测**：I2C协议包括仲裁机制，以解决多个主机同时尝试控制总线的情况。此外，还有冲突检测机制，以确保数据传输的完整性。

10. **扩展性**：I2C总线可以通过添加更多的从机设备来扩展，只要总线上的电容和电阻在规定的范围内。

11. **应用广泛**：I2C总线广泛应用于各种电子设备中，包括传感器、显示器、存储器、音频设备等。

I2C总线因其简单、灵活和成本效益高而被广泛采用，特别是在嵌入式系统中。

### 硬件连接

一个典型的 I2C 硬件连接如下图所示：

![I2C-connection](\resources\porting\I2C-connection1.png)

在常见的应用中，通常只有一个主设备。而 I2C 总线的多主机功能允许通过外部连接到生产线快速测试和调整最终用户的设备。

每个从设备都有一个唯一的地址识别。

### 传输时序

参见视频讲解：[一次讲透I2C总线时序图，单片机IIC通信协议底层【洋桃电子大百科P28】](https://www.bilibili.com/video/BV1RzxJeMEdy)

已知，在同一条线上，只能有2种状态：高电平 和 低电平。那么 I2C 是如何使用这 2 条线的 2 种状态来传输数据的呢？

在数字电路中，我们将高电平视为 1，将低电平视为 0。

为了避免总线信号的混乱，要求各设备连接到总线的输出端必须是开漏输出或集电极开路输出的结构。总线空闲时，上拉电阻使 SDA 和 SCL 线都保持高电平。为了使上拉电阻有效，连接到总线的器件输出级必须是漏极开路或集电极开路。根据开漏输出或集电极开路输出信号的“线与”逻辑，I2C 总线上任意器件输出低电平都会使相应总线上的信号线变低。

SDA 线上的数据必须在时钟的高电平周期保持稳定。数据线的高或低电平状态只有在 SCL 线的时钟信号是低电平时才能改变。

一个完整的数据包由 开始信号（S）、寻址命令、数据传输、答信号（ACK）、结束信号（P） 组成：

* 开始信号：SCL 为高电平时，SDA 由高电平向低电平跳变，开始传送数据。

* 结束信号：SCL 为高电平时，SDA 由低电平向高电平跳变，结束传送数据。

  ![I2C-Signal-SP](\resources\porting\I2C-Signal-SP.png)

* 在开始信号和结束信号之间以字节为单位传输数据，每个字节后必须跟一个响应位，每次传输可以发送的字节数量不受限制。数据是一位一位地进行传输，先传输高位（MSB），再传输低位（LSB）。

* 应答信号：接收设备在接收到 8bit 数据后，在第 9 个时钟周期，向发送设备发送低电平，表示成功收到数据。

  ![I2C-Signals](\resources\porting\I2C-Signals.jpeg)

总线在起始条件后被认为处于忙的状态，在停止条件的某段时间后，总线被认为再次处于空闲状态。

**设备地址**

I2C 设备用一个 7 位或 10 位的数字，唯一标识自己。在地址匹配后，从设备做出应答并执行对应操作。I2C 设备地址由固定部分和可编程部分构成。这样 I2C 总线就可以支持一个 I2C 总线上挂载多个同样的器件，而地址不同。I2C 地址的可编程部分最大数量就是可以连接到 I2C 总线上相同器件的数量。一般可编程的部分的值，由特定引脚的电器连接决定。例如，I2C 器件用 7 为地址来标识自己，有 4 个固定的和 3 个可编程的地址位，那么相同的总线上共可以连接 8 个相同的器件。

**总线寻址**

I2C 总线的寻址过程是通常在起始信号后的第一个字节决定了主机选择哪一个从机。例外的情况是可以寻址所有器件的广播地址，使用这个地址时理论上所有器件都会发出一个响应，但是也可以使器件忽略这个地址。

第一个字节的头 7 位组成了从机地址，第 8 位它决定了数据传输的方向，第一个字节的最低位是 0 表示主机会写信息到被选中的从机；1  表示主机会向从机读信息。如下图所示：

![I2C-Addr7](\resources\porting\I2C-Addr7.png)

10 位寻址和 7 位寻址兼容，而且可以混合使用。10 位寻址过程是起始信号后的头两个字节，通常决定了主机要寻址哪个从机。若第一个字节的头 5 位是 11110，则代表使用 10 位寻址，而第一个字节的 6、7 位 + 第二个字节 8 位 组成了 10 位寻址，第一个字节的 6、7 位为 10 位地址的两个最高位（MSB）。第一个字节的第 8 位是 R/W 位，决定了数据传输的方向，0 表示主机会写信息到被选中的从机；1  表示主机会向从机读信息。如下图所示：

![I2C-Addr10](\resources\porting\I2C-Addr10.png)

<br/>

## BH1750

实际使用模块：[Risym GY-302 BH1750 光强度光照度模块](https://detail.tmall.com/item.htm?_u=u1nungt9a60f&id=543135220776&spm=a1z09.2.0.0.77712e8dCIcbPw)

### 简介

BH1750是一款高精度的数字光照强度传感器，由罗姆（ROHM）半导体集团出品。以下是BH1750的一些基本特性和简介：

1. **两线式串行总线接口**：具有I2C总线接口，支持快速或标准模式（F/S模式），使得连接和使用变得简单。
2. **可选择2种I2C从地址**：支持两种I2C地址，可以通过 ADDR 引脚的电平来选择。（ADDR 引脚浮空或接地时地址为 0x23，接电源时地址为 0x5C）
3. **内置16位AD转换器**：内置了16位的模数转换器，能够将光照强度转换为数字信号，无需额外的计算。
4. **宽泛的测量范围**：光照度测量范围是1-65535lx。
5. **近似人眼的光谱反应能力**：光谱响应接近人眼，能够更准确地模拟人对光线的感知。
6. **光源依赖性小**：受红外线影响很小，适用于多种光源，如白炽灯、荧光灯、卤素灯、白光LED、太阳光等。
7. **光噪声抑制功能**：具备50Hz/60Hz光噪声抑制功能，提高测量的准确性。

BH1750 广泛应用于移动手机、LCD电视、电脑PC、掌上游戏机、数码相机、数码摄像机、汽车导航、PDA、LCD显示屏幕等设备中，用于调节背光亮度或智能灯光控制。

### 数据传输

BH1750 支持 6 种读取模式，这里选择 One Time H-Resolution Mode，命令码为 0x20。

![BH1750-InstructionSet](\resources\itop4412-porting\bh1750\BH1750-InstructionSet.png)

#### 循环单次读取流程

- Power Down：write 0x00

- Power On：write 0x01

- Reset：write 0x07

- read():

  * One Time H-Resolution：write 0x20
  * Measurement Time：sleep_ms(180)
  * read(2)
  * 计算

### 硬件连接

| BH1750      | 开发板                |
| ----------- | --------------------- |
| VCC（3-5V） | GPIO - DC3.3V (11)    |
| GND         | GPIO - GND (9)        |
| SCL         | CAMERA - I2C_SCL7 (4) |
| SDA         | CAMERA - I2C_SDA7 (2) |

开发板端口：

<img src="/resources/itop4412-porting/itop-4412-gpio.png" width=800 />

<img src="/resources/itop4412-porting/board-camera-port.png" width=600 />

实际连接图：

<img src="/resources/itop4412-porting/bh1750/bh1750-connect.jpeg" height=500 />

### 驱动程序

#### 相关知识

在 Linux 驱动架构中，和 I2C 相关的有以下 4 个方面：

```
Bus    -> Algorithm
          Adapter
Device -> Driver
          Client
```

Algorithm 驱动程序包含可用于整类I2C适配器的通用代码。每个特定的适配器驱动程序要么依赖于一个算法驱动程序，要么包含自己的实现。

Driver 驱动程序（是的，这听起来很荒谬，抱歉）包含访问某种类型设备的通用代码。每个检测到的设备在客户端结构中都有自己的数据。通常，驱动程序和客户端比算法和适配器集成得更紧密。

对于给定的配置，需要 I2C 总线的驱动程序和 I2C 设备的驱动程序（通常每个设备一个驱动程序）。

目前，Linux 仅在主模式下运行I2C（或SMBus）；不能使用这些 API 使 Linux 系统表现为从属/设备，无论是使用自定义协议还是模拟其他设备。

一个具体的I2C设备驱动由 i2c_driver 的形式进行组织，用于将设备挂接于I2C总线，组织好了后，再完成设备本身所属类型的驱动。

#### 作为模块加载

```c
// bh1750.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h> 
#include <linux/miscdevice.h>

#define DEVICE_NAME  "bh1750"
#define BH1750_I2C_ADDR 0x23

static struct i2c_board_info my_bh1750_info = {
	I2C_BOARD_INFO(DEVICE_NAME, BH1750_I2C_ADDR),
};

static struct i2c_client *bh1750_client;

static int bh1750_open_func(struct inode * ind, struct file *filp)
{
	uint8_t cmd[] = {0x00, 0x01, 0x07};
	struct i2c_msg msgs[] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[0],
		},
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[1],
		},
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[2],
		},
	};

	int ret = i2c_transfer(bh1750_client->adapter, msgs, 2);
	if (ret < 0) {
		pr_err("read reg error!\n");
		return -1;
	}

	return 0;
}

static ssize_t bh1750_read_func(struct file *filp, char __user *buffer, size_t count, loff_t *ppos){
	uint8_t readbuf[2] ={0};
	uint32_t light_intensity_32 = 0;
	uint16_t light_intensity = 0;
	
	uint8_t cmd = 0x20;

	struct i2c_msg msg_send[1] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd,
		}
	};

	struct i2c_msg msgs_read[1] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 1,
			.len	= 2,
			.buf	= readbuf,
		}
	};

	int ret = i2c_transfer(bh1750_client->adapter, msg_send, 1);
	if (ret < 0) {
		pr_err("write cmd error!\n");
		return 0;
	}

	mdelay(180);

	ret = i2c_transfer(bh1750_client->adapter, msgs_read, 1);
	if (ret < 0) {
		pr_err("read reg error!\n");
		return 0;
	}
	// printk("Read data: %x %x\n", readbuf[0], readbuf[1]);

	light_intensity_32 = (readbuf[0]<<8 | readbuf[1]) * 10 / 12;
	light_intensity = light_intensity_32;
    // printk("Lux value: %d\n", light_intensity);

	ret = copy_to_user(buffer,&light_intensity,2);
	
	return 2;
}

static struct file_operations bh1750_ops = {
	.owner 	= THIS_MODULE,
	.open 	= bh1750_open_func,
	.read 	= bh1750_read_func,
};

static struct miscdevice bh1750_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &bh1750_ops,
	.name	= DEVICE_NAME,
};

static const struct i2c_device_id bh1750_id[] = {
	{ DEVICE_NAME, 0 },
	{ }
};

static int __init bh1750_init(void)
{
	struct i2c_adapter *i2c_adap;
    i2c_adap = i2c_get_adapter(7);
    printk("i2c_get_adapter: i2c-%d\n", i2c_adap->nr);
    // bh1750_client = i2c_new_probed_device(i2c_adap, &my_i2c_dev_info, addr_list, NULL);
    bh1750_client = i2c_new_device(i2c_adap, &my_bh1750_info);
    i2c_put_adapter(i2c_adap);

    if (bh1750_client)
    {
		misc_register(&bh1750_dev);
		printk("%s init: addr=0x%x @ IIC%d, irq=%d\n",
			bh1750_client->name,bh1750_client->addr,bh1750_client->adapter->nr,bh1750_client->irq);
        return 0;
    }
    else
        return -ENODEV;
}
static void __exit bh1750_exit(void)
{
	misc_deregister(&bh1750_dev);
	i2c_unregister_device(bh1750_client);
	printk("== %s\n", __func__);
}

module_init(bh1750_init);
module_exit(bh1750_exit);

MODULE_AUTHOR("EasyWang");
MODULE_DESCRIPTION("BH1750_I2CTest");
MODULE_LICENSE("GPL v2");
```

编译文件：

```makefile
# Makefile
obj-m += bh1750.o

KDIR := ../../iTop4412_Kernel_3.0	# Kernel root folder
CC := arm-none-linux-gnueabi-gcc	# Target board compiler

PWD := $(shell pwd) 

all: ko test				# make

ko:	bh1750.c				# make ko
	$(MAKE) ARCH=arm -C $(KDIR) M=$(PWD) modules

test: bh1750_misc_read.c	# make test
	$(CC) $< -o bh1750_read
	
clean:  
	rm *.mod.c *.o *.ko  modules.* Module.* bh1750_read
```

bh1750_misc_read.c 见[读取测试](#读取测试)。

#### 编译到内核

添加编译选项：

```makefile
# iTop4412_Kernel_3.0/drivers/i2c/chips/Kconfig
#
# Miscellaneous I2C chip drivers configuration
#

menu "Miscellaneous I2C Chip support"

config SENSOR_ROHM_BH1721
	tristate "Rohm BH1721 Light Sensor"
	depends on EXPERIMENTAL
	help
	  If you say yes here you get support for Rohm BH1721 Light Sensor.

config SENSOR_ROHM_BH1750
	tristate "Rohm BH1750 Light Sensor"
	depends on EXPERIMENTAL
	help
	  If you say yes here you get support for Rohm BH1750 Light Sensor.

endmenu
```

修改编译脚本：

```makefile
# iTop4412_Kernel_3.0/drivers/i2c/chips/Makefile

#
# Makefile for miscellaneous I2C chip drivers.
#
obj-$(CONFIG_SENSOR_ROHM_BH1721)	+= rohm_bh1721.o
obj-$(CONFIG_SENSOR_ROHM_BH1750)	+= rohm_bh1750.o
```

驱动文件：

```c
// iTop4412_Kernel_3.0/drivers/i2c/chips/rohm_bh1750.c

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h> 
#include <linux/miscdevice.h>

#define DEVICE_NAME  "bh1750"
#define BH1750_I2C_ADDR 0x23

static struct i2c_client *bh1750_client;

static int bh1750_open_func(struct inode * ind, struct file *filp)
{
	uint8_t cmd[] = {0x00, 0x01, 0x07};
	struct i2c_msg msgs[] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[0],
		},
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[1],
		},
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd[2],
		},
	};

	int ret = i2c_transfer(bh1750_client->adapter, msgs, 2);
	if (ret < 0) {
		pr_err("read reg error!\n");
		return -1;
	}

	return 0;
}

static ssize_t bh1750_read_func(struct file *filp, char __user *buffer, size_t count, loff_t *ppos){
	uint8_t readbuf[2] ={0};
	uint32_t light_intensity_32 = 0;
	uint16_t light_intensity = 0;
	
	uint8_t cmd = 0x20;

	struct i2c_msg msg_send[1] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &cmd,
		}
	};

	struct i2c_msg msgs_read[1] = {
		{
			.addr	= bh1750_client->addr,
			.flags	= 1,
			.len	= 2,
			.buf	= readbuf,
		}
	};

	int ret = i2c_transfer(bh1750_client->adapter, msg_send, 1);
	if (ret < 0) {
		pr_err("write cmd error!\n");
		return 0;
	}

	mdelay(180);

	ret = i2c_transfer(bh1750_client->adapter, msgs_read, 1);
	if (ret < 0) {
		pr_err("read reg error!\n");
		return 0;
	}
	// printk("Read data: %x %x\n", readbuf[0], readbuf[1]);

	light_intensity_32 = (readbuf[0]<<8 | readbuf[1]) * 10 / 12;
	light_intensity = light_intensity_32;
    // printk("Lux value: %d\n", light_intensity);

	ret = copy_to_user(buffer,&light_intensity,2);
	
	return 2;
}

static struct file_operations bh1750_ops = {
	.owner 	= THIS_MODULE,
	.open 	= bh1750_open_func,
	.read 	= bh1750_read_func,
};


static struct miscdevice bh1750_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &bh1750_ops,
	.name	= DEVICE_NAME,
};

static int bh1750_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	bh1750_client = client;
	printk("%s init: addr=0x%x @ IIC%d, irq=%d\n",
		bh1750_client->name,bh1750_client->addr,bh1750_client->adapter->nr,bh1750_client->irq);

    if (bh1750_client)
    {
		misc_register(&bh1750_dev);
        return 0;
    }
    else
        return -ENODEV;
}

static int __devexit bh1750_remove(struct i2c_client *client)
{
	misc_deregister(&bh1750_dev);
	printk("== %s\n", __func__);
	return 0;
}

static const struct i2c_device_id bh1750_id[] =
{
	{ DEVICE_NAME, 0 },
	{ }
};

static struct i2c_driver bh1750_driver =
{
	.probe		= bh1750_probe,
	.remove		= __devexit_p(bh1750_remove),
	.id_table	= bh1750_id,
	.driver = {
		.name	= DEVICE_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init bh1750_init(void)
{
	return i2c_add_driver(&bh1750_driver);
}
static void __exit bh1750_exit(void)
{
	i2c_del_driver(&bh1750_driver);
}

module_init(bh1750_init);
module_exit(bh1750_exit);

MODULE_AUTHOR("EasyWang");
MODULE_DESCRIPTION("BH1750_I2CTest");
MODULE_LICENSE("GPL v2");
```

`make meunconfig` 打开 `CONFIG_SENSOR_ROHM_BH1750`。

```
Device Drivers  --->
    <*> I2C support  --->
        Miscellaneous I2C Chip support  --->
            <*> Rohm BH1750 Light Sensor
```

在板级文件：arch\arm\mach-exynos\mach-itop4412.c 中添加：

```
static struct i2c_board_info i2c_devs7[] __initdata = {
#ifdef CONFIG_SENSOR_ROHM_BH1750
	{
        I2C_BOARD_INFO("bh1750", 0x23),
        .type		= "bh1750",
        .platform_data	= NULL,
        .irq = IRQ_EINT(0),
     }
#endif
};

static void __init smdk4x12_machine_init(void)
{
	s3c_i2c7_set_platdata(NULL);
	i2c_register_board_info(7, i2c_devs7, ARRAY_SIZE(i2c_devs7));
}
```

加载信息如下：

```
[    4.040677] i2c /dev entries driver
[    4.045122] bh1750 init: addr=0x23 @ IIC7, irq=352
```

另记一个手动将设备添加到 /dev 的方法（此处因注册 misc 会自动添加）：

```sh
$ cat /sys/class/misc/bh1750/dev 
10:50
$ mknod /dev/bh1750_0 c 10 50
```

### 读取测试

在驱动加载好之后，就可以使用用户程序读取 BH1750 的值了。

```c
// bh1750_misc_read.c

#include <fcntl.h>    // open
#include <unistd.h>   // read, close
#include <stdio.h>    // printf

int main(int argc, char *argv[]) {
    int fd;
    unsigned short light_intensity = 0;

    if(argc == 2)
        fd = open(argv[1], O_RDONLY|O_NONBLOCK);
    else
        fd = open("/dev/bh1750", O_RDONLY|O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    printf("device open success.\r\n");

    int ret = read(fd, &light_intensity, 2);
    printf("Read size: %d.\r\n", ret);

    if (ret == 2) {
        printf("Read light intensity: %d lux.\r\n", light_intensity);
    }

    close(fd);
    printf("device close success.\r\n");
    return 0;
}
```

运行输出：

```
device open success.
Read size: 2.
Read light intensity: 205 lux.
device close success.
```

## i2ctool(i2cdetect)

### 编译部署

源码仓库：[i2c-tools/i2c-tools.git - I2C and SMBus user-space tools](https://git.kernel.org/pub/scm/utils/i2c-tools/i2c-tools.git/)

```shell
wget https://git.kernel.org/pub/scm/utils/i2c-tools/i2c-tools.git/snapshot/i2c-tools-4.4.tar.gz
tar xzf i2c-tools-4.4.tar.gz
cd i2c-tools-4.4/

make CC=arm-none-linux-gnueabi-gcc BUILD_DYNAMIC_LIB=1 BUILD_STATIC_LIB=0
make CC=arm-none-linux-gnueabi-gcc AR=arm-none-linux-gnueabi-ar STRIP=arm-none-linux-gnueabi-strip strip
mkdir ../output/i2c-tools
make PREFIX=../output/i2c-tools install
```

得到文件：

```
output/i2c-tools/
├── bin
│   ├── ddcmon
│   ├── decode-dimms
│   ├── decode-edid
│   └── decode-vaio
├── include
│   └── i2c
│       └── smbus.h
├── lib
│   ├── libi2c.a
│   ├── libi2c.so -> libi2c.so.0
│   ├── libi2c.so.0 -> libi2c.so.0.1.1
│   └── libi2c.so.0.1.1
├── sbin
│   ├── i2cdetect
│   ├── i2cdump
│   ├── i2cget
│   ├── i2cset
│   ├── i2c-stub-from-dump
│   └── i2ctransfer
└── share
    └── man
        ├── man1
        │   ├── decode-dimms.1
        │   └── decode-vaio.1
        ├── man3
        │   └── libi2c.3
        └── man8
            ├── i2cdetect.8
            ├── i2cdump.8
            ├── i2cget.8
            ├── i2cset.8
            ├── i2c-stub-from-dump.8
            └── i2ctransfer.8
```

将 lib/ 和 sbin/ 下的文件放置到开发板的对应目录即完成部署。

### 测试

调试 BH1750 示例（挂在 i2c-7，设备地址为 0x23）：

```shell
$ i2cdetect -y 7
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- 23 -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --
$ i2cset -y 7 0x23 0
$ i2cset -y 7 0x23 1
$ i2cset -y 7 0x23 0x20
$ i2ctransfer -y 7 r2@0x23
```

## 参考

* [Specification - I2C Bus](https://www.i2c-bus.org/specification/)
* [I2C-bus specification and user manual](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)
* 宋宝华.Linux设备驱动开发详解:基于最新的Linux 4.0内核[M].机械工业出版社,2015.
* 华清远见嵌入式学院,刘洪涛,秦山虎.ARM嵌入式体系结构与接口技术:Cortex-A9版.北京:人民邮电出版社,2017.
* 数据手册：BH1750FVI.pdf

