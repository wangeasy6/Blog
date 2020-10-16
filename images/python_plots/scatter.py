#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

def randrange(n, vmin, vmax):
    '''
    @return 范围在min~max的n个随机数
    '''
    return (vmax - vmin) * np.random.rand(n) + vmin


plt.figure()
ax = plt.axes(projection='3d')

n = 10

# For循环2次
# 第一次：产生10个x在0~5，y在0~10，x在0~10的三维坐标，以红色点状绘制在3D图上
# 第二次：产生10个x在0~5，y在0~10，x在-10~0的三维坐标，以蓝色三角状绘制在3D图上
for c, m, zlow, zhigh in [('r', 'o', 0, 10), ('b', '^', -10, 0)]:
    xs = randrange(n, 0, 5)
    ys = randrange(n, 0, 10)
    zs = randrange(n, zlow, zhigh)
    ax.scatter(xs, ys, zs, c=c, marker=m)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()