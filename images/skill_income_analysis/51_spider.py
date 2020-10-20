#!/usr/bin/env python3
# -*- coding:utf-8 -*-
'''
  @功能 根据关键词、城市抓取51Job的数据
  @date 2020-10-19
'''

import re
import sys
import csv
import urllib
import requests
from bs4 import BeautifulSoup

'''
:param url 爬取页面第一页，页数用{}代替
:param page 爬取页数
:return 返回一个存放每个职位详细信息的链接 列表
'''
def get_urls(url, page):
    sub_urls = []  # 存放子链接列表
    count = 1  # 数据计数
    # 获取子链接
    print(page)
    for i in range(1, page):
        response = urllib.request.urlopen( url.format(i) )
        response_text = response.read().decode("gbk")
        response.close()

        soup = BeautifulSoup(response_text, "html.parser")     # 用html.parser解析 将返回的文本转BeautifulSoup对象
        pattern = re.compile(r"var a = '(.*?)';$", re.MULTILINE | re.DOTALL)

        script = soup.findAll("script")
        # print(script)
        for result in script:
            if "text/javascript" in result:
                print(result)
        sys.exit(0)
        '''
        p_info = soup.find_all('p', class_="t1")    # 获取到p标签的信息
        for info in p_info:
            sub_urls.append(info.span.a['href'])  # 获取a标签的 href链接 添加到sub_url
            print("已获取{}个字网页链接".format(count))
            count += 1
        '''
    return sub_urls  # 返回子链接列表


'''
:param sub_urls 子页面链接
:param filename 存放信息的csv文件名称
:return 无返回，会在当前目录创建该文件，获取的信息会存放在该文件中
'''
def get_info(sub_urls, filename):
    # 数据计数
    count = 1

    # 写入要获取的信息
    with open(store_file, 'w', newline='', encoding='utf-8') as f:
        csv_write = csv.writer(f)
        csv_write.writerow(['公司', '职位名称', '工资', '地点及要求', '福利', '职位信息'])

    # 访问子链接获取需要的信息
    for url in sub_urls:
        # 获取文本
        try:
            info_response = requests.get(url, headers=header)
            info_response.encoding = 'gbk'
            info_soup = BeautifulSoup(info_response.text, "html.parser")

            # 职位、薪资、公司信息抓取
            infomation_less = info_soup.find('div', class_='cn')  # 简单信息
            position = infomation_less.h1.text      # 职位
            salary = infomation_less.strong.text    # 薪资
            company = infomation_less.p.a.text      # 公司

            # 招工信息抓取 地点、工作经验、学历要求、招工人数、发布日期等
            info_employ = info_soup.find('p', class_='msg').text
            req = info_employ.split("\xa0\xa0|\xa0\xa0")  # 字符串分割

            # 福利信息抓取
            try:
                info_welare = info_soup.find('div', class_='t1')
                welare_all = info_welare.findAll('span', class_='sp4')
                welare = []  # 建立存储福利列表
                for i in welare_all:
                    welare.append(i.text)  # 将所有span标签中的福利信息存入
                # print(welare)
            except Exception as e:  # 异常抛出
                print('ware', end=" ")
                print(e)
                welare = "None"

            # 职位信息抓取
            try:
                info_position = info_soup.find("div", class_="bmsg")
                position_all = info_position.find_all("p")
                pos = ""
                positioninfo = []   # 建立存储职位信息列表
                for i in position_all:
                    positioninfo.append(i.text)
                pos = pos.join(positioninfo)    # 列表信息合并为一个字符串
            except Exception as e:  # 异常抛出
                print('pos', end=" ")
                print(e, end=" ")
                pos = "None"

            # 信息整合
            all = [company, position, salary, req, welare, pos]

            # 信息写入
            with open(filename, 'a', encoding='utf-8') as file:
                write = csv.writer(file)
                write.writerow(all)
        except Exception as e:  # 如果有异常，打印  并且存放None
            print(e, end=" ")
            with open(filename, 'a', encoding='utf-8') as file:
                write = csv.writer(file)
                write.writerow(['None'])
        print("已爬取{}条数据".format(count))
        count += 1


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("[Error]Usage: Python3 51_spider.py <keyword> <city>")
        sys.exit(0)

    store_file = "51_Job_" + sys.argv[1] + "_" + sys.argv[2] + ".csv"
    print(store_file)

    url = "https://search.51job.com/list/070200,000000,0000,00,9,99," \
        + urllib.parse.quote(sys.argv[1]) + ",2,{}.html"
    print( url )

    header = {
        'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36",
    }

    sub_urls = get_urls(url, 10)
    print(sub_urls)
    get_info(sub_urls, store_file)

'''
https://search.51job.com/list/070200,000000,0000,00,9,99,%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F%25E8%25BD%25AF%25E4%25BB%25B6%25E5%25B7%25A5%25E7%25A8%258B%25E5%25B8%2588,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=

%E5%B5%8C%E5%85%A5%E5%BC%8F%E8%BD%AF%E4%BB%B6%E5%B7%A5%E7%A8%8B%E5%B8%88
'''