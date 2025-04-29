---
title: 线性代数的Python示例
date: 2021-08-26 15:43:00
toc: true
categories:
  - Technology
tags:
  - Python
  - Math
---

<img src="/resources/LearnMatrixByPython/NumPy-Linear-Algebra.jpg" width=522 height=285 />

线性代数是计算机科学中常用的数学工具。

在学习的过程中，本着学以致用的原则，也掌握一下使用 Python 的 numpy 库作为计算工具时的使用方法。

当前的参考书籍是：dlbook_cn_v0.5-beta.pdf

以下示例均运行在 Jupyter Notebook，可以直接[下载](/resources/LearnMatrixByPython/LearnMatrixByPython.ipynb)到自己的电脑进行试验。

<!--more-->

<br/>

## 试验环境

* Python - 3.8.5
* numpy - 1.19.2

## 定义一个矩阵


```python
import numpy as np

# 打印矩阵
def printMatrix(m):
    for ele in m:
        for e in ele:
            print('%3d' % e, end='')
        print('')
        
a = [[1, 2, 3, 4],[5, 6, 7, 8],[9, 10, 11, 12]]

printMatrix(a)

A = np.matrix(a)

print(A)
print("维度数：", A.ndim)
print("在每个维度的大小：", A.shape)
print("元素总数：", A.size)
print("元素数据类型：", A.dtype)
print("元素的字节大小：", A.itemsize)
print("返回Python内置list类型：")
printMatrix(A.tolist())
```

      1  2  3  4
      5  6  7  8
      9 10 11 12
    [[ 1  2  3  4]
     [ 5  6  7  8]
     [ 9 10 11 12]]
    维度数： 2
    在每个维度的大小： (3, 4)
    元素总数： 12
    元素数据类型： int32
    元素的字节大小： 4
    返回Python内置list类型：
      1  2  3  4
      5  6  7  8
      9 10 11 12


## 转置矩阵

矩阵的转置（transpose）是以对角线为轴的镜像，这条从左上角到右下角的对角线被称为 主对角线（main diagonal）。

我们将矩阵$A$ 的转置表示为 $A^⊤$，定义如下：

$$
(A^⊤)_{i,j} = A_{j,i}
$$


```python
AT = np.transpose(A)

print(AT)
```

    [[ 1  5  9]
     [ 2  6 10]
     [ 3  7 11]
     [ 4  8 12]]


## 矩阵相加

只要矩阵的形状一样，我们可以把两个矩阵相加。

两个矩阵相加是指对应位置的元素相加，比如$C = A + B$，其中$C_{i,j} = A_{i,j} + B_{i,j}$。

标量和矩阵相加时，我们只需将其与矩阵的每个元素相乘或相加，比如$D = a · B + c$，其中$D_{i,j} = a · B_{i,j} + c$。

在深度学习中，我们也使用一些不那么常规的符号。我们允许矩阵和向量相加，产生另一个矩阵：$C = A + b$，其中$C_{i,j} = A_{i,j} + b_j$。换言之，向量 b 和矩阵 A 的每一行相加。这个简写方法使我们无需在加法操作前定义一个将向量 b 复制到每一行而生成的矩阵。这种隐式地复制向量 b 到很多位置的方式，被称为广播（broadcasting）。


```python
# 两矩阵相加

print(A+A)

# 标量和矩阵相加

print(A+1)

# 矩阵和向量相加（广播）

b = np.matrix([1,2,3,4])

print(A+b)
```

    [[ 2  4  6  8]
     [10 12 14 16]
     [18 20 22 24]]
    [[ 2  3  4  5]
     [ 6  7  8  9]
     [10 11 12 13]]
    [[ 2  4  6  8]
     [ 6  8 10 12]
     [10 12 14 16]]


## 矩阵相乘

两个矩阵 A 和 B 的 矩阵乘积（matrix product）是第三个矩阵 C。为了使乘法定义良好，矩阵 A 的列数必须和矩阵 B 的行数相等。

如果矩阵 A 的形状是 m × n，矩阵 B 的形状是 n × p，那么矩阵 C 的形状是 m × p。

定义矩阵乘法为 $C = AB$，具体地，该乘法操作定义为： $C_{i,j} = \sum_{k}A_{i,k}B_{k,j}$。


```python
B = np.matrix([2, 2, 2, 2])

B = B.reshape(4, 1)

print(B)

print(A*B)
```

    [[2]
     [2]
     [2]
     [2]]
    [[20]
     [52]
     [84]]


