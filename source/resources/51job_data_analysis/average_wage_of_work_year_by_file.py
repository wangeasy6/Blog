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


work_years = {}
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

        # if "本科" not in row[3]:
        #     continue

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
numbers = [work_years[k][1] for k in keys]
averages = [work_years[k][2] for k in keys]

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