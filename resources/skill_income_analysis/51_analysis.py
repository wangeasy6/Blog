#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import csv
import ast

with open("51_Job_嵌入式软件_南京_2020-10-21_10-37-39.csv", encoding='utf-8', errors='ignore') as f:
    reader = csv.reader(f)
    rows=[row for row in  reader]
    # print(len(rows))
    for i in range(1, 20):
        if rows[i]:
            # print(rows[i][:-1])
            print(rows[i][2])
            brief_info = ast.literal_eval(rows[i][3])
            print( brief_info )
            welfare_info = ast.literal_eval(rows[i][4])
            print( welfare_info)