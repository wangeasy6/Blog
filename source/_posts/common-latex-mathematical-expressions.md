---
title: 常用的Latex数学表达式
date: 2021-11-06 17:03:00
toc: true
categories:
  - Technology
tags:
  - Math
---

![Cover.jpg](/resources/Cover/latex.jpg)

Latex 是一种基于ΤΕΧ的排版系统，对于生成复杂的数学公式，LaTeX表现的尤为出色，因此它非常适用于生成高印刷质量的科技和数学类文档。

<!--more-->

<br/>

## 希腊字母

### 小写希腊字母

| 字母        | Latex表达式    | 字母          | Latex表达式      |
| ----------- | --------- | ------------- | ----------- |
| $\alpha$    | \alpha    | $\beta$       | \beta       |
| $\gamma$    | \gamma    | $\delta$      | \delta      |
| $\epsilon$  | \epsilon  | $\varepsilon$ | \varepsilon |
| $\zeta$     | \zeta     | $\eta$        | \eta        |
| $\theta$    | \theta    | $\vartheta$   | \vartheta   |
| $\iota$     | \iota     | $\kappa$      | \kappa      |
| $\lambda$   | \lambda   | $\mu$         | \mu         |
| $\nu$       | \nu       | $\xi$         | \xi         |
| $o$         | o         | $\pi$         | \pi         |
| $\omega$    | \omega    | $\rho$        | \rho        |
| $\sigma$    | \sigma    | $\varsigma$   | \varsigma   |
| $\tau$      | \tau      | $\upsilon$    | \upsilon    |
| $\phi$      | \phi      | $\varphi$     | \varphi     |
| $\chi$      | \chi      | $\psi$        | \psi        |

### 大写希腊字母

| 字母       | Latex表达式    | 字母      | Latex表达式  |
| ---------- | --------- | --------- | ------- |
| $\Gamma$   | \Gamma    | $\Lambda$ | \Lambda |
| $\Sigma$   | \Sigma    | $\Delta$  | \Delta  |
| $\Psi$     | \Psi      | $\Xi$     | \Xi     |
| $\Upsilon$ | \Upsilon  | $\Omega$  | \Omega  |
| $\Theta$   | \Theta    | $\Pi$     | \Pi     |
| $\Phi$     | \Phi      |           |         |

## 乘法

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $ab$         | `ab`          |
| $a \cdot b$  | `a \cdot b`   |
| $a \times b$ | `a \times b`  |

## 除法

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $x = \frac a b$ | `\frac a b`   |
| $x = \dfrac a b$ | `\dfrac a b`（不压缩行间距） |
| $a \div b$   | `a \div b`    |

## 正负号

| 数学式       | Latex表达式    | 数学式      | Latex表达式  |
| ------------ | --------- | --------- | ------- |
| $a \pm b$    | `a \pm b`   | $a \mp b$ | `a \mp b` |

## 绝对值

| 数学式                         | Latex表达式                  |
| ------------------------------ | --------------------------- |
| $\left&#124; a \right&#124;$   | \left&#124; a \right&#124;  |

## 无线循环小数

| 数学式              | Latex表达式       |
| ------------------- | ----------------- |
| $1.\dot{3}$         | 1.\dot{3}         |
| $1.\dot{1}2\dot{5}$ | 1.\dot{1}2\dot{5} |

## 关系表达符号

| 数学式     | Latex表达式  |
| ---------- | ----------- |
| $<$        | `<`         |
| $>$        | `>`         |
| $\leq$     | `\leq`      |
| $\le$      | `\le`       |
| $\geq$     | `\geq`      |
| $\ge$      | `\ge`       |
| $\approx$  | `\approx`   |
| $\neq$     | `\neq`      |
| $\ne$      | `\ne`       |
| $\equiv$   | `\equiv`    |
| $\overset{over}{=}$ | `\overset{over}{=}` |
| $\underset{\text{under}}{=}$ | `\underset{\text{under}}{=}` |
| $\require{extpfeil} a \xlongequal[over]{under} b$ | `a \xlongequal[over]{under} b` |

