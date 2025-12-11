---
title: 基于51job的嵌入式软件行业分析
date: 2020-11-10 23:14:00
toc: true
categories:
  - tech
tags:
  - Python
  - Data
  - 嵌入式
---

![cover.png](/resources/51job_data_analysis/cover.png)

依靠上一篇的[51Job数据抓取](/tech/51Job数据抓取/)程序，本篇我以嵌入式软件行业为例，基于51Job上的职位信息进行了一些简单的分析。

只要稍微更改搜索关键词就可以十分方便的应用在其它行业。

<!--more-->

<br/>

### 哪个城市的嵌入式需求更多？

思路：根据51job的所有城市名，获取关键词——“嵌入式”的搜索页数，以页数作为城市需求量的热力值，绘制城市热力图。

结果：绘制的城市热力图如下，也可以查看网页版的[嵌入式城市热力图.html](/resources/51job_data_analysis/嵌入式城市热力图.html)，支持缩放和鼠标悬浮显示数据等操作：

![嵌入式城市热力图.png](/resources/51job_data_analysis/嵌入式城市热力图.png)

我把城市热力值按颜色分了3个层次——青、红、黑。

黑色：60往上的属于第一梯队，深圳131、上海103；

红色：60-30的属于第二梯队，广州40、杭州36、北京34、南京34、武汉34、苏州31；

青色：30-8的属于第三梯队，成都27、西安19、合肥13、无锡12、长沙11、东莞10、重庆8、宁波8、佛山8、珠海8；

[代码](/resources/51job_data_analysis/city_demand_analysis.py)如下：

```python
# coding:utf-8
# requirement: pyecharts 1.7.1

from spider_51job import *
from pyecharts import options as opts
from pyecharts.charts import Map

all_code = get_city_code()
city_code = {}
for i in all_code.keys():
    if "区" in i or "县" in i or \
        "镇" in i or "省" in i:
        continue
    if i == "国外":
        break
    city_code[i] = all_code[i]
print(city_code)

city_value = {}
for city in city_code.keys():
    cmd = "spider_51job.py 嵌入式 -c " + city
    args = convert_str2args(cmd)
    url = get_url(args)
    city_value[city] = get_number_of_pages(url)
    print("{} - {}".format(city, city_value[city]))

print(city_value)
citys = list(city_value.keys())
values = list(city_value.values())

map = Map(init_opts=opts.InitOpts(width="800px", height="600px"))
map.add(
    "嵌入式城市热力图",
    [list(z) for z in zip(citys, values)],
    "china-cities",
    is_map_symbol_show=False,  # 不显示小红点
    label_opts=opts.LabelOpts(is_show=False)  # 不显示城市名称Label
)
map.set_global_opts(
    title_opts=opts.TitleOpts(title="中国-城市地图"),
    visualmap_opts=opts.VisualMapOpts(
        min_=0, max_=70,
        range_color=["#EEEEEE", "#9ACD32", "#FF0000","#000000"]
    )
)
map.render("嵌入式城市热力图.html")
```

<br/>

### 不同工作经验的工资情况？

思路：抓取“嵌入式 软件 -c 南京”的数据。读取csv数据，根据不同的工作年限算出平均工资，绘制柱形图。

结果：通过关键词获取到1500条职位信息，其中1189条包含工作经验的工作信息。计算并绘制出下图：

![average_or_wy.png](/resources/51job_data_analysis/average_or_wy.png)

[代码](/resources/51job_data_analysis/average_wage_of_work_year_by_file.py)如下（以上结果为median取值）：

