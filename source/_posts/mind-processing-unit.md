---
title: 心智处理器
toc: true
date: 2024-10-14 14:30:00
categories:
  - ithink
---


```c
LifeTimeLoop(){
    吃饭;
    睡觉;
    打豆豆;
}
```

人的大脑其实在很多时候都类似于一个单核处理器，关键在于我们的调度和执行。

<!--more-->

<br/>

## 处理流程

其调度流程类比如下：

```python
任务表{
    项目计划表,
    年计划表,
    月计划表,
    周计划表,
    日计划表（每日事项、从以上计划中筛选出来的任务）,
    ToDoList（杂项）
}

全局数据: 任务表、中断事务表、日志

硬中断:
    保存（当前任务数据）
    执行中断请求

软中断（杂事）:
    保存（当前任务数据）
    加入中断事务表（中断事件）
    if 中断事件优先级 is Max:
        执行中断事件
        删除中断事务表（中断事件）
        更新日志

中断注册:
    硬中断（吃喝拉撒、身体不适等）
    软中断（杂事）

获取任务:
    遍历任务表、中断事务表
    判断任务优先级（紧急程度 & 重要程度）
    return 下一任务

执行任务：
    分解任务
    更新任务表（分解任务）
    for 子任务 in 分解任务:
        执行子任务
        if timeout:
            break
    更新任务表（当前状态）
    更新日志

main():
    while:
        next_task = 获取任务()
        set_timeout_may_be()
        if next_task:
            执行任务()
            have_a_rest_may_be()
        else:
            flow_for_a_period_of_time()
```

## 调度器

### 数据结构

#### 人生清单

人生的意义是自己定义的，设立自己的终局目标即为设置了自己的人生算法的启发式，它将会指引你实现自己的意义。

下载 [人生清单.xlsx](/resources/mind-processing-unit/人生清单.xlsx)，内容示例如下：

![人生清单](/resources/mind-processing-unit/人生清单.png)

#### 年度计划表

**每年的元旦假期**:

* 总结：上一年的 年度计划
* 制定：下一年的 **年度计划表**、**季度计划表**、**月计划表**

下载 [年度计划表.xlsx](/resources/mind-processing-unit/年度计划表.xlsx)，内容示例如下：

![年度计划表](/resources/mind-processing-unit/年度计划表.png)

#### 季度计划表

**每季度末**：

* 总结：本季度
* 调整：下季度计划、年度计划、月计划

下载 [季度计划表.xlsx](/resources/mind-processing-unit/季度计划表.xlsx)，内容示例如下：

![季度计划表](/resources/mind-processing-unit/季度计划表.png)

#### 月计划表

**每月末当日**：

* 总结：本月
* 调整：下月计划、季度计划、年度计划
* 制定：周计划

下载 [月计划表.xlsx](/resources/mind-processing-unit/月计划表.xlsx)，内容示例如下：

![月计划表](/resources/mind-processing-unit/月计划表.png)

一个月的总工作日约为：20d - 22d，总工时约为：200h - 220h。

#### 周计划表

**每周日的晚上**：

* 总结：本周
* 调整：下周计划
* 制定：日计划

下载 [周计划表.xlsx](/resources/mind-processing-unit/周计划表.xlsx)，内容示例如下：

![周计划表](/resources/mind-processing-unit/周计划表.png)

每周的总工时约为：50h - 60h，可拆分为：上班 40h、下班 20h。

#### 日程表

**每天晚上**：

* 总结：当日计划
* 调整：明日计划

下载 [每天三件事.xlsx](/resources/mind-processing-unit/每天三件事.xlsx)，内容示例如下：

![每天三件事](/resources/mind-processing-unit/每天三件事.png)

#### ToDo List

以 A5 笔记本纸进行管理。

#### 中断向量表

以 A5 笔记本纸进行管理。

#### 娱乐

以 A5 笔记本纸进行管理。

#### 日志

日志模板。 



## 待添加部分

* 调度算法
* 执行器

