---
title: 常见的 Camera 接口
date: 2026-08-23 16:03:15
toc: true
categories:
  - tech
tags:
  - 音视频
---

常用的 Camera（图像传感器/摄像头模组）输出接口，按**传输信号类型**和**应用场景**梳理一下。

<!--more-->

<br/>

## 一、并行接口（传统/低速）

### 1. DVP

- **全称**：Digital Video Port / Parallel Camera Interface
- **信号组成**：PCLK（像素时钟）、VSYNC（帧同步）、HSYNC（行同步）+ 8/10/12/16 位数据线
- **特点**：
  - 引脚多、走线复杂，对 PCB 布线要求高
  - 速率有限，通常最高约 150–200 MHz PCLK，分辨率/帧率受限
- **应用**：早期手机、嵌入式 Linux 板卡（如树莓派旧款）、低端 IPC 摄像头
- **现状**：逐渐被 MIPI CSI 取代，但在低成本 MCU 方案中仍常见

------

## 二、高速串行差分接口（当前主流）

### 2. MIPI CSI-2 / CSI-3

- **全称**：Mobile Industry Processor Interface Camera Serial Interface
- **物理层**：差分对（C-PHY 或 D-PHY），常用 D-PHY
- **通道数**：1–4 lane（常见 2-lane 或 4-lane）
- **速率**：D-PHY v1.2 每 lane 可达 2.5 Gbps；v2.1 可达 4.5 Gbps
- **特点**：
  - 引脚少、抗干扰强、带宽高
  - 支持多路虚拟通道（Virtual Channel），可多摄复用
  - 是手机、平板、嵌入式的主流标准
- **应用**：智能手机多摄、树莓派、NVIDIA Jetson、各类 SoC 平台
- **注意**：MIPI CSI-2 是**协议+物理层**的组合，很多 SoC 的"CSI 接口"实际就是指它

### 3. LVDS

- **全称**：Low-Voltage Differential Signaling
- **常见形式**：
  - **Sub-LVDS**：常用于中低端 CMOS 传感器（如 OV 系列），引脚定义非标准化
  - **Channel Link**（如 TI FPDLink）：工业相机、车载常用
- **特点**：
  - 差分传输，抗干扰好
  - 带宽中等，协议相对简单
- **应用**：工业相机、车载摄像头、安防监控、医疗影像

### 4. SLVS / SLVS-EC

- **全称**：Sony Low Voltage Signaling
- **特点**：
  - 比 Sub-LVDS 更高带宽、更低功耗
  - SLVS-EC 是增强版，支持嵌入式时钟、更高速率
- **应用**：Sony 高分辨率工业传感器（如 IMX 系列），机器视觉领域

### 5. HiSPi

- **全称**：High-Speed Serial Pixel Interface
- **开发商**：ON Semiconductor（安森美）
- **特点**：类似 MIPI 但协议不同，有 Packetized 和 Streaming 两种模式
- **应用**：安森美高分辨率传感器（如 AR 系列），需桥接芯片转换到 MIPI

### 6. GMSL / FPD-Link

- **全称**：**GMSL**（Gigabit Multimedia Serial Link，Maxim/ADI）
- **FPD-Link III/IV**（Flat Panel Display Link，TI）
- **特点**：
  - 单根同轴/双绞线可传 **15–30 米**
  - 同时传输视频 + 控制（I²C）+ 供电（PoC）
  - 支持高达 4K@30fps 或 8MP@30fps
- **应用**：车载环视、ADAS、工业长距离相机

------

## 三、USB 接口（即插即用）

### 7. USB 2.0 / USB 3.0 / USB4

- **常见标准**：
  - **USB Video Class (UVC)**：免驱，插上即用
  - USB 2.0：最高约 480 Mbps，适合 1080p@30fps
  - USB 3.0（SuperSpeed）：5 Gbps，支持 4K@30fps 及以上
- **特点**：
  - 软件生态成熟（V4L2、DirectShow、WebRTC 等）
  - 无需专用硬件接口，PC/开发板通用
- **应用**：网络直播摄像头、工业 USB 相机、会议系统、教育实验

------

## 四、网络接口（IP Camera）

### 8. Ethernet（RJ45）

- **协议**：RTSP / ONVIF / HTTP / RTP
- **形态**：
  - 百兆/千兆以太网
  - PoE（Power over Ethernet）供电
- **特点**：
  - 传输距离远（100 米+，可中继）
  - 可多路集中管理
- **应用**：安防监控（IPC）、智慧城市、远程监控

------

## 五、其他专用/小众接口

| 接口                        | 简介                                       | 典型应用                             |
| --------------------------- | ------------------------------------------ | ------------------------------------ |
| **SPI**                     | 串行外设接口，带宽极低                     | 极低分辨率模块（如 OV7670 小屏应用） |
| **I²C/I3C**                 | 仅用于**控制**（寄存器配置），不传图像数据 | 所有带 ISP 的传感器                  |
| **SDI**（HD-SDI / 3G-SDI）  | 广播级串行数字接口，同轴传输，无压缩       | 广电、高端监控                       |
| **Camera Link**             | 基于 Channel Link 的工业标准，极高带宽     | 高端机器视觉、线阵相机               |
| **CoaXPress**               | 同轴超高速，可达 12.5 Gbps/lane            | 高速工业检测                         |
| **HDMI 输出**（摄像头直出） | 部分 HDMI 摄像机内置编码                   | 直播、医疗内窥镜显示                 |

------

## 六、选型速查表

| 场景              | 首选接口                     | 备选                |
| ----------------- | ---------------------------- | ------------------- |
| 手机 / 嵌入式 SoC | **MIPI CSI-2**               | DVP（低端）         |
| 车载              | **GMSL / FPD-Link**          | LVDS                |
| 工业相机（板载）  | **MIPI CSI-2 / LVDS / SLVS** | Camera Link         |
| PC 即插即用       | **USB 3.0 UVC**              | USB 2.0（低分辨率） |
| 网络远程监控      | **Ethernet (PoE)**           | —                   |
| 长距离（>5m）     | **GMSL / FPD-Link / SDI**    | 光纤扩展            |
| MCU 直连          | **DVP / SPI**                | 需 FIFO 缓冲        |

------

## 七、一个容易混淆的点

> **I²C 不是图像输出接口！** 它只负责配置传感器寄存器（地址、分辨率、帧率等），图像数据走的是上述接口之一。几乎所有摄像头都是"控制走 I²C + 数据走 MIPI/DVP/USB"。

