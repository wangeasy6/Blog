---
title: 设备通信接口一览表
toc: true
categories:
  - tech
tags:
  - 嵌入式
date: 2026-08-22 19:00:00
---

常用的设备通信接口一览，包括：IIC、UART、RS232、RS422、RS485、SPI、CAN、USB、 Ethernet、PCIe。

<!--more-->

<br/>

## 一览表

| 接口                         | 物理接口                                                     | 通信速率（bps）                                              | 最大通信距离（以实际芯片/线缆为准）                          | 同步方式                           | 双工类型                       | 多机架构                                    | 电平                    | 相关标准                                                     | 常见设备                              |
| ---------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------------- | ------------------------------ | ------------------------------------------- | ----------------------- | ------------------------------------------------------------ | ------------------------------------- |
| **IIC**                      | 2P(SDA/SCL)                                                  | 100K<br/>400K<br/>1M<br/>3.4M<br/>5M                         | 5m<br/>1m<br/>0.5m<br/>10cm<br/>10cm<br/>                    | 同步                               | 半                             | 主从架构，支持多主多从，7位或10位从设备地址 | TTL电平                 | [I2C-bus specification and user manual](https://www.nxp.com/docs/en/user-guide/UM10204.pdf) | EEPROM、RTC、屏幕/相机 配置端口       |
| **CMOS/TTL compatible UART** | 3P(TX/RX/GND)                                                | 9600<br/>19200<br/>38400<br/>57600<br/>115200<br/>460800     | 10m<br/>5m<br/>3m<br/>2m<br/>1.5m<br/>0.3m                   | 异步                               | 全                             | 点对点                                      | TTL电平                 | 事实标准（以芯片数据手册为准）                               | 调试串口、GPS模块、蓝牙模块           |
| **RS232**                    | DB-25<br/>DB-9<br/>3P(TX/RX/GND)                             | 1200<br/>2400<br/>4800<br/>9600<br/>19200<br/>38400<br/>57600<br/>115200 | 200m<br/>100m<br/>50m<br/>25m<br/>15m<br/>7.5m<br/>5m<br/>3m | 异步                               | 全                             | 点对点                                      | 单端                    | TIA/EIA-232-(C/D/E/F)（标准只规定**电气特性、机械连接器、功能引脚和握手流程**，不规定上层通信协议） | 调试串口                              |
| **RS422**                    | 4P(T+/T-/R+/R-)                                              | 100k<br/>200k<br/>1M<br/>2M<br/>6M<br/>10M                   | 1200m<br/>488m<br/>100m<br/>40m<br/>17m<br/>12m              | 异步                               | 全                             | 点对多（1主10从）                           | 差分                    | TIA/EIA-422-(A/B)                                            | PLC、电机驱动器、CNC 数控系统         |
| **RS485**                    | 2P：A+、B-                                                   | 2400<br/>4800<br/>9600<br/>19200<br/>38400<br/>57600<br/>115200<br/>1M<br/>10M | 1800m<br/>1200m<br/>1200m<br/>800m<br/>300m<br/>150m<br/>100m<br/>100m<br/>15m | 异步                               | 半                             | 多点多主（32个节点）                        | 差分                    | TIA/EIA-485(-A)                                              | PLC、智能电/水/气表、充电桩控制器     |
| **SPI**                      | 4P(MOSI/MISO/SCLK/CS)                                        | 100k<br/>400k<br/>1M<br/>5M<br/>20M<br/><br/>50M             | 5m<br/>2m<br/>1m<br/>50cm<br/>30cm<br/>20cm<br/>10cm         | 同步                               | 全                             | 一主多从（靠片选CS引脚）                    | TTL电平                 | 事实标准（[SPI Block Guide V4](https://web.pa.msu.edu/people/edmunds/Disco_Kraken/SPI_Documents/motorola_freescale_nxp_spi_manual_2000.pdf)） | Flash、陀螺仪、WiFi/ETH/BLE 配置      |
| **CAN**                      | 3P(CAN_L/CAN_H/GND)                                          | 5k<br/>10k<br/>20k<br/>50k<br/>125k<br/>250k<br/>500k<br/>1M | 10km<br/>5km<br/>2.5km<br/>1km<br/>500m<br/>250m<br/>100m<br/>20m | 异步                               | 半                             | 多主对等                                    | 差分                    | [ISO 11898-(1/2/3/4)](https://www.iso.org/search.html?PROD_isoorg_en[query]=11898&PROD_isoorg_en[menu][facet]=standard) | 汽车、船舶、航天、工业控制            |
| **USB1.x**                   | 2P(D+/D-)                                                    | 1.5M<br/>12M                                                 | 5m                                                           | 混合                               | 半                             | 一主多从                                    | 差分                    | [USB-IF Document](https://www.usb.org/documents)             | 键盘、鼠标                            |
| **USB2.0**                   | 2P(D+/D-)                                                    | 12M<br/>480M                                                 | 5m                                                           | 混合                               | 半                             | 一主多从                                    | 差分                    | [USB-IF Document](https://www.usb.org/documents)             | 移动硬盘、摄像头、USB网卡             |
| **USB3.0**                   | 6P(D+/D-/TX+/TX-/RX+/RX-)                                    | 5G<br/>10G<br/>20G                                           | 3m<br/>2m<br/>1m                                             | 混合                               | 全                             | 一主多从                                    | 差分                    | [USB-IF Document](https://www.usb.org/documents)             | 高速 U 盘、高速工业相机、VR 头显      |
| **USB4**                     | Type-C                                                       | 20G<br/>40G<br/>120G                                         | 0.8m                                                         | 混合                               | 全                             | 一主多从                                    | 差分                    | [USB-IF Document](https://www.usb.org/documents)             | 外置 GPU 坞、8K 显示器直连、AI 加速棒 |
| **Ethernet**                 | Cat3+ 双绞线/同轴线<br/>Cat5+ 双绞线/多模光纤<br/>Cat5e/Cat6 双绞线/多模光纤/单模光纤<br/>Cat5e/Cat6 双绞线<br/>Cat6a/多模/单模<br/>Cat8/多模/单模<br/>Cat8/多模/单模<br/>多模 / 单模<br/>单模为主 | 10M<br/>100M<br/>1G<br/>2.5G/5G<br/>10G<br/>25G<br/>40G<br/>100G<br/>400G | 100m/500m<br/>100m/400m<br/>100m/550m/5km<br/>100m<br/>100m/300m/10km<br/>30m/100m/10km<br/>30m/150m/10km<br/>100m/10km<br/>500m/2km/10km 级 | 同步                               | 可协商半/全双工<br/>强制全双工 | 多主对等                                    | 差分（光纤/同轴不适用） | [IEEE SA - IEEE 802.3-2022](https://standards.ieee.org/ieee/802.3/10422/) | 以太网                                |
| **PCIe**                     | CEM 金手指边缘连接器<br/>M.2<br/>U.2<br/>OCuLink<br/>SlimSAS<br/>MCIO<br/>CopperLink | 2.5GT<br/>5GT<br/>8GT<br/>16GT<br/>32GT<br/>64GT<br/>        | 21in（板级，下同）<br/>21in<br/>14in<br/>8in<br/>6in<br/>3.4in | 介于“自同步串行”与“协议层同步”之间 | 全                             | 点对点                                      | 差分                    | [PCI-SIG](https://pcisig.com/)                               | 显卡、网卡、SSD                       |

<br/>

## 名词解释

* 全双工：允许数据在两个设备间同时双向传输，就像双向车道可以同时通车。
* 半双工：允许数据双向传输，但不能同时进行，某个时刻只能有一方发送，另一方接收，就像单车道桥，需要分时使用。
* TTL电平：如0V表示逻辑0，3.3V/5V表示逻辑1。
* 负逻辑电平：-3V至-15V表示逻辑1，+3V至+15V表示逻辑0。
* 单端传输：使用一根信号线和一根公共地线来传输信号。信号电压是相对于公共地线的。外界干扰会同时作用于信号线和地线，在接收端容易因两地电势差引入共地噪声和共模干扰，导致传输距离短、抗噪能力弱。
* 差分：使用一对双绞线（A和B）来传输信号，发送端驱动器产生大小相等、方向相反的差分电压。接收端只关心这两根线之间的电压差。外界的共模干扰在两条线上产生的噪声电压接近相等，在计算电压差时会被大幅抵消，因此抗干扰能力极强，适合远距离传输。
* 现在业界通常所说的“RS-232”接口，实际上指的是后来修订的RS-232C标准。
* in：英寸，1 in = 2.54 cm = 25.4 mm。

<br/>

## 参考

* [2个422 并联一个总线_RS-232、RS-422、RS-485原理区别和联系！一看就懂-CSDN博客](https://blog.csdn.net/weixin_30916627/article/details/112142247)
* [RS-232 - Wikipedia](https://en.wikipedia.org/wiki/RS-232)
