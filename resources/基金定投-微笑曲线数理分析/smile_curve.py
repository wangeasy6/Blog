# -*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 21, 1)

y = []
for i in x:
    y.append((i-10) * (i-10)+50)
plt.plot(x, y, color="r", linestyle="-", linewidth=2, marker="o", label="微笑曲线")
print(y)

# 深微
# y = []
# for i in x:
#     y.append((2*(i-10)) * (2*(i-10))+50)
# plt.plot(x, y, color="r", linestyle="-", linewidth=2, marker="o", label="微笑曲线")

# 假设每期投15块
investment = 15
stock_per_invest = []
for i in y:
    stock_per_invest.append(investment/i)
print(stock_per_invest)

z = []
for i in range(len(y)):
    # 当前价格 * 手上所有的持有股份 - 100*期数
    profit = (y[i] * np.sum(stock_per_invest[:i])) - (investment * i)
    z.append(profit)
plt.plot(x, z, color="b", linestyle="-", linewidth=2, marker="o", label="收益曲线")
print(z)

zero_line = [0]*len(x)
plt.plot(x, zero_line, color="#444444", linestyle="--", linewidth=1)

plt.show()