#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

names = ['a', 'b', 'c']
values = [1, 10, 100]

plt.figure(figsize=(9, 3))

plt.subplot(131)  # 指定位置，1行3列的第1个
plt.scatter(names, values)
plt.subplot(132)
plt.plot(names, values)
plt.subplot(133)
plt.bar(names, values)
plt.suptitle('subplot Sample')
plt.show()