### 元素对应乘积

需要注意的是，两个矩阵的标准乘积不是指两个矩阵中对应元素的乘积。不过，那样的矩阵操作确实是存在的，被称为 元素对应乘积（element-wise product）或者 Hadamard 乘积（Hadamard product），记为 A ⊙ B。


```python
a = np.array([[1,2],[3,4]])

b = np.array([[2,2],[3,3]])

ab = np.multiply(a,b)

print(ab)
```

    [[ 2  4]
     [ 9 12]]


### 点积

点积在数学中，又称数量积（dot product; scalar product），是指接受在实数R上的两个向量并返回一个实数值标量的二元运算。它是欧几里得空间的标准内积。

两个向量$a = [a_1, a_2,…, a_n]$和$b = [b_1, b_2,…, b_n]$的点积定义为：$a·b = a_1·b_1 + a_2·b_2 + … + a_n·b_n$。

两个相同维数的向量 x 和 y 的 点积（dot product）可看作是矩阵乘积 $x^⊤y$。

### 矩阵乘积服从分配律

$A(B + C) = AB + AC$

### 矩阵乘积也服从结合律

$A(BC) = (AB)C$

### 交换律

不同于标量乘积，矩阵乘积并不满足交换律（AB = BA 的情况并非总是满足）。

然而，两个向量的 点积（dot product）满足交换律：$x^⊤y = y^⊤x$。

### 转置的分配

$(AB)^T = B^TA^T$

## 单位矩阵

我们将保持 n 维向量不变的单位矩阵记作 $I_n$。

形式上，$I_n ∈ R^{n×n}$，$∀x ∈ R^n, I_nx = x$。

单位矩阵的结构很简单：所有沿主对角线的元素都是 1，而所有其他位置的元素都是 0。


```python
print(np.eye(3))
```

    [[1. 0. 0.]
     [0. 1. 0.]
     [0. 0. 1.]]


## 逆矩阵

矩阵 $A$ 的 矩阵逆（matrix inversion）记作 $A^{−1}$，其定义的矩阵满足如下条件：$A^{−1}A = I_n$。

使用逆矩阵，可以得到以下求解式：

$Ax = b$

$A^{-1}Ax = A^{-1}b$

$I_nx = A^{-1}b$

$x = A^{-1}b$

使用逆矩阵求解，取决于我们能否找到一个逆矩阵$A^{-1}$。当逆矩阵 $A^{-1}$ 存在时，有几种不同的算法都能找到它的闭解形式。

理论上，相同的逆矩阵可用于多次求解不同向量 b 的方程。然而，逆矩阵 $A^{-1}$ 主要是作为理论工具使用的，并不会在大多数软件应用程序中实际使用。这是因为逆矩阵 $A^{-1}$ 在数字计算机上只能表现出有限的精度，有效使用向量 b 的算法通常可以得到更精确的 x。


```python
A = np.matrix([[2,3],[4,5]])

A_I = A.I

print(A_I)

print(A_I * A)
```

    [[-2.5  1.5]
     [ 2.  -1. ]]
    [[1. 0.]
     [0. 1.]]


## 线性相关


```python
a = np.matrix([[1, 2], [3, 4]])

b = np.matrix([[1, 2], [2, 4]])

if a.ndim > np.linalg.matrix_rank(a):
    print("a线性相关")
else:
    print("a线性无关")
    
if b.ndim > np.linalg.matrix_rank(b):
    print("b线性相关")
else:
    print("b线性无关")
```

    a线性无关
    b线性相关


## 范数

我们经常使用被称为 范数（norm）的函数衡量向量大小。

形式上，$L_p$ 范数定义如下：

$$
\|x\|_p = (\sum_i \vert x_i\vert^p)^{\frac 1 p}
$$

其中 p ∈ R，p ≥ 1。

范数（包括 Lp 范数）是将向量映射到非负值的函数。直观上来说，向量 x 的范数衡量从原点到点 x 的距离。

当 p = 2 时，$L_2$ 范数被称为 欧几里得范数（Euclidean norm）。它表示从原点出发到向量 x 确定的点的欧几里得距离。

当 p = 1 时，$L_1$ 范数是指权值向量中各个元素的绝对值之和，可以产生稀疏权值矩阵（稀疏矩阵指的是很多元素为 0，只有少数元素是非零值的矩阵，即得到的线性回归模型的大部分系数都是 0. ），即产生一个稀疏模型，可以用于特征选择。

