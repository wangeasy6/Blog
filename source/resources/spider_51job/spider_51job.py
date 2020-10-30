#!/usr/bin/env python3
# -*- coding:utf-8 -*-

'''
  功能： 根据指定参数，抓取51Job的数据
  日期： 2020-10-28
'''

import re
import sys
import csv
import time
import json
import urllib
import argparse
import requests
from bs4 import BeautifulSoup

__version__ = '0.8.0'
__max_line = None
__store_file = None


def _ParseArguments(argv):
    '''解析命令行输入，使用argparse

    参数:
        argv: 命令行参数，例如 sys.argv

    返回:
        argparse对象
    '''

    parser = argparse.ArgumentParser(description='Configurable "51job.com" spider script.')

    parser.add_argument(
        'keyword',
        help='搜索关键字')

    parser.add_argument(
        '-v',
        '--version',
        action='store_true',
        help='show version number and exit')

    parser.add_argument(
        '-u',
        '--url',
        help='根据搜索页的URL直接抓取数据。当指定此参数时，仅参数max_line、save_format生效，keyword仅用作保存文件的命名')

    parser.add_argument(
        '-c',
        '--cities',
        action='append',
        help='工作地点，最多可添加5个')

    parser.add_argument(
        '-smin',
        '--salary_min',
        type=int,
        help='最低薪资')

    parser.add_argument(
        '-smax',
        '--salary_max',
        type=int,
        help='最高薪资')

    parser.add_argument(
        '-wy',
        '--workyears',
        type=int,
        choices=range(1,7),
        action='append',
        help='工作年限：(1-6)分别表示("在校生/应届生", "1-3年", "3-5年", "5-10年", "10年以上", "无需经验")')

    parser.add_argument(
        '-df',
        '--degree_from',
        type=int,
        choices=range(1,8),
        action='append',
        help='学历要求：(1-7)分别表示("初中及以下", "高中/中技/中专", "大专", "本科", "硕士", "博士", "无学历要求")')

    parser.add_argument(
        '-cn',
        '--company_nature',
        type=int,
        choices=range(1,12),
        action='append',
        help='公司性质：(01-11)分别表示("外资（欧美）", "外资（非欧美）", "合资", "国企", "民营企业", "外企代表处", "政府机关", "事业单位", "非营利组织", "上市公司", "创业公司")')

    parser.add_argument(
        '-cs',
        '--company_size',
        type=int,
        choices=range(1,8),
        action='append',
        help='公司规模：(1-7)分别表示("少于50人", "50-150人", "150-500人", "500-1000人", "1000-5000人", "5000-10000人", "10000人以上")')

    parser.add_argument(
        '-ml',
        '--max_line',
        type=int,
        help='设置抓取数据条目的最大数')

    parser.add_argument(
        '-sf',
        '--save_format',
        choices=['csv'],
        default='csv',
        help='数据保存格式，当前仅支持csv')

    if len(argv) < 2:
        parser.print_help()
        sys.exit(-1)

    return parser.parse_args(argv[1:])


def main(argv):
    '''主程序

    参数:
        argv: 命令行参数
    '''

    global __store_file
    global __max_line

    args = _ParseArguments(argv)
    if args.version:
        print('51job_spider {}'.format(__version__))
        return 0

    __store_file = "51job_" + args.keyword + \
        time.strftime('_%Y-%m-%d_%H-%M-%S') + ".csv"
    print(__store_file)

    if args.max_line:
        __max_line = args.max_line

    url = get_url(args)
    print("搜索URL：", url)

    number_of_pages = get_number_of_pages(url.format(1))
    print("一共{}页".format(number_of_pages))
    sub_urls = get_urls(url, number_of_pages)
    get_info(sub_urls, __store_file)


