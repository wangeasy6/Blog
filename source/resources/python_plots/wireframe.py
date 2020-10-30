#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.figure()
ax = plt.axes(projection='3d')

x = np.arange(-5, 6, 1)
X, Y = np.meshgrid(x, x)
Z = -(X*X + Y*Y)

ax.plot_wireframe(X, Y, Z)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()