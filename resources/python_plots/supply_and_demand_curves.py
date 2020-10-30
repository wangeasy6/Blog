#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

# 需求数据
supply_x = np.arange(0, 35, 5)
supply_y = np.arange(12, -2, -2)

# 供给数据
demand_x = np.arange(0, 35, 5)
demand_y = np.arange(0, 7, 1)

plt.plot(supply_x, supply_y, color="r", linestyle="-", linewidth=2, marker="o", label="需求曲线")
plt.plot(demand_x, demand_y, color="b", linestyle="-", linewidth=2, marker="o", label="供给曲线")

# 下面3行效果和上面等价
# supply, demand = plt.plot(supply_x, supply_y, demand_x, demand_y)
# plt.setp(supply, color='r', linestyle="-", linewidth=2, marker="o")
# plt.setp(demand, color='b', linestyle="-", linewidth=2, marker="o")

# 如果不指定linewidth，可以只用1行
# plt.plot(supply_x, supply_y, 'ro-', demand_x, demand_y, 'bo-')

plt.legend(loc="upper right")  # 显示图例说明并指定位置
# label不写在plot里，也可以写在这儿
# plt.legend(labels=["需求曲线", "供给曲线"], loc="upper right")

plt.axis([-1, 31, -1, 13])  # axis 函数接受一个[xmin，xmax，ymin，ymax]列表，用于指定x、y轴的范围，即整个显示窗口的大小

plt.rcParams["font.sans-serif"] = ["SimHei"]  # 设置字体为SimHei才可以显示中文
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号
plt.title("冰淇淋-供需平衡")  # 设置整个图的Title为“冰淇淋-供需平衡”
plt.xlabel("价格")  # 设置x轴的标签为“价格”
plt.ylabel("需求量")  # 设置y轴的标签为“需求量”
plt.grid()  # 显示网格
plt.show()