另外一个经常在机器学习中出现的范数是 $L^∞$ 范数，也被称为 最大范数（max norm）。这个范数表示向量中具有最大幅值的元素的绝对值：

$$
\|x\|^∞ = \max \limits_i \vert x_i\vert
$$


```python
'''
np.linalg.norm(x, ord=None, axis=None, keepdims=False)

linalg=linear（线性）+algebra（代数），norm则表示范数

* x:表示矩阵（也可以是一维）
* ord:范数类型
ord=2，二范数（默认值）
ord=1，一范数
ord=np.inf，最大范数
* axis:轴向
axis=1，按行向量处理，求多个行向量的范数
axis=0，按列向量处理，求多个列向量的范数
axis=None，矩阵范数。
* keepdims:是否保持矩阵的二维特性
'''

a = np.matrix([3, 4])

print(np.linalg.norm(a, 2, 1))
print(np.linalg.norm(a, 1, 1))
print(np.linalg.norm(a, np.inf, 1))
```

    [5.]
    [7.]
    [4.]


### Frobenius 范数

有时候我们可能也希望衡量矩阵的大小。在深度学习中，最常见的做法是使用 Frobenius 范数（Frobenius norm），其类似于向量的 L2 范数。

$$
\|A\|_F = \sqrt {\sum_{i,j} {A_{i,j}}^2}
$$


```python
a = np.matrix([[0, 4], [4, 1]])

print(np.linalg.norm(a))

print(np.sqrt(33))
```

    5.744562646538029
    5.744562646538029


## 特征分解

许多数学对象可以通过将它们分解成多个组成部分或者找到它们的一些属性而更好地理解。

例如，整数可以分解为质因数。我们可以用十进制或二进制等不同方式表示整数 12，但是 12 = 2 × 2 × 3 永远是对的。从这个等式中我们可以获得一些有用的信息，比如 12 不能被 5 整除，或者 12 的倍数可以被 3 整除。

正如我们可以通过分解质因数来发现整数的一些内在性质，我们也可以通过分解矩阵来发现矩阵表示成数组元素时不明显的函数性质。

特征分解（eigendecomposition）是使用最广的矩阵分解之一，即我们将矩阵分解成一组特征向量和特征值。

定义：对于给定矩阵A，如果存在一个常数λ（可以为复数）和非零向量x，满足$Ax=λx$，则称λ为A的特征值，x为对应于λ的特征向量。

例：设 $A = \begin{bmatrix} 1 & 6 \\ 5 & 2 \end{bmatrix}$，$u = \begin{bmatrix} 6 \\ -5 \end{bmatrix}$，$u$是否是A的特征向量？

解：$Au = \begin{bmatrix} 1 & 6 \\ 5 & 2 \end{bmatrix} \begin{bmatrix} 6 \\ -5 \end{bmatrix} = \begin{bmatrix} -24 \\ 20 \end{bmatrix} = -4 \begin{bmatrix} 6 \\ -5 \end{bmatrix} = -4u$，因此，$u$是特征值-4对应的特征向量，故$u$是A的特征向量。

如果 v 是 A 的特征向量，那么任何缩放后的向量 sv (s ∈ R，$s \neq 0$) 也是 A 的特征向量。此外，sv 和 v 有相同的特征值。基于这个原因，通常我们只考虑单位特征向量。


```python
A = np.matrix([[1, 6], [5, 2]])

eigenvalue, featurevector = np.linalg.eig(A)

print(eigenvalue, featurevector)

# 根据特征值和特征向量得到原矩阵

print(featurevector * np.diag(eigenvalue) * np.linalg.inv(featurevector))
```

    [-4.  7.] [[-0.76822128 -0.70710678]
     [ 0.6401844  -0.70710678]]
    [[1. 6.]
     [5. 2.]]


使用numpy进行分解时，对于同一个矩阵可能产生多组特征值和特征向量。其结果符合以下定义：

假设矩阵 A 有 n 个线性无关的特征向量 {v(1), . . . , v(n)}，对应着特征值{λ1, . . . , λn}。我们将特征向量连接成一个矩阵，使得每一列是一个特征向量：V = [v(1), . . . , v(n)]. 类似地，我们也可以将特征值连接成一个向量 λ = [λ1, . . . , λn]⊤。