def get_url(args):
    '''根据解析的命令行参数，拼接URL

    参数:
        args: 命令行参数对象

    返回:
        str: URL（其中页数用{}代替）
    '''

    if args.url:
        url = args.url
        url = args.url.replace("1.html","{}.html")
        end = url.find(".html")
        start = url[:end].rfind(",")

        if end == -1 or start == -1:
            print("URL parameter parsing failed.")
            sys.exit(-1)

        new_url = url[:start+1] + "{}"+ url[end:]
        return new_url

    # 城市
    cities = ""
    if args.cities:
        if len(args.cities) > 5:
            print(args.cities)
            print("You can only select 5 cities at most. exit")
            return -1
        city_code = get_city_code()
        for city in args.cities:
            if city in city_code.keys():
                cities += city_code[city] + '%252c'

    if not cities:
        cities = "000000"
    elif cities[-5:] == '%252c':
        cities = cities[:-5]

    # 薪资范围
    if args.salary_min and args.salary_max \
        and args.salary_min > args.salary_max:
        print("Salary range start greater than range end. exit")
        return -1

    salary_range = (str(args.salary_min) if args.salary_min else "") \
         + "-" + \
         (str(args.salary_max) if args.salary_max else "")
    if salary_range == "-":
        salary_range = "99"

    # 工作年限
    workyears = ""
    if args.workyears:
        for workyear in args.workyears:
            workyears += '0' + str(workyear) + '%252c'
        if workyears[-5:] == '%252c':
            workyears = workyears[:-5]

    # 学历要求
    degree_from = ""
    if args.degree_from:
        for i in args.degree_from:
            degree_from += '0' + str(i) + '%252c'
        if degree_from[-5:] == '%252c':
            degree_from = degree_from[:-5]

    # 公司性质
    company_nature = ""
    if args.company_nature:
        for i in args.company_nature:
            company_nature += "%02d" % i + '%252c'
        if company_nature[-5:] == '%252c':
            company_nature = company_nature[:-5]

    # 公司规模
    company_size = ""
    if args.company_size:
        for i in args.company_size:
            company_size += '0' + str(i) + '%252c'
        if company_size[-5:] == '%252c':
            company_size = company_size[:-5]

    url = "https://search.51job.com/list/" + cities \
        + ",000000,0000,00,9," + salary_range + "," \
        + urllib.parse.quote(urllib.parse.quote(args.keyword)) + ",2,{}.html"

    if workyears or degree_from or \
        company_nature or company_size:
        url += "?"
    if workyears:
        url += "workyear=" + workyears + "&"
    if degree_from:
        url += "degreefrom=" + degree_from + "&"
    if company_nature:
        url += "cotype=" + company_nature + "&"
    if company_size:
        url += "companysize=" + company_size + "&"

    return url


def get_number_of_pages(url):
    '''获取搜索内容的总页数

    参数:
        url: 搜索列表页的URL

    返回:
        int: 页数
    '''

    response = urllib.request.urlopen( url )
    response_text = response.read().decode("gbk")
    response.close()

    soup = BeautifulSoup(response_text, "html.parser")
    scripts = soup.findAll("script")
    for script in scripts:
        if "__SEARCH_RESULT__ " in str(script):
            script_str = str(script)[60:-9]
            script_node = json.loads(script_str)

            if "total_page" in script_node.keys():
                return int(script_node["total_page"])

    return 0


def get_urls(url, page):
    '''获取搜索内容的每个职位的链接列表

    参数:
        url: 爬取页面第一页的URL，页数用{}代替
        page: 爬取页数

    返回:
        list: 每个职位详细信息的链接列表
    '''

    sub_urls = []  # 存放子链接列表
    count = 1  # 数据计数

    # 获取子链接
    for i in range(1, page+1):
        print("第{}页".format(i))
        response = urllib.request.urlopen( url.format(i) )
        response_text = response.read().decode("gbk")
        response.close()

        soup = BeautifulSoup(response_text, "html.parser")     # 用html.parser解析 将返回的文本转BeautifulSoup对象
        pattern = re.compile(r"var a = '(.*?)';$", re.MULTILINE | re.DOTALL)

        script = soup.findAll("script")
        for result in script:
            result_str = str(result)
            ret = result_str.find("__SEARCH_RESULT__")
            if ret != -1:
                result_str = result_str[60:-9]
                ret_node = json.loads(result_str)
                for node in ret_node['engine_search_result']:
                    sub_urls.append(node["job_href"])
                    print("已获取{}个子网页链接".format(count))
                    if __max_line and count >= __max_line:
                        return sub_urls
                    count += 1

    return sub_urls  # 返回子链接列表


def get_info(sub_urls, filename):
    '''获取职位页面的详细信息，并保存

    参数:
        sub_urls: 职位页面链接列表
        filename: 存放信息的文件名称

    返回:
        无返回，获取的信息保存到文件中
    '''

    count = 1
    header = {
        'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36",
    }

    # 写入要获取的信息
    with open(__store_file, 'w', newline='', encoding='utf-8') as f:
        csv_write = csv.writer(f)
        csv_write.writerow(['公司', '职位名称', '工资', '地点及要求', '福利', '职位信息', 'URL'])

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
            all = [company, position, salary, req, welare, pos, url]

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
        if __max_line and count >= __max_line:
            break
        count += 1


def get_city_code():
    '''获取每个城市和城市码对应的字典

    返回:
        dict: {"城市名":"city_code"}
    '''
    search_url = 'https://search.51job.com/list/000000,000000,0000,00,9,99,%2B,2,1.html'

    response = urllib.request.urlopen( search_url )
    search_url_text = response.read().decode("gbk")
    response.close()
    search_html = BeautifulSoup(search_url_text, "html.parser")
    scripts = search_html.findAll("script")

    js_url = ""
    for script in scripts:
        if str(script).find("common") != -1:
            js_url = script['src']

    if js_url != "":
        rstr = requests.get("https:" + js_url).text
        start = rstr.find('window.area') + 12
        end = rstr.find('}',start)
        rrstr = rstr[start:end+1]
        code_city = eval(rrstr)
        # print(city_dict)
        # print(len(city_dict))
        city_code = dict(zip(code_city.values(), code_city.keys()))
        return city_code


if __name__ == '__main__':
    main(sys.argv)