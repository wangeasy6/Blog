#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.figure()
ax = plt.axes(projection='3d')

'''
# 楼梯折线图
[1,5] [5,5]
[1,1] [5,1]
一共5楼
'''
x = [1, 1, 5, 5] * 5
y = [1, 5, 5, 1] * 5
z = np.arange(20)

ax.plot(x, y, z)

plt.show()