## 指数（幂）

$a^n = b$，n为指数，b为幂。

| 数学式      | Latex表达式   |
| ----------- | ------------ |
| $a^n = b$   | `a^n = b`    |

## 开根号

| 数学式             | Latex表达式         |
| ------------------ | ------------------ |
| $\sqrt a$ | `\sqrt a` |
| $\sqrt[n]{b} = a$  | `\sqrt[n]{b} = a`  |

## 对数运算

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $log_ab = n$ | `log_ab = n`  |
| $\lg 10 = 1$ | `\lg 10 = 1` |
| $\ln e = 1$ | `\ln e = 1` |

## 三角函数

| 函数名     | 数学式                                                    | Latex表达式 |
| ---------- | --------------------------------------------------------- | ----------- |
| 正弦函数   | $y=\sin x$                                                | `\sin x`      |
| 余弦函数   | $y = \cos x$                                              | `\cos x`      |
| 正切函数   | $y = \tan x = \frac {\sin x} {\cos x}$                    | `\tan x`      |
| 余切函数   | $y = \cot x = \frac 1 {\tan x} = \frac {\cos x} {\sin x}$ | `\cot x`      |
| 正割函数   | $y = \sec x = \frac 1 {\cos x}$                           | `\sec x`      |
| 余割函数   | $y = \csc x = \frac 1 {\sin x}$                           | `\csc x`      |
| 反正弦函数 | $y = \arcsin x$                                           | `\arcsin x`   |
| 反余弦函数 | $y = \arccos x$                                           | `\arccos x`   |
| 反正切函数 | $y = \arctan x$                                           | `\arctan x`   |
| 反余切函数 | $\newcommand{\arccot}{\mathrm{arccot}\,} y = \arccot x$                                           | `\newcommand{\arccot}{\mathrm{arccot}\,} y = \arccot x`   |

## 分段函数

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $f(x) = \begin{cases} x^2 & x \ge 0 \\ x & x < 0 \end{cases}$   |  `f(x) = \begin{cases} x^2 & x \ge 0 \\ x & x < 0 \end{cases}`   |

## 求和符号

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $p = \sum\limits_{n=1}^N {a_n = a_1 + a_2 + ... + a_N}$ | `p = \sum\limits_{n=1}^N {a_n = a_1 + a_2 + ... + a_N}` |

## 求积符号

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $p =\prod\limits_{n=1}^N {a_n = a_1 \times a_2 \times ... \times a_N}$ | `p =\prod\limits_{n=1}^N {a_n = a_1 \times a_2 \times ... \times a_N}` |

## 极限

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $\lim\limits_{x \to a}{\frac {p(x)} {q(x)}}$ | `\lim\limits_{x \to a}{\frac {p(x)} {q(x)}}` |
| $\lim\limits_{x \to 0^+}$ | `\lim\limits_{x \to 0^+}` |
| $\lim\limits_{x \to -\infty}$ |  `\lim\limits_{x \to -\infty}`  |
| $\lim\limits_{x \to +\infty}$ |  `\lim\limits_{x \to +\infty}`  |
| $\lim\limits_{x \to \infty}$ | `\lim\limits_{x \to \infty}` |


## 微积分