```python
# coding:utf-8

import csv
import ast
import numpy as np
import matplotlib.pyplot as plt

# 一般的薪资都是一个范围，支持使用3种算法
# min       取最小值
# median    取中间值
# max       取最大值
calculation_method = "median"
file_name = "51job_嵌入式-软件_2020-11-09_22-32-12.csv"


work_exp = {}
with open(file_name, encoding='utf-8', errors='ignore') as f:
    reader = csv.reader(f)
    for row in reader:
        if len(row) < 5:
            continue

        salary = 0
        if '千' in row[2]:
            num_str = row[2].split('千')[0]
            multiplier = 1000
        elif '万' in row[2]:
            num_str = row[2].split('万')[0]
            multiplier = 10000
        else:
            continue
        salary_range = num_str.split('-')
        if len(salary_range) == 1:
            salary = float(salary_range[0]) * multiplier
        elif len(salary_range) == 2:
            min_s,max_s = salary_range
            if calculation_method == "median":
                salary = (float(min_s) + float(max_s)) / 2 * multiplier
            elif calculation_method == "min":
                salary = float(min_s) * multiplier
            else:
                salary = float(max_s) * multiplier
        else:
            continue
        if '年' in row[2]:
            salary /= 14  # 默认按照14薪计算

        keyword = ""
        requirements = ast.literal_eval(row[3])
        for requirement in requirements:
            if "经验" in requirement:
                keyword = requirement

        if keyword:
            if keyword in work_exp.keys():
                work_exp[keyword][0] += salary
                work_exp[keyword][1] += 1
            else:
                work_exp[keyword] = [salary,1]

for i in work_exp.keys():
     work_exp[i].append(int(work_exp[i][0]/work_exp[i][1]))

keys = ['无需经验', '1年经验', '2年经验', '3-4年经验', '5-7年经验', '8-9年经验', '10年以上经验']

keys = [k for k in keys if k in work_exp.keys()]
numbers = [work_exp[k][1] for k in keys]
averages = [work_exp[k][2] for k in keys]

plt.figure()

ax1 = plt.subplot(211)
ax1.set_title('工作数量')
plt.bar(keys, numbers)
for i,n in zip(range(len(keys)),numbers):
    plt.text(i,n,"%d"%n,ha='center',va='bottom')

ax2 = plt.subplot(212)
ax2.set_title('平均薪资')
plt.bar(keys, averages)
for i,n in zip(range(len(keys)),averages):
    plt.text(i,n,"%d"%n,ha='center',va='bottom')

plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.show()
```

<br/>

### 不同学历的工资情况？

思路：根据不同的学历，从低到高，依次计算上面的平均工资，绘制柱形图。

结果：

对于学历要求的职位数量统计：

![education_numbers.png](/resources/51job_data_analysis/education_numbers.png)

不同学历在不同工作经验的薪资水平，图示如下：

![average_of_education.png](/resources/51job_data_analysis/average_of_education.png)

从上可以看出，学历对于嵌入式从业的影响是十分正相关的，中专、大专、本科、硕士一级一级工资的增长肉眼可见。博士就比较魔幻了，可能是已经到另一个境界了吧。

[代码](/resources/51job_data_analysis/average_of_education.py)如下：

```python
# coding:utf-8

import sys
import csv
import ast
import numpy as np
import matplotlib.pyplot as plt

# 一般的薪资都是一个范围，支持使用3种算法
# min       取最小值
# median    取中间值
# max       取最大值
calculation_method = "median"
file_name = "51job_嵌入式-软件_2020-11-09_22-32-12.csv"

educations = ['中专','大专','本科','硕士','博士']

plt.figure()
index = 0
for edu in educations:
    work_years = {}
    with open(file_name, encoding='utf-8', errors='ignore') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) < 5:
                continue

            if edu not in row[3]:
                continue

            salary = 0
            if '千' in row[2]:
                num_str = row[2].split('千')[0]
                multiplier = 1000
            elif '万' in row[2]:
                num_str = row[2].split('万')[0]
                multiplier = 10000
            else:
                continue
            salary_range = num_str.split('-')
            if len(salary_range) == 1:
                salary = float(salary_range[0]) * multiplier
            elif len(salary_range) == 2:
                min_s,max_s = salary_range
                if calculation_method == "median":
                    salary = (float(min_s) + float(max_s)) / 2 * multiplier
                elif calculation_method == "min":
                    salary = float(min_s) * multiplier
                else:
                    salary = float(max_s) * multiplier
            else:
                continue
            if '年' in row[2]:
                salary /= 14  # 默认按照14薪计算

            keyword = ""
            requirements = ast.literal_eval(row[3])
            for requirement in requirements:
                if "经验" in requirement:
                    keyword = requirement

            if keyword:
                if keyword in work_years.keys():
                    work_years[keyword][0] += salary
                    work_years[keyword][1] += 1
                else:
                    work_years[keyword] = [salary,1]

    for i in work_years.keys():
         work_years[i].append(int(work_years[i][0]/work_years[i][1]))

    keys = ['无需经验', '1年经验', '2年经验', '3-4年经验', '5-7年经验', '8-9年经验', '10年以上经验']

    keys = [k for k in keys if k in work_years.keys()]
    averages = [work_years[k][2] for k in keys]

    index += 1
    subplot_index = 510 + index
    ax = plt.subplot(subplot_index)
    ax.set_title(edu)
    plt.bar(keys, averages)
    for i,n in zip(range(len(keys)),averages):
        plt.text(i,n,"%d"%n,ha='center',va='top',color="#ffffff")

plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.show()
```

<br/>

### 参考

* [Map_china_citites | pyecharts](https://gallery.pyecharts.org/#/Map/map_china_cities)
