---
title: 'DL - Minst'
toc: true
categories:
  - Technology
tag:
  - AI
date: 2020-06-15 20:57:08
---

![mnist.jpg](/resources/mnist/mnist.jpg)

Mnist素有深度学习的Hello World之称，其任务是识别手写的数字0-9，属于分类问题。训练集提供了60000个训练数据和10000个测试数据，属于监督学习的一种。

根据《Python 深度学习》，搭建了Mnist程序环境：

* 系统：Win10 64位
* GPU：GTX 1050Ti
* Python：3.5.3
* Keras：2.3.1
* tensorflow-gpu：2.2.0
* CUDA：10.0
* cuDNN：7.6.5,for CUDA 10.0

<!--more-->
<br/>

## 环境搭建

1.首先需要安装Python环境

2.其次安装keras，命令：`pip install keras`

3.安装TensorFlow，命令：`pip install tensorflow-gpu`

   （如果没有GPU，则执行`pip install tensorflow`，此版本直接使用CPU进行运算，使用CPU版本则不用安装CUDA和cuDNN）

4.安装对应TensorFlow版本的CUDA

   1. 查询显卡支持的CUDA版本，在终端输入命令`nvidia-smi`，在输出中有“CUDA Version：”
   2. [TensorFlow与CUDA版本对应关系](https://tensorflow.google.cn/install/source_windows#gpu)
   3. [CUDA下载地址](https://developer.nvidia.com/cuda-toolkit-archive)
   4. 在终端输入`nvcc -V`查看是否成功安装

5.安装对应CUDA版本的cuDNN

   1. [cuDNN下载地址](https://developer.nvidia.com/rdp/cudnn-download)（需要注册一下）
   2. 安装：把解压的三个目录（bin、include、lib）下的文件对应放到CUDA的安装目录下

6.运行程序

**程序：**

```python
# -*- coding: utf-8 -*-
'''
Trains a simple deep NN on the MNIST dataset.
Gets to 98.40% test accuracy after 20 epochs
'''

from __future__ import print_function

import numpy as np
import keras
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import RMSprop
import matplotlib.pyplot as plt

batch_size = 128
num_classes = 10
epochs = 20

# Keras 官方推荐写法
# (x_train, y_train), (x_test, y_test) = mnist.load_data()

path='mnist.npz'
f = np.load(path)
x_train, y_train = f['x_train'], f['y_train']
x_test, y_test = f['x_test'], f['y_test']
f.close()

x_train = x_train.reshape(60000, 784).astype('float32')
x_test = x_test.reshape(10000, 784).astype('float32')
x_train /= 255
x_test /= 255
print(x_train.shape[0], 'train samples')
print(x_test.shape[0], 'test samples')

y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)

model = Sequential()
model.add(Dense(512, activation='relu', input_shape=(784,)))
model.add(Dense(512, activation='relu'))
model.add(Dense(num_classes, activation='softmax'))

model.summary()

model.compile(loss='categorical_crossentropy',
              optimizer=RMSprop(),
              metrics=['accuracy'])

history = model.fit(x_train, y_train,
                    batch_size=batch_size,
                    epochs=epochs,
                    verbose=1,
                    validation_data=(x_test, y_test))

history_dict = history.history
loss_values = history_dict['loss']
val_loss_values = history_dict['val_loss']
epochs = range(1, len(loss_values) + 1)
plt.plot(epochs, loss_values, 'bo', label='Training loss')
plt.plot(epochs, val_loss_values, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.legend()
plt.show()

score = model.evaluate(x_test, y_test, verbose=0)
print('Test loss:', score[0])
print('Test accuracy:', score[1])

model.save("mnist.hdf5")
```

程序会显示这20次训练的历史精度和准确性的趋势图，如下：

![history.png](/resources/mnist/history_.png)

输出测试模型下模型的损失值和度量值。

最后保存此次训练结果到 “mnist.hdf5”，后面可以使用`keras.models.load_model`加载此训练模型。

**异常**

1. ”Could not load dynamic library 'cufft64_10.dll'; dlerror: cufft64_10.dll not found“
   用GPU进行运算时，会加载CUDA和cuDNN库，但是库名称会有一点儿问题，出现如上提示，之后打印：”Skipping registering GPU devices...“，跳过GPU加载，而查看任务管理器会发现只使用了CPU。
   解决方法：此时无法加载的cufft64_10.dll实际命名是cufft64_100.dll，把到程序安装目录里找到cufft64_100.dll，改成cufft64_10.dll就行了。
2. Downloading mnist.npz速度慢的问题
   Keras默认加载mnist数据的时候会从amazonaws上下载，国内几乎无法下载。
   解决方法：提前下载[mnist.npz](/resources/mnist/mnist.npz)，使用numpy库来加载。

<br/>

## 应用

如何使用此模型来识别自己的手写数字呢？

首先，将手写的数字拍下来，使用下面这个程序来将其转换成28\*28的灰度图片：

```python
# -*- coding: utf-8 -*-
import cv2

global img
global point1, point2
def on_mouse(event, x, y, flags, param):
    global img, point1, point2
    img2 = img.copy()
    if event == cv2.EVENT_LBUTTONDOWN:         #左键点击
        point1 = (x,y)
        cv2.circle(img2, point1, 10, (0,255,0), 5)
        cv2.imshow('image', img2)
    elif event == cv2.EVENT_MOUSEMOVE and (flags & cv2.EVENT_FLAG_LBUTTON):   #按住左键拖曳
        cv2.rectangle(img2, point1, (x,y), (255,0,0), 5) # 图像，矩形顶点，相对顶点，颜色，粗细
        cv2.imshow('image', img2)
    elif event == cv2.EVENT_LBUTTONUP:         #左键释放
        point2 = (x,y)
        cv2.rectangle(img2, point1, point2, (0,0,255), 5)
        cv2.imshow('image', img2)
        min_x = min(point1[0], point2[0])
        min_y = min(point1[1], point2[1])
        width = abs(point1[0] - point2[0])
        height = abs(point1[1] -point2[1])
        cut_img = img[min_y:min_y+height, min_x:min_x+width]
        resize_img = cv2.resize(cut_img, (28,28)) # 调整图像尺寸为28*28
        ret, thresh_img = cv2.threshold(resize_img,127,255,cv2.THRESH_BINARY) # 二值化
        cv2.imshow('result', thresh_img)
        cv2.imwrite('new8.jpg', thresh_img)  # 预处理后图像保存位置

if __name__ == '__main__':
    global img
    img = cv2.imread('8.jpg')  # 手写数字图像所在位置
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) # 转换图像为单通道(灰度图)
    cv2.namedWindow('image')
    cv2.setMouseCallback('image', on_mouse) # 调用回调函数
    cv2.imshow('image', img)
    cv2.waitKey(0)
```

使用以下代码，加载生成的这个灰度图片的效果，可以同时加载训练库中的图片，对比看看效果：

```python
# -*- coding: utf-8 -*-
import keras
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

num_classes = 10

if 0:
    path='mnist.npz'
    f = np.load(path)
    x_train, y_train = f['x_train'], f['y_train']
    x_train = x_train.reshape(60000, 28, 28).astype('float32')
    y_train = keras.utils.to_categorical(y_train, num_classes)
    num = 6
    print(x_train[num])
    print(type(x_train[num]))
    print(x_train[num].dtype)
    print(x_train[num].shape)
    print(y_train[num])
    plt.imshow(x_train[num], cmap=plt.cm.binary)
    plt.show()


im = Image.open('new8.jpg')
data = list(im.getdata())
img_data = [(255-x)*1.0 for x in data]
img_data = np.array( img_data ).reshape(28, 28).astype('float32')
plt.imshow(img_data, cmap=plt.cm.binary)
plt.show()

print(img_data)
print(type(img_data))
print(img_data.dtype)
print(img_data.shape)
```

通过对比训练库图片发现，最好使用白板笔之类的粗笔，不然转化的图片几乎看不出来；另外，训练库白色的地方值都是0，而自己生成的照片中有很多1、2之类的，最好过滤5以下的灰度值，能更接近训练库图片。

**开始识别：**

加载训练好的模型，来识别我们所写的数字：

```python
import keras
import numpy as np
from PIL import Image

im = Image.open('new8.jpg')
data = list(im.getdata())
img_data = [(255-x)*1.0 for x in data]
for i in range(len(img_data)):
    if img_data[i] < 10:
        img_data[i] = 0

img_data = np.array( img_data ).astype('float32')

model = keras.models.load_model("mnist.hdf5")
result = model.predict(img_data.reshape(1,784))
if result.any():
    print(result)
else:
    print("Get None")
```

如果一切正常的话，就会得到以下结果：

`[0,0,0,0,0,0,0,0,1,0]`

程序的输出为One-Hot编码，第9个数字为1，代表程序识别此数字为8。

<br/>

## 代码说明

### to_categorical

**原型**：`to_categorical(y, num_classes=None, dtype='float32')`

**功能**：将整型的类别标签转为**One-Hot编码**。y为整型数组，num_classes为标签类别总数，大于max(y)（标签从0开始的）。

**返回**：如果num_classes=None，返回len(y) \* [max(y)+1]，否则为len(y)\* num_classes。

**One-Hot编码**：又称为一位有效编码，主要是采用N位状态寄存器来对N个状态进行编码，每个状态都由他独立的寄存器位，并且在任意时候只有一位有效。One-Hot编码是分类变量作为二进制向量的表示。这首先要求将分类值映射到整数值。然后，每个整数值被表示为二进制向量，除了整数的索引之外，它都是零值，它被标记为1。例如，此处y_train[0]本来是uint8的5，转换之后的y_train[0]为[0. 0. 0. 0. 0. 1. 0. 0. 0. 0.]。

<br/>

### Sequential

Sequential 是 Keras模型中的一种，叫做序贯模型，适用于简单的层堆栈，其中每一层正好具有一个输入张量和一个输出张量。

整个模型提供的是将单一输入映射为单一输出。对于Mnist这个例子，我们其实的模型做的其实是使用多层结构的映射关系，提供了一个从高维到低维的映射。

可以通过`.add()`方法，将layer加入到模型中，也可以向构造函数传递一个layer list 来构造模型，如本例可以改写成：

```python
model = Sequential(
    [Dense(512, activation='relu', input_shape=(784,)),
    Dense(512, activation='relu'),
    Dense(num_classes, activation='softmax')
    ])
```

模型需要知道输入数据的shape，因此，Sequential的第一层需要接受一个关于输入数据shape的参数，后面的各个层则可以自动的推导出中间数据的shape，因此不需要为每个层都指定这个参数。

<br/>

### Dense层

**原型**：`keras.layers.Dense(units, activation=**None**, use_bias=**True**, kernel_initializer='glorot_uniform', bias_initializer='zeros', kernel_regularizer=**None**, bias_regularizer=**None**, activity_regularizer=**None**, kernel_constraint=**None**, bias_constraint=**None**)`

`Dense` 实现以下操作： `output = activation(dot(input, kernel) + bias)` 其中 `activation` 是按逐个元素计算的激活函数，`kernel` 是由网络层创建的权值矩阵，以及 `bias` 是其创建的偏置向量 (只在 `use_bias` 为 `True` 时才有用)。

**参数**

- **units**: 正整数，输出空间维度。
- **activation**: 激活函数。 若不指定，则不使用激活函数 (即，「线性」激活: `a(x) = x`)。
- **use_bias**: 布尔值，该层是否使用偏置向量。
- **kernel_initializer**: `kernel` 权值矩阵的初始化器 (详见 [initializers](https://keras.io/zh/initializers/))。
- **bias_initializer**: 偏置向量的初始化器 (see [initializers](https://keras.io/zh/initializers/)).
- **kernel_regularizer**: 运用到 `kernel` 权值矩阵的正则化函数 (详见 [regularizer](https://keras.io/zh/regularizers/))。
- **bias_regularizer**: 运用到偏置向的的正则化函数 (详见 [regularizer](https://keras.io/zh/regularizers/))。
- **activity_regularizer**: 运用到层的输出的正则化函数 (它的 "activation")。 (详见 [regularizer](https://keras.io/zh/regularizers/))。
- **kernel_constraint**: 运用到 `kernel` 权值矩阵的约束函数 (详见 [constraints](https://keras.io/zh/constraints/))。
- **bias_constraint**: 运用到偏置向量的约束函数 (详见 [constraints](https://keras.io/zh/constraints/))。

被称作全连接层。之所以叫全连接是因为他的每一层和上一层所有单元相连接，如下图所示：

![dense.jpg](/resources/mnist/dense.jpg)

<br/>

#### 非线性函数(activation)

也叫激活函数。如果没有激活函数，Dense层将只包含两个线性运算——点积和加法：
`output = dot(input, k) + b`
这样 Dense 层就只能学习输入数据的线性变换：该层的假设空间是从输入数据到 16 位空间所有可能的线性变换集合。这种假设空间非常有限，无法利用多个表示层的优势，因为多个线性层堆叠实现的仍是线性运算，添加层数并不会扩展假设空间。
为了得到更丰富的假设空间，从而充分利用多层表示的优势，需要添加非线性或激活函数。

##### relu

**原型**：`keras.activations.relu(x, alpha=0.0, max_value=**None**, threshold=0.0)`

整流线性单元（Rectified Linear Unit, ReLU），又称修正线性单元。

使用默认值时，它返回逐元素的 `max(x, 0)`。

否则，它遵循：

- 如果 `x >= max_value`：`f(x) = max_value`，
- 如果 `threshold <= x < max_value`：`f(x) = x`，
- 否则：`f(x) = alpha * (x - threshold)`。

**参数**

- **x**: 张量。
- **alpha**：负数部分的斜率。默认为 0。
- **max_value**：输出的最大值。
- **threshold**: 浮点数。Thresholded activation 的阈值值。

**返回**

一个张量。

函数图像：

![relu.jpg](/resources/mnist/relu.jpg)

##### softmax

**原型**：`keras.activations.softmax(x, axis=-1)`

Softmax 激活函数：

$$
S_i =\frac{e^i}{\sum e^j}
$$

**参数**

- **x**：张量。
- **axis**：整数，代表softmax所作用的维度。

**返回**

映射区间[0, 1]。

**异常**

- **ValueError**：如果 `dim(x) == 1`。

##### sigmoid

也称logistic函数，映射区间（0, 1）。

$$
f(x) = \frac1{1 + e^{-x}}
$$

函数图像：

![sigmod.png](/resources/mnist/sigmoid.gif)

##### hard_sigmoid

$$
f(x) =
\begin{cases}
1, & 1 \leq x \cr
\frac{x+1}2, & -1 < x < 1 \cr
0, & x \leq -1
\end{cases}
$$

##### elu

**原型**：`keras.activations.elu(x, alpha=1.0)`

**参数**

- **x**：张量。
- **alpha**：一个标量，表示负数部分的斜率。

融合了sigmoid和ReLU。
$$
f(x) =
\begin{cases}
x, & x\ge0\cr
\alpha(e^x - 1), &x < 0
\end{cases}
$$
函数图像：

![elu.jpg](/resources/mnist/elu.png)

##### selu

可伸缩的指数线性单元，等同于：`scale * elu(x, alpha)`，其中 alpha 和 scale 是预定义的常量。只要正确初始化权重（参见 `lecun_normal` 初始化方法）并且输入的数量「足够大」（参见参考文献获得更多信息），选择合适的 alpha 和 scale 的值，就可以在两个连续层之间保留输入的均值和方差。

##### tanh

也称双切正切函数，映射区间 （-1，1）。
$$
f(x) = \frac{e^x-e^{-x}}{e^x + e^{-x}}
$$
函数图像：

![tanh.jpg](/resources/mnist/tanh.png)

##### softplus

$$
f(x)=log(1+e^x)
$$

函数图像：

![softplus.jpg](/resources/mnist/softplus.jpg)

##### softsign

$$
f(x)=\frac{x}{1+|x|}
$$

函数图像：

![softsign.jpg](/resources/mnist/softsign.jpg)

##### exponential

$$
f(x) = 2^{x-1}
$$

![exponential.png](/resources/mnist/exponential.png)

##### 高级激活函数

对于 Theano/TensorFlow/CNTK 不能表达的复杂激活函数，如含有可学习参数的激活函数，可通过[高级激活函数](https://keras.io/zh/layers/advanced-activations/)实现，可以在 `keras.layers.advanced_activations` 模块中找到。 这些高级激活函数包括 `PReLU` 和 `LeakyReLU`。

<br/>

### summary

**原型**：`Model.summary(line_length=None, positions=None, print_fn=None)`

**功能**：打印模型的字符串摘要。

**参数**

- **line_length**: 打印行的总长度（例如，设置此项以使显示适应不同的终端窗口大小）。
- **positions**: 每行日志元素的相对或绝对位置。如果未提供，则默认为“[.33、.55、.67、1.]`”。
- **print_fn**: 要使用的打印功能。默认为“print”。它将在摘要的每一行调用。可以将其设置为自定义函数以捕获字符串摘要。

**异常**

- **ValueError**: 如果在生成模型之前调用了“summary()”。

<br/>

### compile

**功能**：用于配置训练模型。

**原型**：

```python
Model.compile(
    optimizer="rmsprop",
    loss=None,
    metrics=None,
    loss_weights=None,
    weighted_metrics=None,
    run_eagerly=None,
    **kwargs
)
```

**参数**

* **optimizer**: 优化器，字符串（优化器的名称）或优化器实例。
* **loss**: 损失函数，字符串（目标函数的名称）、目标函数或tf.keras.losses.Loss实例。
* **metrics**: 度量指标列表，可以是字符串（内置函数的名称）、函数或tf.keras.metrics实例。
* **loss_weights**: 指定标量系数（Python float）以加权不同模型输出的损失贡献的可选列表或字典。
* **weighted_metrics**: 在训练和测试期间，按样本权重或类别权重评估和加权的度量列表。
* **run_eagerly**: 布尔值。默认为False。如果为True，则此模型的逻辑将不包含在tf.function。
* **\*\*kwargs**: 额外参数。

**异常**

* **ValueError**: 不合法的 `optimizer`, `loss`,`metrics`参数。

**说明**

通过代码可以看出，此模型可以看作是下面这样的一个函数：

$$
f(x) = Layer3( Layer2( Layer1(x) ) )
$$

第一次训练，每层都会使用一个随机值作为初始参数，第一次训练后会得到一个结果 ${y\\_pred}_1$，`loss`函数会评估${y\\_pred}_1$和实际结果${y\\_true}_1$的差距，而`metrics`则是评估指标，最后根据评估结果，使用`optimizer`优化器来调整每一层的参数，如此循环。

训练完成后，得到一个最终的函数，就是我们的训练结果。

<br/>

#### 优化器（optimizer）

上述说明其实是一个简化的训练过程，在实际使用中，为了向最优化的方向一直前进，优化器不仅考虑一个循环所得结果，还会记录反向传播所得得梯度大小和衰减的动量。

可以在调用`model.compile()`之前初始化一个优化器对象，然后传入该函数，也可以在调用`model.compile()`时传递一个预定义优化器名的string。在后者情形下，优化器的参数将使用默认值。

**默认优化器**

| 名词     | 说明                                                         |
| -------- | ------------------------------------------------------------ |
| RMSprop  | RMSProp算法对权重和偏置的梯度使用了微分平方加权平均数。这种做法有利于消除了摆动幅度大的方向，用来修正摆动幅度，使得各个维度的摆动幅度都较小。另一方面也使得网络函数收敛更快。 |
| SGD      | 随机梯度下降算法。随机梯度下降算法每次只随机选择一个样本来更新模型参数，因此每次的学习是非常快速的，并且可以进行在线更新。 |
| Adam     | 带动量的RMSprop优化器。                                      |
| Adamax   | 它是Adam基于无穷范数的变体。                                 |
| Adagrad  | Adagrad是一个具有参数特定学习率的优化器，它根据训练期间参数更新的频率进行调整。参数接收到的更新越多，学习速率越小。 |
| Adadelta | Adadelta是Adagrad的一个更健壮的扩展，它基于梯度更新的移动窗口调整学习速率，而不是累积所有过去的梯度。这样，即使已经做了很多更新，Adadelta也会继续学习。与Adagrad相比，在Adadelta的原始版本中，您不必设置初始学习速率。在这个版本中，初始学习率和衰减因子可以设置，就像大多数其他的Keras优化器一样。 |
| Nadam    | 带Nesterov动量的RMSprop优化器。                              |

<br/>

#### 损失函数（loss）

##### 概率损失([Probabilistic losses](https://keras.io/api/losses/probabilistic_losses/))

| name                            | 名称           | 说明                                                         |
| ------------------------------- | -------------- | ------------------------------------------------------------ |
| binary_crossentropy             | 二元交叉熵     | 计算预测值与真值的交叉熵，亦称作对数损失。用于二元分类问题。 |
| categorical_crossentropy        | 分类交叉熵     | 计算预测值与真值的多类交叉熵。用于多分类问题。               |
| sparse_categorical_crossentropy | 稀疏分类交叉熵 | 与多类交叉熵相同,适用于稀疏情况。                            |
| poisson                         | 泊松分布损失   | --                                                           |
| kl_divergence                   | KL散度         | 俗称KL距离，常用来衡量两个概率分布的距离。                   |

##### 回归损失([Regression losses](https://keras.io/api/losses/regression_losses/))

| name                           | 名称               | 说明                                                         |
| ------------------------------ | ------------------ | ------------------------------------------------------------ |
| mean_squared_error             | 均方误差           | 计算标签和预测之间误差平方的平均值。                         |
| mean_absolute_error            | 平均绝对误差       | 计算标签和预测之间绝对差的平均值。                           |
| mean_absolute_percentage_error | 平均绝对百分比误差 | 计算标签和预测之间的平均绝对百分比误差。                     |
| mean_squared_logarithmic_error | 均方对数误差       | 计算标签和预测之间的均方对数误差。                           |
| cosine_similarity              | 余弦相似性         | 计算标签和预测之间的余弦相似性。它是介于-1和0之间的负数，其中0表示正交性，而接近-1的值表示更大的相似性。 |
| huber_loss                     | Huber损失          | 计算标签和预测之间的Huber损失。                              |
| log_cosh                       | 双曲余弦的对数     | 计算预测误差的双曲余弦的对数。                               |

##### 合页损失([Hinge losses](https://keras.io/api/losses/hinge_losses/))

用于“最大间隔”分类。

| name              | 名称         | 说明                               |
| ----------------- | ------------ | ---------------------------------- |
| hinge             | 合页损失     | 计算标签和预测之间的合页损失。     |
| squared_hinge     | 平均合页损失 | 计算标签和预测之间的平均合页损失。 |
| categorical_hinge | 分类合页损失 | 计算标签和预测之间的分类合页损失。 |

<br/>

#### 度量指标(metrics)

##### 准确度指标([Accuracy metrics](https://keras.io/api/metrics/accuracy_metrics))

| name                              | 名称                | 说明                                                         |
| --------------------------------- | ------------------- | ------------------------------------------------------------ |
| accuracy                          | 准确度              | 比如我们有6个样本，其真实标签y_true为[0, 1, 3, 3, 4, 2]，但被一个模型预测为了y_pred=[0, 1, 3, 4, 4, 4]，那么该模型的accuracy=4/6=66.67%。 |
| binary_accuracy                   | 二进制精度          | binary_accuracy适用于2分类的情况。binary_accuracy除了y_true和y_pred外，还有一个threshold参数，默认为0.5。比如有6个样本，其y_true为[0, 0, 0, 1, 1, 0]，y_pred为[0.2, 0.3, 0.6, 0.7, 0.8, 0.1]，那么其binary_accuracy=5/6=87.5%。具体计算方法为：1）将y_pred中的每个预测值和threshold对比，大于threshold的设为1，小于等于threshold的设为0，得到y_pred_new=[0, 0, 1, 1, 1, 0]；2）将y_true和y_pred_new代入到accuracy中计算得87.5%。 |
| categorical_accuracy              | 分类准确度          | categorical_accuracy针对的是y_true为onehot标签，y_pred为向量的情况。 |
| top_k_categorical_accuracy        | top_k分类准确度     | --                                                           |
| sparse_top_k_categorical_accuracy | 稀疏top_k分类准确度 | --                                                           |



##### 概率度量([Probabilistic metrics](https://keras.io/api/metrics/probabilistic_metrics))

| name                            | 名称           | 说明 |
| ------------------------------- | -------------- | ---- |
| binary_crossentropy             | 二元交叉熵     | --   |
| categorical_crossentropy        | 交叉损失熵     | --   |
| sparse_categorical_crossentropy | 稀疏交叉损失熵 | --   |
| kullback_leibler_divergence     | KL散度         | --   |
| poisson                         | 泊松散度       | --   |



##### 回归度量([Regression metrics](https://keras.io/api/metrics/regression_metrics))

| name                           | 名称           | 说明 |
| ------------------------------ | -------------- | ---- |
| mean_squared_error             | 均方差         | --   |
| root_mean_squared_error        | 均方根差       | --   |
| mean_absolute_error            | 平均绝对差     | --   |
| mean_absolute_percentage_error | 平均绝对百分差 | --   |
| mean_squared_logarithmic_error | 均方对数差     | --   |
| cosine_similarity              | 余弦相似性     | --   |
| logcosh                        |                | --   |



##### 基于真/假阳性和阴性的分类度量([Classification metrics based on True/False positives & negatives](https://keras.io/api/metrics/classification_metrics))

| name                          | 名称 | 说明 |
| ----------------------------- | ---- | ---- |
| AUC()                         |      | --   |
| Precision()                   |      | --   |
| Recall()                      |      | --   |
| TruePositives()               |      | --   |
| TrueNegatives()               |      | --   |
| FalsePositives()              |      | --   |
| FalseNegatives()              |      | --   |
| PrecisionAtRecall(recall=0.8) |      | --   |
| SensitivityAtSpecificity()    |      | --   |
| SpecificityAtSensitivity()    |      | --   |



##### 图像分割度量([Image segmentation metrics](https://keras.io/api/metrics/segmentation_metrics))

| name                   | 名称 | 说明 |
| ---------------------- | ---- | ---- |
| MeanIoU(num_classes=2) |      | --   |



##### 合页度量([Hinge metrics for "maximum-margin" classification](https://keras.io/api/metrics/hinge_metrics))

| name               | 名称 | 说明 |
| ------------------ | ---- | ---- |
| Hinge()            |      | --   |
| SquaredHinge()     |      | --   |
| CategoricalHinge() |      | --   |

<br/>

### fit

开始训练模型。

**原型**：

```python
Model.fit(
    x=None,
    y=None,
    batch_size=None,
    epochs=1,
    verbose=1,
    callbacks=None,
    validation_split=0.0,
    validation_data=None,
    shuffle=True,
    class_weight=None,
    sample_weight=None,
    initial_epoch=0,
    steps_per_epoch=None,
    validation_steps=None,
    validation_batch_size=None,
    validation_freq=1,
    max_queue_size=10,
    workers=1,
    use_multiprocessing=False,
)
```

**参数**

- **x:** 输入数据， 它可以是：
  - Numpy数组（或类似于数组的数组）或数组列表（如果模型有多个输入）。
  - A TensorFlow tensor, or a list of tensors（如果模型有多个输入）。
  - 如果模型具有命名的输入，则可以使用将输入映射到相应的数组/张量的dict。
  - `tf.data` 类型。返回一个 `(inputs, targets)` 或者 `(inputs, targets, sample_weights)`的元组类型。
  - 一个生成器或者 `keras.utils.Sequence` 返回的`(inputs, targets)` 或者 `(inputs, targets, sample_weights)`。下面给出了迭代器类型(Dataset, generator, Sequence)的解包行为的更详细描述。
- **y**: 目标数据。 与输入数据“x”一样，它可以是Numpy数组或TensorFlow张量。它应该与“x”一致（不能有Numpy输入和张量目标，也不能相反）。如果“x”是数据集、生成器或`keras.utils.Sequence`不应指定实例“y”（因为目标将从“x”获取）。
- **batch_size**: 整数或`None`。每次渐变更新的采样数。如果未指定，将默认为32。 在使用数据集、生成器或`keras.utils.Sequence`实例时，不要指定 `batch_size`，它们自己已经生成了。
- **epochs**: 整数。训练模型的阶段数。epoch是对所提供的整个“x”和“y”数据的迭代。请注意，“epoch”与“initial_epoch”一起被理解为“final epoch”。该模型不针对“epochs”给出的多次迭代进行训练，而只是在达到索引“epochs”的epoch之前。
- **verbose**: 0、1或2。详细模式。0=无声，1=进度条，2=每个epoch一行。请注意，当记录到文件时，进度条不是特别有用，因此建议在不以交互方式运行（例如，在生产环境中）时使用verbose=2。
- **callbacks**: `keras.callbacks.Callback` List。训练所要使用的回调列表。
- **validation_split**: （0， 1）的Float值。要用作验证数据的训练数据的一部分。该模型将把这部分训练数据分开，不进行训练，并将在每个epoch结束时评估该数据的损失和任何模型度量。验证数据是在洗牌前从所提供的“x”和“y”数据中的最后一个样本中选择的。当“x”是数据集、生成器或 `keras.utils.Sequence`实例。
- **validation_data**: 用于评估损失的数据以及每个epoch结束时的任何模型度量。模型将不接受此数据的训练。因此，请注意，使用 `validation_split`或 `validation_data`提供的数据的验证丢失不受规则化层（如noise和dropuout）的影响。`validation_data` 将覆盖 `validation_split`。`validation_data`可以是`(x_val, y_val)`Numpy数组元组，或者张量——`(x_val, y_val, val_sample_weights)`Numpy数组元组。前一种必须提供`batch_size`；后一种情况，可以提供`validation_steps`。注意`validation_data`不支持"x"支持的所有类型，例如dict, generator，keras.utils.Sequence`。
- **shuffle**: Boolean（是否在每个epoch之前打乱训练数据）或str（用于“batch”）。当“x”是生成器，或者，`steps_per_epoch` 不为`None`，将忽略此参数。“batch”是一个特殊的选项，用于处理HDF5数据的限制；它以批处理大小的块进行无序处理。
- **class_weight**: 可选字典将类索引（整数）映射到权重（浮点）值，用于加权损失函数（仅在训练期间）。这有助于告诉模型更加关注来自表示不足的类的样本。
- **sample_weight**: 训练样本的可选Numpy权重数组，用于加权损失函数（仅在训练期间）。 可以传递长度与输入样本相同的平面（1D）Numpy数组（权重和样本之间的1:1映射），或者在时间数据的情况下，可以传递形状为`(samples, sequence_length)`的2D数组，以便对每个样本的每个时间步应用不同的权重。此参数当`x` 是数据集，生成器，或者 `keras.utils.Sequence` 实例，替代sample_weights作为 `x`的第三个元素.
- **initial_epoch**: 整数。开始训练的时间（对恢复以前的训练运行有用）。
- **steps_per_epoch**: 整数或者`None`。在声明一个epoch结束并开始下一个epoch之前的步骤（样本批次）总数。使用输入张量（如TensorFlow数据张量）进行训练时，默认的“None”等于数据集中的样本数除以批大小，如果无法确定，则为1。如果x是`tf.data`数据集，且`steps_per_epoch`为`None`，则epoch将一直运行，直到输入数据集用完为止。传递无限重复的数据集时，必须指定`steps_per_epoch`参数。数组输入不支持此参数。
- **validation_steps**: 仅当提供 `validation_data`且是 `tf.data` 数据集时。在每个epoch结束时执行验证时，在停止之前要绘制的步骤（样本批）总数。如果`validation_steps`是`None`, 则将运行验证，直到 `validation_data` 数据集耗尽。对于无限重复的数据集，它将运行到无限循环中。如果指定了 `validation_steps`，并且只使用部分数据集，则计算将从每个epoch的数据集开始。这样可以确保每次都使用相同的验证样本。
- **validation_batch_size**: 整数或者`None`。每个验证批次的样本数。如果未指定，则默认为`batch_size`。如果你的数据是datasets, generators, `keras.utils.Sequence` 实列，不要声明 `validation_batch_size` 。
- **validation_freq**: 仅当提供验证数据时才相关。整数或`collections_abc.Container` 实例（如 list, tuple...）。如果是整数， 则指定在执行新的验证运行之前要运行多少个培训时段，例如，`validation_freq=2`每两个时段运行一次epoch。如果是容器，则指定要在其上运行验证的时间段，例如`validation_freq=[1，2，10]`在第一、第二和第十个时间段的末尾运行epoch。
- **max_queue_size**: 整数。仅用于generator或`keras.utils.Sequence`输入。生成器队列的最大大小。如果未指定， `max_queue_size`将默认为10。
- **workers**: 整数。仅用于generator或`keras.utils.Sequence`输入。使用基于进程的线程时要启动的最大进程数。如果未指定， `workers` 将默认为1。如果为0，将在主线程上执行生成器。
- **use_multiprocessing**: 布尔值。仅用于generator或`keras.utils.Sequence`输入。 如果为“True”，请使用基于进程的线程。如果未指定，`use_multiprocessing`将默认为“False”。请注意，由于此实现依赖于多处理，因此不应将不可选取的参数传递给生成器，因为它们不容易传递给子进程。

**返回**

函数返回一个`History`的对象，其`History.history`属性记录了损失函数和其他指标的数值随epoch变化的情况，如果有验证集的话，也包含了验证集的这些指标变化情况。

**异常**

* **RuntimeError**：如果模型从未编译过。
* **ValueError**：如果提供的输入数据与模型期望的不匹配。

<br/>

### evaluate

**原型**：

```python
Model.evaluate(
    x=None,
    y=None,
    batch_size=None,
    verbose=1,
    sample_weight=None,
    steps=None,
    callbacks=None,
    max_queue_size=10,
    workers=1,
    use_multiprocessing=False,
    return_dict=False,
)
```

返回测试模式下模型的损失值和度量值。计算是分批完成的。

**参数**

- **x** ：输入数据， 它可以是：
  - Numpy数组（或类似于数组的数组）或数组列表（如果模型有多个输入）。
  - A TensorFlow tensor, or a list of tensors（如果模型有多个输入）。
  - 如果模型具有命名的输入，则可以使用将输入映射到相应的数组/张量的dict。
  - `tf.data` 类型。返回一个 `(inputs, targets)` 或者 `(inputs, targets, sample_weights)`的元组类型。
  - 一个生成器或者 `keras.utils.Sequence` 返回的`(inputs, targets)` 或者 `(inputs, targets, sample_weights)`。下面给出了迭代器类型(Dataset, generator, Sequence)的解包行为的更详细描述。
- **y**：目标数据。 与输入数据“x”一样，它可以是Numpy数组或TensorFlow张量。它应该与“x”一致（不能有Numpy输入和张量目标，也不能相反）。如果“x”是数据集、生成器或`keras.utils.Sequence`，不应指定实例“y”（因为目标将从`iterator/dataset`获取）。
- **batch_size**：整数或`None`。每次渐变更新的采样数。如果未指定，将默认为32。 在使用数据集、生成器或`keras.utils.Sequence`实例时，不要指定 `batch_size`，它们自己已经生成了。
- **verbose**：0或1。详细模式。0=静默，1=进度条。
- **sample_weight**： 测试样本的可选Numpy权重数组，用于加权损失函数。可以传递长度与输入样本相同的平面（1D）Numpy数组（权重和样本之间的1:1映射），或者在时间数据的情况下，可以传递形状为 `(samples, sequence_length)`的2D数组，以便对每个样本的每个时间步应用不同的权重。当“x”是数据集时不支持此参数，而是将样本权重作为“x”的第三个元素传递。
- **steps**： 整数或`None`。在宣告评估回合结束之前的步骤总数（样本批次）。默认忽略，值为`None`。 如果`x` 是一个`tf.data` dataset 并且`steps` is None， 'evaluate' 将运行，直到数据集用完。数组输入不支持此参数。
- **callbacks**： `keras.callbacks.Callback` List。测评时所使用的回调列表。
- **max_queue_size**：整数。仅用于generator或`keras.utils.Sequence`输入。生成器队列的最大大小。如果未指定， `max_queue_size`将默认为10。
- **workers**：整数。仅用于generator或`keras.utils.Sequence`输入。使用基于进程的线程时要启动的最大进程数。如果未指定， `workers` 将默认为1。如果为0，将在主线程上执行生成器。**use_multiprocessing**: 布尔值。仅用于generator或`keras.utils.Sequence`输入。 如果为“True”，请使用基于进程的线程。如果未指定，`use_multiprocessing`将默认为“False”。请注意，由于此实现依赖于多处理，因此不应将不可选取的参数传递给生成器，因为它们不容易传递给子进程。
- **return_dict**：`True`，loss和metric结果将作为dict返回，每个键都是度量的名称。`False`，它们将作为列表返回。

**返回**

标量测试损失（如果模型只有一个输出，没有metrics）或scalars列表（如果模型有多个输出和/或metrics）。属性`model.metrics_names`名称将为标量输出提供显示标签。

**异常**

* **ValueError**：如果参数无效。

<br/>

### predict

**原型**：

```python
Model.predict(
    x,
    batch_size=None,
    verbose=0,
    steps=None,
    callbacks=None,
    max_queue_size=10,
    workers=1,
    use_multiprocessing=False,
)
```

为输入样本生成输出预测。

计算是分批进行的。这种方法是为在大规模输入下的性能而设计的。对于一个批处理中的少量输入，建议直接使用`__call__`以加快执行速度，例如，model(x)或model(x, training=False)，诸如`tf.keras.layers.BatchNormalization`等表现不同的layer。

另外，测试损耗不受regularization layers（如noise和dropout）的影响。

**参数**

- **x:** 输入数据， 它可以是：
  - Numpy数组（或类似于数组的数组）或数组列表（如果模型有多个输入）。
  - A TensorFlow tensor, or a list of tensors（如果模型有多个输入）。
  - `tf.data` 数据集。
  - `generator`或者 `keras.utils.Sequence` 实例。
- **batch_size**: 整数或`None`。每次渐变更新的采样数。如果未指定，将默认为32。 在使用数据集、生成器或`keras.utils.Sequence`实例时，不要指定 `batch_size`，它们自己已经生成了。
- **verbose**: 详细模式，0或1。
- **steps**: 在声明预测轮结束之前的步骤总数（样本批）。默认忽略，值为`None`。如果x是`tf.data` 并且`steps` 是 None。`predict` 将运行，直到输入数据集用完为止。
- **callbacks**: `keras.callbacks.Callback` List。prediction期间使用的回调列表。
- **max_queue_size**：整数。仅用于generator或`keras.utils.Sequence`输入。生成器队列的最大大小。如果未指定， `max_queue_size`将默认为10。
- **workers**: 整数。仅用于generator或`keras.utils.Sequence`输入。使用基于进程的线程时要启动的最大进程数。如果未指定， `workers` 将默认为1。如果为0，将在主线程上执行生成器。
- **use_multiprocessing**: 布尔值。仅用于generator或`keras.utils.Sequence`输入。 如果为“True”，请使用基于进程的线程。如果未指定，`use_multiprocessing`将默认为“False”。请注意，由于此实现依赖于多处理，因此不应将不可选取的参数传递给生成器，因为它们不容易传递给子进程。

**返回**

预测的Numpy数组。

**异常**

- **ValueError**：如果所提供的输入数据与模型的期望不匹配，或者如果有状态模型接收到的样本数不是batch size的倍数。

<br/>

## 名词解释

* **Keras**

  Keras 是一个模型级（model-level）的库，为开发深度学习模型提供了层次的构建模块。它不处理张量操作、求微分等低层次的运算。相反，它依赖于一个专门的、高度优化的张量库来完成这些运算，这个张量库就是 Keras 的后端引擎（backend engine）。Keras 没有选择单个张量库并将 Keras 实现与这个库绑定，而是以模块化的方式处理这个问题。因此，几个不同的后端引擎都可以无缝嵌入到 Keras 中。

  目前，Keras 有三个后端实现：TensorFlow 后端、Theano 后端和微软认知工具包（CNTK，Microsoft cognitive toolkit）后端。通过TensorFlow（或 Theano、CNTK），Keras 可以在 CPU 和 GPU 上无缝运行。

  ![keras.jpg](/resources/mnist/keras soft arch.png)

* **TensorFlow**

  TensorFlow由谷歌人工智能团队，谷歌大脑（Google Brain）开发和维护。是一个基于数据流编程（dataflow programming）的符号数学系统。

  它灵活的架构让你可以在多种平台上展开计算，例如台式计算机中的一个或多个CPU（或GPU），服务器，移动设备等等。

  节点（Nodes）在图中表示数学操作，图中的线（edges）则表示在节点间相互联系的多维数据数组，即张量（tensor）。

* **PyTorch**

  2017年1月，由Facebook人工智能研究院（FAIR）基于Torch推出了PyTorch。它是一个基于Python的可续计算包，提供两个高级功能：1、具有强大的GPU加速的张量计算（如NumPy）。2、包含自动求导系统的的深度神经网络。

* **CNTK**

  CNTK是微软出品的一个开源的深度学习工具包，可以运行在CPU上，也可以运行在GPU上。CNTK的所有API均基于C++设计，因此在速度和可用性上很好。CNTK对外提供了基于C++、C#和Python的接口。

* **Theano**

  Theano 基于 Python，是最早的深度学习开源框架。

  Theano 严格来说是一个擅长处理多维数组的 Python 库，十分适合与其它深度学习库结合起来进行数据探索，高效地解决多维数组的计算问题。它设计的初衷是为了执行深度学习中大规模神经网络算法的运算。其实，Theano 可以被更好地理解为一个数学表达式的编译器：用符号式语言定义你想要的结果，该框架会对你的程序进行编译，在 GPU 或 CPU 中高效运行。

* **Caffe**

  Caffe是一款知名的深度学习框架，由加州大学伯克利分校的贾扬清博士于2013年在Github上发布。自那时起，Caffe在研究界和工业界都受到了极大的关注。Caffe的使用比较简单，代码易于扩展，运行速度得到了工业界的认可，同时还有十分成熟的社区。

  全称是：Convolutional architecture forfast feature embedding（快速特征嵌入的卷积结构），它是一个清晰、高效的深度学习框架，它的license是BSD 2-Clause，核心语言是C++，它支持命令行、Python和Matlab接口，它既可以在CPU上运行也可以在GPU上运行。

  Deep Learning比较流行的一个原因，主要是因为它能够自主地从数据上学到有用的feature。特别是对于一些不知道如何设计feature的场合，比如说图像和speech。

* **Caffe2**

  caffe2 可以看作是 caffe 更细粒度的重构，在实用的基础上，增加了扩展性和灵活性。

  caffe2 宣称是轻量级、模块化和可扩展的一个框架，code once，run anywhere。

* **Caffe2Go**

  Caffe2Go是一个以开源项目Caffe2为基础、使用Unix理念构建的轻量级、模块化框架。其核心架构非常轻量化，而且可以附加多个模块。它是Facebook开发的一个可以在移动平台上实时获取、分析、处理像素的深度学习框架Caffe2Go。考虑到速度是计算密集型移动应用的核心，该框架的轻量化设计让他们可以针对特定平台上定义的操作符进行优化。Caffe2go提升了AI处理速度，让它可以在移动终端上运行。但要实现实时性，并提供高质量、高分辨率的图像，风格转换模型也需要进行优化。Facebook将Caffe2go推上了战略地位，因为“它的大小、速度和灵活性”。

* **MXNet**

  MXNet 是亚马逊（Amazon）选择的深度学习库。它拥有类似于 Theano 和 TensorFlow 的数据流图，为多 GPU 配置提供了良好的配置，有着类似于 Lasagne 和 Blocks 更高级别的模型构建块，并且可以在你可以想象的任何硬件上运行（包括手机）。对 Python 的支持只是其冰山一角—MXNet 同样提供了对 R、Julia、C++、Scala、Matlab，和 Javascript 的接口。

* **Chainer**

  Chainer是一个强大、灵活和直观的深度学习框架。

  Chainer支持CUDA计算。它只需要几行代码就可以利用GPU。它还运行在多个gpu上，几乎不费什么力气。

  Chainer支持各种网络结构，包括前向网、convnets、递归网和递归网。它还支持每批架构。

  正向计算可以包含Python的任何控制流语句，而不缺乏反向传播的能力。它使代码直观且易于调试。

* **PaddlePaddle**

  飞桨*（PaddlePaddle）*以百度多年的深度学习技术研究和业务应用为基础，集深度学习核心框架、基础模型库、端到端开发套件、工具组件和服务平台于一体，2016 年正式开源，是全面开源开放、技术领先、功能完备的产业级深度学习平台。

* **Conda**

  Python, R, Ruby, Lua, Scala, Java, JavaScript, C/ C++, FORTRAN等等的包、依赖和环境管理。

  Conda 是一个开源的软件包管理系统和环境管理系统，用于安装多个版本的软件包及其依赖关系，并在它们之间轻松切换。

  Conda 是为 Python 程序创建的，适用于 Linux，OS X 和Windows，也可以打包和分发其他软件 。

* **MATLAB**

  MATLAB是美国MathWorks公司出品的商业数学软件，用于数据分析、无线通信、深度学习、图像处理与计算机视觉、信号处理、量化金融与风险管理、机器人，控制系统等领域。
  MATLAB是matrix&laboratory两个词的组合，意为矩阵工厂（矩阵实验室），软件主要面对科学计算、可视化以及交互式程序设计的高科技计算环境。它将数值分析、矩阵计算、科学数据可视化以及非线性动态系统的建模和仿真等诸多强大功能集成在一个易于使用的视窗环境中，为科学研究、工程设计以及必须进行有效数值计算的众多科学领域提供了一种全面的解决方案，并在很大程度上摆脱了传统非交互式程序设计语言（如C、Fortran）的编辑模式。
  MATLAB和Mathematica、Maple并称为三大数学软件。它在数学类科技应用软件中在数值计算方面首屈一指。MATLAB可以进行矩阵运算、绘制函数和数据、实现算法、创建用户界面、连接其他编程语言的程序等。MATLAB的基本数据单位是矩阵，它的指令表达式与数学、工程中常用的形式十分相似，故用MATLAB来解算问题要比用C，FORTRAN等语言完成相同的事情简捷得多，并且MATLAB也吸收了像Maple等软件的优点，使MATLAB成为一个强大的数学软件。在新的版本中也加入了对C，FORTRAN，C++，JAVA的支持。

* **Jupyter**

  Jupyter Notebook（此前被称为 IPython notebook）是一个交互式笔记本，支持运行 40 多种编程语言。

  Jupyter Notebook 本质是一个 Web 应用程序，便于创建和共享文学化程序文档，支持实时代码，数学方程，可视化和markdown。 用途包括：数据清理和转换，数值模拟，统计建模，机器学习等等。

  数据挖掘领域中最热门的比赛 Kaggle 里的资料都是 Jupyter 格式。

* **Scikit-learn**

  对Python语言有所了解的科研人员可能都知道SciPy——一个开源的基于Python的科学计算工具包。基于SciPy，目前开发者们针对不同的应用领域已经发展出了为数众多的分支版本，它们被统一称为Scikits，即SciPy工具包的意思。而在这些分支版本中，最有名，也是专门面向机器学习的一个就是Scikit-learn。

  Scikit-learn项目最早由数据科学家David Cournapeau 在2007 年发起，需要NumPy、SciPy和matplotlib等其他包的支持，是Python语言中专门针对机器学习应用而发展起来的一款开源框架。

  作为专门面向机器学习的Python开源框架，它内部实现了各种各样成熟的算法，容易安装和使用，样例丰富，而且教程和文档也非常详细。

  Scikit-learn也有缺点。例如它不支持深度学习和强化学习，这在今天已经是应用非常广泛的技术。此外，它也不支持图模型和序列预测，不支持Python之外的语言，不支持PyPy，也不支持GPU加速。

  看到这里可能会有人担心Scikit-learn的性能表现，这里需要指出的是：如果不考虑多层神经网络的相关应用，Scikit-learn的性能表现是非常不错的。究其原因，一方面是因为其内部算法的实现十分高效，另一方面或许可以归功于Cython编译器；通过Cython在Scikit-learn框架内部生成C语言代码的运行方式，Scikit-learn消除了大部分的性能瓶颈。

  Scikit-learn的基本功能主要被分为六大部分：分类，回归，聚类，数据降维，模型选择和数据预处理。

<br/>

## 参考

* [TensorFlow下利用MNIST训练模型并识别自己手写的数字 - 控球强迫症 - 博客园](https://www.cnblogs.com/XDU-Lakers/p/10526748.html)
* [Keras官方文档](https://keras.io/)
* [Keras.metrics中的accuracy总结 - 知乎](https://zhuanlan.zhihu.com/p/95293440)