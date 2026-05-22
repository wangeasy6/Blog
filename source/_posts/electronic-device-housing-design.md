---
title: 3D打印电子设备外壳设计
date: 2026-05-22 09:45:00
toc: true
categories:
  - tech
tags:
  - 3D
---

![Cover](/resources/3D/device-housing-design/cover.png)

适用于 FDM 3D 打印的电子设备外壳设计方案总结，包括 [卡扣式](/tech/electronic-device-housing-design/#卡扣式)、[滑块式](/tech/electronic-device-housing-design/#滑块式)、[铰链式](/tech/electronic-device-housing-design/#铰链式)、[螺丝固定](/tech/electronic-device-housing-design/#螺丝固定)、[磁吸固定](/tech/electronic-device-housing-design/#磁吸固定)。

<!--more-->

<br/>

## 卡扣式

以上下盖的凹点和凸点形成卡扣，形成无需工具和额外配件即可开合的外壳结构。凸起厚度既影响安装，也是决定紧密程度的关键。

对于所有依靠上下盖咬合的设计都需要注意，预留外壳间的缝隙空间，适配打印精度。

示例：

![1](/resources/3D/device-housing-design/1.png)

梯形、三角形，调整垂直方向的角度低于“支撑悬垂角度”，即可实现无支撑打印。

![2](/resources/3D/device-housing-design/2.png)

单点、长条，还是整个边的全尺寸卡口，可以视拆装难度和紧密度而决定。

![3](/resources/3D/device-housing-design/3.png)

<br/>

## 滑块式

如图所示，可以用于上盖、下盖、侧面，优势是简单易用。

![8](/resources/3D/device-housing-design/8.png)

但是如无额外的固定结构，容易自行滑出，建议增加定位（螺丝/磁吸固定位效果更佳）。

![9](/resources/3D/device-housing-design/9.png)

<br/>

## 铰链式

图示的这种箱子式设计十分简洁流畅，也无需额外配件。

一侧使用了免支撑打印的铰链，底部有用于固定主板的定位点，顶部的立柱也用于固定主板，另一侧用微小的三角作为卡扣。

![7](/resources/3D/device-housing-design/7.png)

<br/>

## 螺丝固定

一般电子主板都会预留用于固定的螺丝孔位，结合螺丝孔位，使用螺丝进行固定也是主流的设计方案。

**优点**

* 设计简单
* 固定牢固
* 拆装方便

**缺点**

* 需要额外购买螺丝
* 拆装需要依靠工具
* 需要适配主板上的孔位（在主板外加固定位也可以，但会显得比较臃肿）

示例：

![4](/resources/3D/device-housing-design/4.png)

以上使用的是自攻螺丝，固定柱的内径应减去自攻螺丝的牙径，以便攻紧。同时固定柱的墙壁层数应取大，否则壁厚太薄可能会直接攻破。固定柱直径也应扩大，因为此时的受力在 Z 轴方向，而 FDM 3D 打印的 Z 轴方向强度是由层间粘连决定的，所以应增加接触面积。

对于长久使用，自攻螺丝可能会引起滑丝，更换为普通螺丝可以更加稳定。

![5](/resources/3D/device-housing-design/5.png)

在底部安装螺母，中间的柱体作为支撑，使用长螺丝固定上下壳。此方案会更加稳固，可以反复拆装。

此设计为沉头螺丝和六角螺母都预留了安装位置，更加方便美观。

除了普通的六角螺母，这种热熔螺母可以获得更加稳固的固定效果和更加美观的视觉效果。缺点是安装相对麻烦，需要用烙铁加热后安装。

![6](/resources/3D/device-housing-design/6.jpg)

<br/>

## 磁吸固定

加入磁铁，依据磁力的大小，既可以作为主要固定部件，也可以在其它设计中作为辅助固定部件。

**优点**

* 设计简单
* 可实现快拆快装

**缺点**

* 需要额外购买磁铁
* 固定到孔位时需要注意磁铁极性对应
* 牢固程度依靠磁力大小而定，可能会出现过大或过小的情况
* 磁铁可能会干扰 NFC信号、低频RFID信号、地磁传感器（指南针）

如下设计分为 3 层，使用 16 个 6x2 毫米的磁铁进行固定。

![10](/resources/3D/device-housing-design/10.png)

这个设计中，把磁铁固定到孔位一般需要使用模型胶水。

使用隐藏式的安装方式可以获得更加美观的效果：在打印过程中添加暂停点，当打印机暂停后放入磁铁，继续运行后打印顶层。**此方法需要确定磁铁不会被打印机喷头吸住。**

![11](/resources/3D/device-housing-design/11.png)

滑块式 + 磁吸辅助固定 示意：

![12](/resources/3D/device-housing-design/12.png)

<br/>

## 参考

* [树莓派 5 卡扣式外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/159076-shu-mei-pai-5-qia-kou-shi-wai-ke?from=search#profileId-72424)
* [树莓派 4 机箱 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/361759-shu-mei-pai-4-ji-xiang?from=search#profileId-287785)
* [最简单的树莓派 4B 外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/2071385-zui-jian-dan-de-shu-mei-pai-4b-wai-ke?from=search#profileId-2314858)
* [树莓派4 B型 25mm风扇带散热片卡扣式外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/1058191-shu-mei-pai-4-bxing-25mmfeng-shan-dai-san-re-pian?from=search#profileId-1096596)
* [树莓派4B外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/2087309-shu-mei-pai-4bwai-ke?from=search#profileId-2334283)
* [树莓派3B+外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/140588-shu-mei-pai-3b-wai-ke?from=search#profileId-46641)
* [树莓派 Zero 2W 外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/961784-shu-mei-pai-zero-2w-wai-ke?from=search#profileId-977691)
* [又一款树莓派 Zero 外壳——带铰链 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/1026217-you-yi-kuan-shu-mei-pai-zero-wai-ke-dai-jiao-lian?from=search#profileId-1057391)
* [树莓派5带抽屉的外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/427897-shu-mei-pai-5dai-chou-ti-de-wai-ke?from=search#profileId-340999)
* [树莓派4滑块外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/911889-shu-mei-pai-4hua-kuai-wai-ke?from=search#profileId-915120)
* [Raspberry Pi 4 磁吸式外壳 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/1600035-raspberry-pi-4-ci-xi-shi-wai-ke?from=search#profileId-1749518)
* [磁吸收纳盒 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/1257539-ci-xi-shou-na-he?from=search#profileId-1344440)
* [中国地图-磁吸-冰箱贴-拼图 - 免费 3D 打印模型 - MakerWorld](https://makerworld.com.cn/zh/models/578589-zhong-guo-di-tu-ci-xi-bing-xiang-tie-pin-tu?from=search#profileId-729786)
