import numpy as np
import matplotlib.pyplot as plt

demand_x = np.arange(0, 35, 5)
demand_y = np.arange(0, 7, 1)

plt.plot(demand_x, demand_y, 'b', linewidth=2)
plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.title('冰淇淋-供给曲线')
plt.xlabel("价格")
plt.ylabel("供给量")
plt.xticks(np.arange(0, 35, 5))
plt.yticks(np.arange(0, 13, 1))
plt.grid()
plt.show()