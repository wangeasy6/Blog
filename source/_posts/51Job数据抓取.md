---
title: 51Job数据抓取
date: 2020-10-30 19:00:00
toc: true
categories:
  - Technology
tags:
  - Python
  - Data
---

![51job.jpg](/images/spider_51job/51job.jpg)

最近想要做一个“嵌入式技能回报率”的分析，本来想使用网上现成的spider，但好像没有满足需要的，只能自己写了。

本51job数据抓取程序包含以下功能：

  * 根据传入参数进行搜索、抓取数据
  * 根据关键词搜索
  * 根据地理位置（城市）搜索
  * 根据月薪范围、工作年限、学历要求、公司性质、公司规模搜索
  * 以CSV格式保存
  * 也可以作为库被调用

<!--more-->

<br/>

### 代码

* [spider_51job.py](/images/spider_51job/spider_51job.py)

<br/>

### 使用说明

```shell
usage: spider_51job.py [-h] [-v] [-u URL] [-c CITIES] [-smin SALARY_MIN]
                       [-smax SALARY_MAX] [-wy {1,2,3,4,5,6}] [-df {1,2,3,4,5,6,7}]
                       [-cn {1,2,3,4,5,6,7,8,9,10,11}] [-cs {1,2,3,4,5,6,7}]
                       [-ml MAX_LINE] [-sf {csv}]
                       keyword

"51job.com" spider script.

positional arguments:
  keyword               搜索关键字

optional arguments:
  -h, --help            show this help message and exit
  -v, --version         show version number and exit
  -u URL, --url URL     根据搜索页的URL直接抓取数据。当指定此参数时，仅参数max_line、save_format生效，keyword仅用作保存
                        文件的命名
  -c CITIES, --cities CITIES
                        工作地点，最多可添加5个
  -smin SALARY_MIN, --salary_min SALARY_MIN
                        最低薪资
  -smax SALARY_MAX, --salary_max SALARY_MAX
                        最高薪资
  -wy {1,2,3,4,5,6}, --workyears {1,2,3,4,5,6}
                        工作年限：(1-6)分别表示("在校生/应届生", "1-3年", "3-5年", "5-10年", "10年以上",
                        "无需经验")
  -df {1,2,3,4,5,6,7}, --degree_from {1,2,3,4,5,6,7}
                        学历要求：(1-7)分别表示("初中及以下", "高中/中技/中专", "大专", "本科", "硕士", "博士",
                        "无学历要求")
  -cn {1,2,3,4,5,6,7,8,9,10,11}, --company_nature {1,2,3,4,5,6,7,8,9,10,11}
                        公司性质：(01-11)分别表示("外资（欧美）", "外资（非欧美）", "合资", "国企", "民营企业",
                        "外企代表处", "政府机关", "事业单位", "非营利组织", "上市公司", "创业公司")
  -cs {1,2,3,4,5,6,7}, --company_size {1,2,3,4,5,6,7}
                        公司规模：(1-7)分别表示("少于50人", "50-150人", "150-500人", "500-1000人",
                        "1000-5000人", "5000-10000人", "10000人以上")
  -ml MAX_LINE, --max_line MAX_LINE
                        设置抓取数据条目的最大数
  -sf {csv}, --save_format {csv}
                        数据保存格式，当前仅支持csv
```

<br/>

### 代码说明

#### URL关键字参数

带有1个关键字参数（嵌入式）的URL：