| 名称     | 数学式                    | Latex表达式                |
| -------- | ------------------------- | -------------------------- |
| 微分     | $f'$                      | `f'`                       |
| 不定积分 | $\int x \,dx$             | `\int x \,dx`              |
| 双重积分 | $\iint_D x \,dx$          | `\iint_D x \,dx`           |
| 三重积分 | $\iiint x \,dx$           | `\iiint x \,dx`            |
| 定积分   | $\int_{a}^{b} x \,dx$     | `\int_{a}^{b} x \,dx`      |
| 环路积分 | $\oint_L P \,dx + Q \,dy$ | `$\oint_L P \,dx + Q \,dy` |


## 线性代数

**行列式：**
$$
\begin{vmatrix}
a & b \\
c & d
\end{vmatrix}
$$

```latex
\begin{vmatrix}
a & b \\
c & d
\end{vmatrix}
```

**矩阵：**
$$
\begin{bmatrix}
a & b \\
c & d
\end{bmatrix}
$$

```latex
\begin{bmatrix}
a & b \\
c & d
\end{bmatrix}
```


$$
\begin{pmatrix}
a & b \\
c & d
\end{pmatrix}
$$

```latex
\begin{pmatrix}
a & b \\
c & d
\end{pmatrix}
```

$$
\begin{Bmatrix}
a & b \\
c & d
\end{Bmatrix}
$$

```latex
\begin{Bmatrix}
a & b \\
c & d
\end{Bmatrix}
```

矩阵范数：
$$
\begin{Vmatrix}
a & b \\
c & d
\end{Vmatrix}
$$

```latex
\begin{Vmatrix}
a & b \\
c & d
\end{Vmatrix}
```

带省略的矩阵：
$$
\begin{bmatrix}
a & \cdots & b \\
\vdots & \ddots & \vdots \\
c & \cdots & d
\end{bmatrix}
$$

```latex
\begin{bmatrix}
a & \cdots & b \\
\vdots & \ddots & \vdots \\
c & \cdots & d
\end{bmatrix}
```

横三点是\cdots，竖三点为\vdots，而斜三点为\ddots

### 矩阵内加划分线条

在矩阵内我们有时候需要加上各种线条，尤其在分块矩阵的时候特别管用。但这时如果还用matrix环境就比较麻烦了。通常我们用array的方式来实现。

在array环境下，表示对齐方式的字母中间加上一个小竖线就可以在矩阵里面生成竖线了。
$$
\left [
\begin{array}{c|c}
a & b \\
c & d
\end{array}
\right ]
$$

```latex
\left [
\begin{array}{c|c}
a & b \\
c & d
\end{array}
\right ]
```

而横线则是在每一行的末尾（或者下一行的开头）加上\hline
$$
\left [
\begin{array}{c}
a & b \\
\hline
c & d
\end{array}
\right ]
$$

```latex
\left [
\begin{array}{c}
a & b \\
\hline
c & d
\end{array}
\right ]
```

竖虚线是冒号，横虚线是\hdashline
$$
\left [
\begin{array}{c:c}
a & b \\
\hdashline
c & d
\end{array}
\right ]
$$

```latex
\left [
\begin{array}{c:c}
a & b \\
\hdashline
c & d
\end{array}
\right ]
```

<br/>

## 概率与统计

| 名称          | 数学式          | Latex表达式 |
| ------------- | --------------- | ----------- |
| 必然事件      | $\Omega$        | \Omega      |
| 不可能事件    | $\varnothing$   | \varnothing |
| 包含          | $A \subset B$   | \subset     |
| 相交（积）    | $A \cap B = AB$ | \cap        |
| 相并（和/或） | $A \cup B$      | \cup        |
| 对立          | $\overline A$   | \overline A |

<br/>

## 参考

* [莱斯大学LaTex Math在线PDF手册](https://www.caam.rice.edu/~heinken/latex/symbols.pdf)
* [如何用latex编写矩阵（包括各类复杂、大型矩阵）？](https://zhuanlan.zhihu.com/p/266267223)
* [MathJax Documentation](https://docs.mathjax.org/en/latest/)
* [Supported TeX/LaTeX commands — MathJax](https://docs.mathjax.org/en/latest/input/tex/macros/index.html)
* [The TeX/LaTeX Extension List — MathJax](https://docs.mathjax.org/en/latest/input/tex/extensions/index.html)

