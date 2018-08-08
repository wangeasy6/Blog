---
title: Django使用笔记
date: 2018-08-07 15:23:10
categories:
  - tech
---

### 安装
`sudo pip install Django`

* 指定版本安装
`sudo pip install Django==1.6.8`

* 查看版本：
```
>>> import django
>>> django.VERSION
```
1.7、1.8、1.9各个版本的使用差别还是很大的。

<!--more-->

### 创建
* 创建项目
`django-admin.py startproject "project-name"`

* 创建应用
```
cd "project-name"
python manage.py startapp "app-name"
```

以1.8版本为例，整个文件结构大致如下：
```
Test
├── manage.py
├── Test
│   ├── __init__.py
│   ├── __pycache__
│   │   ├── __init__.cpython-35.pyc
│   │   └── settings.cpython-35.pyc
│   ├── settings.py
│   ├── urls.py
│   └── wsgi.py
└── MyApp
    ├── admin.py
    ├── apps.py
    ├── __init__.py
    ├── migrations
    │   └── __init__.py
    ├── models.py
    ├── tests.py
    └── views.py
```

大的Test就是整个项目，MyApp就是一个项目(网站)，小的Test就是项目的配置文件，包括项目的基本配置(setting.py)，以及url映射文件(urls.py)。

### HelloWord
