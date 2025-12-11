---
title: 一批产品要达到90%的合格率，需要测试多少个合格？
date: 2021-04-25 23:51:12
toc: true
categories:
  - tech
tags:
  - Math
---

![Cover](/resources/estimated_pass_rate/cover.webp)

最近在做一批硬件产品的合格测试的时候，测到了一半1个有问题的都没有，不禁想不会剩下的都没问题吧！一定要全测完吗？

为了不浪费时间，我们来稍作思考。

<!--more-->

<br/>

### 推理

假设现有一批产品100个，测试顺序完全随机。如果要达到90%的合格率，那么至少就要有90个合格。但如果前90个全部合格，那么按照此时90%的合格率，保守估计剩下的10个里面应该也有9个是合格的，那么此时可预测的合格率就不是90%了，而是99%。

### 公式

根据上诉推理，假设产品总数为$N$，已测试为$t$，测试合格为$n$，测试不合格为$m$，可得以下几个公式：

* 实际合格率：$\frac n N$
* 预估（保守）合格率：$\frac {n+n/N*(N-t)} N$
* 预估（激进）合格率：$\frac {n+n/t*(N-t)} N$
* 预估（平衡）合格率：$\frac {n + (\frac {n} {2N} + \frac {n} {2t}) *(N-t)} N$
* 实际不合格率：$\frac m N$
* 预估（保守）不合格率：$\frac {m+m/N*(N-t)} N$
* 预估（激进）不合格率：$\frac {m+m/t*(N-t)} N$
* 预估（平衡）不合格率：$\frac {m + (\frac {m} {2N} + \frac {m} {2t}) *(N-t)} N$

### 说明

预估合格率的中心思想，就是以当前的测试情况来预估剩下未测试的产品中有多少合格/不合格数，再加上已经测试的，除以总数。

在预估的时候，就有两个情况，一是使用当前的实际合格率作为剩余产品的合格/不合格率来预估，二是使用合格/不合格数在已测试的产品中的占比，作为剩余产品的合格/不合格率来预估。

按理来说，使用已有的测试数据中的合格/不合格率来预测剩下的产品的合格/不合格数量的第二种预测方式是符合常理的，就是以以往的数据来预测未来嘛。但是这个公式在数量较少的时候往往是比较过激的，比如测试了1个并且这个正好是合格的时候，此时的预估合格率就是100%；而第一种预估方法，在测试了90个合格之后，也不敢说剩下的10个中全是合格的，相对来说就比较保守了。

所以将二者结合的算法，平均了二者的预测率，是比较平衡的一种估值方式。

### [Python脚本](/resources/estimated_pass_rate/estimated_pass_rate.py)

```python
# -*- coding: UTF-8 -*-
# estimated_pass_rate.py

def actualPassedRate(total, passed):
    return passed/total

def estimatedConservativePassedRate(total, tested, passed):
    estimated_passed = passed/total * (total - tested)
    return (estimated_passed + passed) / total

def estimatedRadicalPassedRate(total, tested, passed):
    estimated_passed = passed/tested * (total - tested)
    return (estimated_passed + passed) / total

def estimatedBalancedPassedRate(total, tested, passed):
    estimated_passed = (passed/tested + passed/total) * (total - tested)
    estimated_passed /= 2
    return (estimated_passed + passed) / total

def actualFailedRate(total, failed):
    return failed/total

def estimatedConservativeFailedRate(total, tested, failed):
    estimated_failed = failed/total * (total - tested)
    return (estimated_failed + failed) / total

def estimatedRadicalFailedRate(total, tested, failed):
    estimated_failed = failed/tested * (total - tested)
    return (estimated_failed + failed) / total

def estimatedBalancedFailedRate(total, tested, failed):
    estimated_failed = (failed/tested + failed/total) * (total - tested)
    estimated_failed /= 2
    return (estimated_failed + failed) / total



if __name__ == "__main__":
    N = 100
    t = 60
    n = 60
    m = t - n

    print("总测试数：{}  已测试：{}  合格数：{}  不合格数：{}".format(N, t, n, m))

    print("实际合格率：{:.1%}".format( actualPassedRate(N, n) ))
    print("预估（保守）合格率：{:.1%}".format( estimatedConservativePassedRate(N, t, n) ))
    print("预估（激进）合格率：{:.1%}".format( estimatedRadicalPassedRate(N, t, n) ))
    print("预估（平衡）合格率：{:.1%}".format( estimatedBalancedPassedRate(N, t, n) ))

    print("实际不合格率：{:.1%}".format( actualFailedRate(N, m) ))
    print("预估（保守）不合格率：{:.1%}".format( estimatedConservativeFailedRate(N, t, m) ))
    print("预估（激进）不合格率：{:.1%}".format( estimatedRadicalFailedRate(N, t, m) ))
    print("预估（平衡）不合格率：{:.1%}".format( estimatedBalancedFailedRate(N, t, m) ))
```

输出：

```
总测试数：100  已测试：55  合格数：55  不合格数：0
实际合格率：55.0%
预估（保守）合格率：79.8%
预估（激进）合格率：100.0%
预估（平衡）合格率：89.9%
实际不合格率：0.0%
预估（保守）不合格率：0.0%
预估（激进）不合格率：0.0%
预估（平衡）不合格率：0.0%
```

### 最后

上例就可以回答我们的问题啦，只要连续测试55%个合格的产品，这批产品的合格率预估就已经达到90%了。

这里的公式有一定的参考意义，但是在测试数量较少的情况下，还是全测吧。

