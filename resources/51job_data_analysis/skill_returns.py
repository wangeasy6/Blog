#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import csv
import ast
import jieba
import jieba.analyse


topK = 50
texts = ""
with open("51_Job_嵌入式软件_南京_2020-10-20_21-54-57 - 副本.csv", encoding='utf-8', errors='ignore') as f:
    reader = csv.reader(f)
    rows=[row for row in  reader]
    # print(len(rows))
    for i in range(1, len(rows)):
        if rows[i] and len(rows[i]) >= 5:
            texts += rows[i][5]


jieba.analyse.set_stop_words('stopword.txt')
tags = jieba.analyse.extract_tags(texts, topK=topK, withWeight=True, allowPOS='n')
# tags = jieba.analyse.textrank(texts, topK=topK, withWeight=True, allowPOS='n')
for tag in tags:
    print("tag: %s\t\t weight: %f" % (tag[0],tag[1]))
    # print(tag[0])

'''
tag: 单片机         weight: 0.828927
tag: 硬件      weight: 0.444306
tag: C语言         weight: 0.263081
tag: 算法      weight: 0.153864
tag: 电路      weight: 0.130381
tag: 原理图         weight: 0.124754
tag: 平台      weight: 0.119759
tag: 架构      weight: 0.116475
tag: 芯片      weight: 0.110682
tag: 嵌入式操作系统         weight: 0.089653
tag: 底层      weight: 0.083189
tag: 驱动程序        weight: 0.077940
tag: 电路设计        weight: 0.075547
tag: 方案设计        weight: 0.068695
tag: 总线      weight: 0.064144
tag: 数字电路        weight: 0.059586
tag: 处理器         weight: 0.057218
tag: 技术支持        weight: 0.055411
tag: 数据结构        weight: 0.052336
tag: 信号处理        weight: 0.050151
tag: 电力      weight: 0.045743
tag: 架构设计        weight: 0.045562
tag: 传感器         weight: 0.044760
tag: 终端      weight: 0.044297
tag: 电机      weight: 0.042813
tag: 软件测试        weight: 0.042217
tag: 网络      weight: 0.041223
tag: 网络协议        weight: 0.038848
tag: 智能      weight: 0.038732
tag: 固件      weight: 0.036743
tag: 单元测试        weight: 0.036743
tag: 性能      weight: 0.036439
tag: 主板      weight: 0.035394
tag: 测试用例        weight: 0.033804
tag: 汽车      weight: 0.033745
tag: 机器人         weight: 0.033362
tag: 自动控制        weight: 0.032581
tag: 框架      weight: 0.029384
tag: 电气工程        weight: 0.028897
tag: 汇编语言        weight: 0.028286
tag: 视频      weight: 0.027735
tag: 表达能力        weight: 0.026939
tag: 示波器         weight: 0.026306
tag: 仪器      weight: 0.024783
tag: 设计方案        weight: 0.024031
tag: 无线      weight: 0.023999
tag: 数字      weight: 0.023882
tag: 电气工程师       weight: 0.023516
tag: 医疗      weight: 0.023448
tag: 风格      weight: 0.023395
'''