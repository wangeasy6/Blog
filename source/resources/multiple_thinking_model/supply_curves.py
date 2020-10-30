import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 35, 5)
y = np.arange(12, -2, -2)

print(x)
print(y)
plt.plot(x, y, 'r', linewidth=2)
plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.title('冰淇淋-需求曲线')
plt.xlabel("价格")
plt.ylabel("需求量")
plt.grid()
plt.show()