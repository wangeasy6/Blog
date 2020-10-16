---
title: Python基础图表绘制
date: 2020-10-16 15:16:29
toc: true
categories:
  - Technology
tags:
  - Python
---

![cover.png](/images/python_plots/surface.png)

Matplotlib是一个综合库，用于在Python中创建静态、动画和交互式可视化。在做数据可视化，尤其是三维图像时，是一个不错的选择。

当前环境：*Win10*、*Python 3.8.5*、*Matplotlib 3.3.2*

<!--more-->

<br/>

### 二维函数绘制

#### 折线图

以下程序是一个最简单（$y = x$）的二维函数绘制例程：

```Python
import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 5, 0.1)
y = x

plt.plot(x, y)
plt.show()
```

![sample.png](/images/python_plots/sample.png)

上述示例就是一个最素的函数绘制，没有title，没有坐标系说明，没有指定线的颜色，没有函数说明等等。

下面我们以[冰淇淋-供需平衡](/Thoughts/多元思维模型/#供给与需求)的数据为例，绘制一个全面的：

```Python
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
```

![supply_and_demand_plot.png](/images/python_plots/supply_and_demand_plot.png)

##### arange

```Python
numpy.arange([start,] stop[, step], dtype=None)
```

根据 start 与 stop 指定的范围以及 step 设定的步长，生成一个 ndarray。

| 参数  | 描述                                                         |
| ----- | ------------------------------------------------------------ |
| start | 起始值（序列包含起始值），不指定时默认为0                    |
| stop  | 终止值（不包含）                                             |
| step  | 步长，不指定时默认为1                                        |
| dtype | 指定返回`ndarray`的数据类型，如果没有指定，则会使用输入数据的类型。 |

##### color

* [colors](https://matplotlib.org/api/colors_api.html?highlight=colors)

| 简写 | 颜色   |
| ---- | ------ |
| 'b'  | 蓝色   |
| 'g'  | 绿色   |
| 'r'  | 红色   |
| 'c'  | 青色   |
| 'm'  | 洋红色 |
| 'v'  | 黄色   |
| 'k'  | 黑色   |
| 'w'  | 白色   |

也可以使用`#1f77b4`这样的RGB值。

##### linestyle

* [lineStyles](https://matplotlib.org/api/_as_gen/matplotlib.lines.Line2D.html?highlight=linestyles#matplotlib.lines.Line2D.lineStyles)

| 字符 | Style  |
| ---- | ------ |
| '-'  | 实线   |
| '--' | 虚线   |
| '-.' | 点划线 |
| ':'  | 点线   |

##### marker

* [All markers](https://matplotlib.org/api/markers_api.html?highlight=markers)

<br/>

#### 饼图

```Python
import matplotlib.pyplot as plt

ax = plt.axes()

# 饼图，切片按逆时针方向排序和绘制
labels = 'Frogs', 'Pigs', 'Dogs', 'Ducks'
sizes = [15, 30, 45, 10]
explode = (0, 0.1, 0, 0)  # 只“爆炸”第二片

ax.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%',
        shadow=True, startangle=90)
ax.axis('equal')  # 等宽高比，确保饼图绘制为圆形

plt.show()
```

![pie.png](/images/python_plots/pie.png)

#### subplot(多坐标系)

在同一个界面显示多个坐标系：

```Python
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
plt.suptitle('Categorical Plotting')
plt.show()
```

效果如下：

![subplot.png](/images/python_plots/subplot.png)

<br/>

### 三维绘图

绘制3D图像主要有两种方式，一种是利用关键字`projection='3d'`来实现，另一种则是通过从`mpl_toolkits.mplot3d`导入对象`Axes3D`来实现，目的都是生成具有三维格式的对象`Axes3D`。

```Python
#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# 方法一，利用关键字

import matplotlib.pyplot as plt

plt.figure()
plt.axes(projection='3d')

plt.show()
```

```Python
#!/usr/bin/env python3
# -*- coding:utf-8 -*-
#方法二，利用三维轴方法

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = Axes3D(fig)

plt.show()
```

#### 散点图

```Python
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
```

![square_scatter_1.png](/images/python_plots/square_scatter_1.png)

另外，也可以使用meshgrid来快速生成：

```Python
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
```

![square_scatter_2.png](/images/python_plots/square_scatter_2.png)

##### meshgrid

[meshgrid](https://numpy.org/doc/stable/reference/generated/numpy.meshgrid.html?highlight=meshgrid#numpy.meshgrid)是一个快速生成坐标矩阵的函数。

以上图为例，生成一个4x4x4的方形矩阵，正方形只需要指定一条边的坐标，长方形也只需要指定3条边的坐标，就可以使用For循环来绘图了，这是一种思路。

还有另外一种思路，这64个点都有x、y、z轴的坐标，所有x轴的坐标存在X里，所有y轴的坐标存在Y里，所以z轴的坐标存在Z里。

scatter函数支持一个一个点的绘制，也支持传入坐标函数的绘制。而meshgrid就是自动根据每边坐标生成坐标矩阵的函数。

#### 折线图

```Python
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
```

![line_chart.png](/images/python_plots/line_chart.png)

#### 线框图

```Python
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
```

![wireframe.png](/images/python_plots/wireframe.png)

#### 表面图

```Python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

fig = plt.figure()
ax = plt.axes(projection='3d')

X = np.arange(-5, 5, 0.25)
Y = np.arange(-5, 5, 0.25)
X, Y = np.meshgrid(X, Y)
R = np.sqrt(X ** 2 + Y ** 2)
Z = np.sin(R)

surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

# 显示颜色板，高度-颜色映射
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()
```

![surface.png](/images/python_plots/surface.png)

##### Colormap

* [Colormaps](https://matplotlib.org/api/pyplot_summary.html?highlight=colormaps#matplotlib.pyplot.colormaps)

如上所示，表面图的颜色可以指定cmap的样式来更换颜色映射。

<br/>

### 参考

* [NumPy Documentation](https://numpy.org/doc/stable/contents.html)
* [NumPy 教程](https://www.runoob.com/numpy/numpy-tutorial.html)
* [Matplotlib documentation](https://matplotlib.org/contents.html)
* [Matplotlib Examples](https://matplotlib.org/gallery/index.html)
* [pyplot_summary](https://matplotlib.org/api/pyplot_summary.html)