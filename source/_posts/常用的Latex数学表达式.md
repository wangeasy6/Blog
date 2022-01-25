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

## 正负号

| 数学式       | Latex表达式    | 数学式      | Latex表达式  |
| ------------ | --------- | --------- | ------- |
| $a \pm b$    | `a \pm b`   | $a \mp b$ | `a \mp b` |

## 乘法

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $ab$         | `ab`          |
| $a \cdot b$  | `a \cdot b`   |
| $a \times b$ | `a \times b`  |

## 除法

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $\frac a b$  | `\frac a b`   |
| $a \div b$   | `a \div b`    |

## 绝对值

| 数学式                         | Latex表达式                  |
| ------------------------------ | --------------------------- |
| $\left&#124; a \right&#124;$   | \left&#124; a \right&#124;  |

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

## 指数（幂）

$a^n = b$，n为指数，b为幂。

| 数学式      | Latex表达式   |
| ----------- | ------------ |
| $a^n = b$   | `a^n = b`    |

## 开根号

| 数学式             | Latex表达式         |
| ------------------ | ------------------ |
| $\sqrt[n]{b} = a$  | `\sqrt[n]{b} = a`  |

## 对数运算

| 数学式       | Latex表达式    |
| ------------ | ------------- |
| $log_ab = n$ | `log_ab = n`  |

## 极限

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $\lim_{x \to a}{\frac {p(x)} {q(x)}}$   |  `\lim_{x \to a}{\frac {p(x)} {q(x)}}`   |
| $\lim_{x \to 0^+}$       |  `\lim_{x \to 0^+}`      |
| $\lim_{x \to -\infty}$   |  `\lim_{x \to -\infty}`  |
| $\lim_{x \to +\infty}$   |  `\lim_{x \to +\infty}`  |
| $\lim_{x \to \infty}$    |  `\lim_{x \to \infty}`   |


## 分段函数

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $f(x) = \begin{cases} x^2 & x \ge 0 \\ x & x < 0 \end{cases}$   |  `f(x) = \begin{cases} x^2 & x \ge 0 \\ x & x < 0 \end{cases}`   |

## 求和符号

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $p = \sum_{n=1}^N {a_n = a_1 + a_2 + ... + a_N}$   |  `p = \sum_{n=1}^N {a_n = a_1 + a_2 + ... + a_N}`   |

## 求积符号

| 数学式       | Latex表达式    |
| ---------- | --------- |
| $p =\prod_{n=1}^N {a_n = a_1 \times a_2 \times ... \times a_N}$   |  `p =\prod_{n=1}^N {a_n = a_1 \times a_2 \times ... \times a_N}`   |

## 参考

* [莱斯大学LaTex Math在线PDF手册](https://www.caam.rice.edu/~heinken/latex/symbols.pdf)