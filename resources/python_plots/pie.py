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