因此 A 的 特征分解（eigendecomposition）可以记作

$$
A = Vdiag(λ)V^{−1}
$$

## 奇异值分解

除了特征分解，还有另一种分解矩阵的方法，被称为 奇异值分解（singular value decomposition, SVD），将矩阵分解为 奇异向量（singular vector）和 奇异值（singular value）。

奇异值分解有更广泛的应用。每个实数矩阵都有一个奇异值分解，但不一定都有特征分解。例如，非方阵的矩阵没有特征分解，这时我们只能使用奇异值分解。

奇异值分解将矩阵 A 分解成三个矩阵的乘积：

$$
A = UDV^⊤
$$

假设 A 是一个 m × n 的矩阵，那么 U 是一个 m × m 的矩阵，D 是一个 m × n的矩阵，V 是一个 n × n 矩阵。矩阵 U 和 V 都定义为正交矩阵，而矩阵 D 定义为对角矩阵。注意，矩阵 D 不一定是方阵。

对角矩阵 D 对角线上的元素被称为矩阵 A 的 奇异值（singular value）。矩阵 U 的列向量被称为 左奇异向量（left singular vector），矩阵 V 的列向量被称 右奇异向量（right singular vector）。

事实上，我们可以用与 A 相关的特征分解去解释 A 的奇异值分解。A 的 左奇异向量（left singular vector）是 $AA^⊤$ 的特征向量。A 的 右奇异向量（right singular vector）是 $A^⊤A$ 的特征向量。A 的非零奇异值是 $A^⊤A$ 特征值的平方根，同时也是 $AA^⊤$ 特征值的平方根。


```python
A = np.matrix([[2, 4], [1, 3], [0, 0]])

print(np.linalg.svd(A))
```

    (matrix([[-0.81741556, -0.57604844,  0.        ],
            [-0.57604844,  0.81741556,  0.        ],
            [ 0.        ,  0.        ,  1.        ]]), array([5.4649857 , 0.36596619]), matrix([[-0.40455358, -0.9145143 ],
            [-0.9145143 ,  0.40455358]]))


## Moore-Penrose 伪逆

对于非方矩阵而言，其逆矩阵没有定义。所以就不能使用逆矩阵求解。

而对于非方矩阵而言，如果矩阵 A 的行数大于列数，那么方程可能没有解。如果矩阵 A 的行数小于列数，那么矩阵可能有多个解。

Moore-Penrose 伪逆（Moore-Penrose pseudoinverse）使我们在这类问题上取得了一定的进展。矩阵 A 的伪逆定义为：

$$
A^+ = \lim_{α↘0}(A^⊤A + αI)^{−1}A^⊤
$$

计算伪逆的实际算法没有基于这个定义，而是使用下面的公式：

$$
A^+ = VD^+U^⊤
$$

其中，矩阵  V，D 和 U 是矩阵 A 奇异值分解后得到的矩阵。对角矩阵 D 的伪逆 D+ 是其非零元素取倒数之后再转置得到的。

当矩阵 A 的列数多于行数时，使用伪逆求解线性方程是众多可能解法中的一种。特别地，$x = A^+y$ 是方程所有可行解中欧几里得范数 $\|x\|_2$ 最小的一个。

当矩阵 A 的行数多于列数时，可能没有解。在这种情况下，通过伪逆得到的 x 使得 Ax 和 y 的欧几里得距离 $\|Ax − y\|_2$ 最小。


```python
A = np.zeros((4, 4))
A[0, -1] = 1
A[-1, 0] = -1
A = np.matrix(A)

print(A)

print(np.linalg.pinv(a))   # 求矩阵 A 的伪逆（广义逆矩阵），对应于MATLAB中 pinv() 函数
```

    [[ 0.  0.  0.  1.]
     [ 0.  0.  0.  0.]
     [ 0.  0.  0.  0.]
     [-1.  0.  0.  0.]]
    [[-6.25000000e-02  2.50000000e-01]
     [ 2.50000000e-01  4.15617076e-17]]


## 迹运算

迹运算返回的是矩阵对角元素的和：

$$
Tr(A) = \sum_i A_{i,i}
$$


```python
A = np.matrix([[2, 4], [1, 3]])

print(A)

print(np.trace(A))
```

    [[2 4]
     [1 3]]
    5


## 参考文档

* dlbook_cn_v0.5-beta.pdf
