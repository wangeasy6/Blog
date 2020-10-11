import numpy as np
import matplotlib.pyplot as plt

supply_x = np.arange(0, 35, 5)
supply_y = np.arange(12, -2, -2)

demand_x = np.arange(0, 35, 5)
demand_y = np.arange(0, 7, 1)

plt.plot(supply_x, supply_y, 'r', linewidth=2)
plt.plot(demand_x, demand_y, 'b', linewidth=2)
plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.title('冰淇淋-供需平衡')
plt.xlabel("价格")
plt.ylabel("需求量")
plt.grid()
plt.show()