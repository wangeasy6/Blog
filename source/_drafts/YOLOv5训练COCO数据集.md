---
title: YOLOv5训练COCO数据集
toc: true
categories:
  - null
tags:
  - null
---

试验环境：

* Ubuntu 18.04.6 LTS
* GeForce RTX 1080Ti

<!--more-->

<br/>

## 配置YOLOv5环境

Python>=3.7.0

```
conda create -n yolov5_37 python==3.7.16
conda activate yolov5_37
```

### 配置 torch-GPU

win+R 弹出框中输入cmd，在弹出框里输入nvidia-smi，如下所示：

```
NVIDIA-SMI 470.103.01   Driver Version: 470.103.01   CUDA Version: 11.4
```

打开[PyTorch](https://pytorch.org/)官网，我的 torch 版本为 1.11.0，所以执行以下命令安装：

```sh
conda install pytorch==1.12.1 torchvision==0.13.1 torchaudio==0.12.1 cudatoolkit=11.3 -c pytorch
```

获取YOLOv5

```sh
git clone https://github.com/ultralytics/yolov5
cd yolov5
pip install -r requirements.txt
```

无法连接github，使用gitee替代：[同步更新官方最新版 Yolov5](https://gitee.com/monkeycc/yolov5)



## 训练模型

```sh
python train.py --data coco128.yaml --weights yolov5s.pt --img 640 --device=0
```

命令会自动下载 coco128 数据集。


## 使用模型

下载测试：

```
python detect.py --source ./data/images/ --weights yolov5s.pt --conf 0.4
```

使用自己生成的模型测试：

`python detect.py --source ./data/images/ --weights runs/train/exp2/weights/best.pt --conf 0.4`