[https://search.51job.com/list/000000,000000,0000,00,9,99,%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,99,%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

带有3个关键字参数（嵌入式 软件 STM32）的URL：

[https://search.51job.com/list/000000,000000,0000,00,9,99,%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F%2520%25E8%25BD%25AF%25E4%25BB%25B6%2520STM32,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,99,%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F%2520%25E8%25BD%25AF%25E4%25BB%25B6%2520STM32,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

可以看出，前面的`https://search.51job.com/list/000000,000000,0000,00,9,99,
`和后面的`,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=`是相同的，提取出不同的是：

`%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F`

`%25E5%25B5%258C%25E5%2585%25A5%25E5%25BC%258F%2520%25E8%25BD%25AF%25E4%25BB%25B6%2520STM32`

在这里，中文被转成了url编码。以第2条为例，使用[UrlEncode编码/UrlDecode解码 - 站长工具](http://tool.chinaz.com/tools/urlencode.aspx)解码，第一次解码之后得`%E5%B5%8C%E5%85%A5%E5%BC%8F%20%E8%BD%AF%E4%BB%B6%20STM32`，再解码之后得`嵌入式 软件 STM32`。

所以，在Python中把关键词参数加入URL时，可以使用以下方法：

```
url = "https://search.51job.com/list/000000,000000,0000,00,9,99," \
    + urllib.parse.quote(urllib.parse.quote(args.keywords)) + ",2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare="
```

<br/>

#### URL城市参数

URL（keywords="Linux" \-\-city="全国"）:

[https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

URL（keywords="Linux" \-\-city="南京"）:

[https://search.51job.com/list/070200,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/070200,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

URL（keywords="Linux" \-\-city="北京"）:

[https://search.51job.com/list/010000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/010000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

当我们打开\-\-city="北京"的这个页面的时候，我们可以看到，在这上面还可以指定到某个区：

![city_area.jpg](/images/spider_51job/city_area.jpg)

当选择“上城区”的时候，URL变成了：

[https://search.51job.com/list/010000,010100,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/010000,010100,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

当同时选择南京、北京（最多选择5项）时，URL变成了：

[https://search.51job.com/list/010000%252c070200,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/010000%252c070200,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

可以看到，在上面的这些URL里仅仅改变了2个地方，`list/`之后的两个字符串，第1个代表城市，而第2个地方表示区号，多个城市之间用`%252c`分隔（选择多个城市时不可选择区号，至多选择5个城市）。51job对每个城市、分区都编了号。

肯定有一个存储这些城市编码的地方，最后找到了这个文件：

![window_area.png](/images/spider_51job/window_area.png)

而在代码中，`get_city_code()`函数就是用来获取这个列表的。

<br/>

#### URL薪酬范围参数

URL（keywords="Linux" -srs=3000）：

[https://search.51job.com/list/000000,000000,0000,00,9,3000-,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,3000-,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

URL（keywords="Linux" -sre=10000）：

[https://search.51job.com/list/000000,000000,0000,00,9,-10000,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,-10000,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

URL（keywords="Linux" -srs=3000 -sre=10000）：

[https://search.51job.com/list/000000,000000,0000,00,9,3000-10000,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,3000-10000,Linux,2,1.html?lang=c&postchannel=0000&workyear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

从上可以看出，薪资范围在关键词前面一个字段，最低值和最高值之间用'-'分隔，忽略时为99，当范围起始值大于范围结束值时也会自动忽略。

<br/>

#### URL工作年限参数

51job支持使用工作年限来搜索：

![workyear.jpg](/images/spider_51job/workyear.jpg)

URL（keywords="Linux" 工作年限="在校生/应届生"）：

[https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=01&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=01&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

URL（keywords="Linux" 工作年限="1-3年"&"5-10年"）：

[https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=02%252c04&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=](https://search.51job.com/list/000000,000000,0000,00,9,99,Linux,2,1.html?lang=c&postchannel=0000&workyear=02%252c04&cotype=99&degreefrom=99&jobterm=99&companysize=99&ord_field=0&dibiaoid=0&line=&welfare=)

在URL中使用了参数workyear来指定工作年限，有6个可选项（"在校生/应届生", "1-3年", "3-5年", "5-10年", "10年以上", "无需经验")，分别用(01, 02, 03, 04, 05, 06)来表示。多选用`%252c`分隔。'=99'时忽略此参数。

<br/>

#### URL学历要求参数

同工作年限一样，学历要求使用参数degree_from添加在URL的参数列表里，(01-07)分别表示以下意思：

![degree_from.png](/images/spider_51job/degree_from.png)

<br/>

#### URL公司性质参数

同工作年限一样，公司性质使用参数cotype添加在URL的参数列表里，(01-11)分别表示("外资（欧美）", "外资（非欧美）", "合资", "国企", "民营企业", "外企代表处", "政府机关", "事业单位", "非营利组织", "上市公司", "创业公司")：

<br/>

#### URL公司规模参数

同工作年限一样，公司性质使用参数companysize添加在URL的参数列表里，(01-07)分别表示以下意思：

![company_size.png](/images/spider_51job/company_size.png)

<br/>

#### 作为库使用

作为库被其它程序引用，传入argv参数，来抓取数据。

```python
#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from spider_51job import main

cmd = 'spider_51job.py url -ml 10 -u http://search.51job.com/list/070200,000000,0000,00,9,99,Linux,2,1.html'
main(cmd.split(" "))
```

<br/>

### 参考

* [Argparse 教程](https://docs.python.org/zh-cn/3/howto/argparse.html)