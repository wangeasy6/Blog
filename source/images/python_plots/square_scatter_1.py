#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.figure()
ax = plt.axes(projection='3d')

# 一个边长为4的正方形散点阵列
# 循环4*4*4次，对每一个点进行绘制
edge = range(4)
for x in edge:
    for y in edge:
        for z in edge:
            if z == 0:
                ax.scatter(x, y, z, c='r', marker='o')  # 根据不同层进行着色
            elif z == 1:
                ax.scatter(x, y, z, c='g', marker='o')
            elif z == 2:
                ax.scatter(x, y, z, c='b', marker='o')
            elif z == 3:
                ax.scatter(x, y, z, c='k', marker='o')

# 调整xyz轴
axis = range(-3, 7, 1)
ax.auto_scale_xyz(axis, axis, axis)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()