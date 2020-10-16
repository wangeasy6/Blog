#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.figure()
ax = plt.axes(projection='3d')

edge = range(4)
X,Y,Z = np.meshgrid(edge, edge, edge)

ax.scatter(X, Y, Z, c='k', marker='o')

# 调整xyz轴
axis = range(-3, 7, 1)
ax.auto_scale_xyz(axis, axis, axis)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()