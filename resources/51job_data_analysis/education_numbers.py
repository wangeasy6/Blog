# coding:utf-8

import sys
import csv
import ast
import numpy as np
import matplotlib.pyplot as plt

file_name = "51job_嵌入式-软件_2020-11-09_22-32-12.csv"

educations = {'中专':0, '大专':0, '本科':0, '硕士':0,'博士':0}

with open(file_name, encoding='utf-8', errors='ignore') as f:
    reader = csv.reader(f)
    for row in reader:
        if len(row) < 5:
            continue

        for edu in educations:
            if edu in row[3]:
                educations[edu] += 1
                break

print(educations)

plt.figure()
plt.bar(educations.keys(), educations.values())
for i,n in zip(range(len(educations)),educations.values()):
    plt.text(i,n,"%d"%n,ha='center',va='bottom')
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.show()