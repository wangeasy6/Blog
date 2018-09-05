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
`python -m django --version`



<!--more-->

### 创建项目
* 创建项目
  `django-admin.py startproject "project-name"`
这行代码将会在当前目录下创建一个 `mysite` 目录。如果命令失败了，查看 [运行``django-admin``时遇到的问题](https://docs.djangoproject.com/zh-hans/2.1/faq/troubleshooting/#troubleshooting-django-admin)，可能能给你提供帮助。

让我们看看 [`startproject`](https://docs.djangoproject.com/zh-hans/2.1/ref/django-admin/#django-admin-startproject) 创建了些什么:

```
mysite/
    manage.py
    mysite/
        __init__.py
        settings.py
        urls.py
        wsgi.py
```

这些目录和文件的用处是：

- 最外层的:file: mysite/ 根目录只是你项目的容器， Django 不关心它的名字，你可以将它重命名为任何你喜欢的名字。
- `manage.py`: 一个让你用各种方式管理 Django 项目的命令行工具。你可以阅读 [django-admin and manage.py](https://docs.djangoproject.com/zh-hans/2.1/ref/django-admin/) 获取所有 `manage.py` 的细节。
- 里面一层的 `mysite/`  目录包含你的项目，它是一个纯 Python 包。它的名字就是当你引用它内部任何东西时需要用到的 Python 包名。 (比如 `mysite.urls`).
- `mysite/__init__.py`：一个 Python 包标识空文件。如果你是 Python 初学者，阅读官方文档中的 [更多关于包的知识](https://docs.python.org/3/tutorial/modules.html#tut-packages)。
- `mysite/settings.py`：Django 项目的配置文件。如果你想知道这个文件是如何工作的，请查看 [Django settings](https://docs.djangoproject.com/zh-hans/2.1/topics/settings/) 了解细节。
- `mysite/urls.py`：Django 项目的 URL 声明，就像你网站的“目录”。阅读 [URL调度器](https://docs.djangoproject.com/zh-hans/2.1/topics/http/urls/) 文档来获取更多关于 URL 的内容。
- `mysite/wsgi.py`：作为你的项目的运行在 WSGI 兼容的Web服务器上的入口。阅读 [如何使用 WSGI 进行部署](https://docs.djangoproject.com/zh-hans/2.1/howto/deployment/wsgi/) 了解更多细节。

确认一下你的 Django 项目是否真的创建成功了 ：

`$ python manage.py runserver`

更换监听端口：

`$ python manage.py runserver 8080`

监听所有服务器的公开IP :

`$ python manage.py runserver 0:8000`



你应该会看到如下输出：

```
Performing system checks...

System check identified no issues (0 silenced).

You have unapplied migrations; your app may not work properly until they are applied.
Run 'python manage.py migrate' to apply them.

八月 15, 2018 - 15:50:53
Django version 2.1, using settings 'mysite.settings'
Starting development server at http://127.0.0.1:8000/
Quit the server with CONTROL-C.
```

现在，服务器正在运行，浏览器访问 <https://127.0.0.1:8000/> 。你将会看到一个“祝贺”页面，随着一只火箭发射，服务器已经运行了。

### 创建应用

在 Django 中，每一个应用都是一个 Python 包，并且遵循着相同的约定。Django 自带一个工具，可以帮你生成应用的基础目录结构。

```
cd "project-name"
python manage.py startapp "app-name"
```

以1.8版本为例，整个文件结构如下：
```
 MyApp/
    ├──__init__.py
    ├── admin.py
    ├── apps.py
    ├── migrations
    │   └── __init__.py
    ├── models.py
    ├── tests.py
    └── views.py
```

### 编写第一个视图

让我们开始编写第一个视图吧。打开 `polls/views.py`，把下面这些 Python 代码输入进去：

polls/views.py

```
from django.http import HttpResponse


def index(request):
    return HttpResponse("Hello, world. You're at the First View.")
```

这是 Django 中最简单的视图。如果想看见效果，我们需要将一个 URL 映射到它。

在 MyApp 目录里新建一个 `urls.py`  文件。你的应用目录现在看起来应该是这样：

```
MyApp/
    __init__.py
    admin.py
    apps.py
    migrations/
        __init__.py
    models.py
    tests.py
    urls.py
    views.py
```

在 `MyApp/urls.py` 中，输入如下代码：

```
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
]
```

下一步是要在根 URLconf 文件中指定我们创建的 `polls.urls` 模块。在 `mysite/urls.py` 文件的 `urlpatterns` 列表里插入一个 `include()`， 如下：

mysite/urls.py

```
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('MyApp/', include('polls.urls')),
    path('admin/', admin.site.urls),
]
```

可以验证是否正常工作，运行下面的命令:

```
$ python manage.py runserver
```

用你的浏览器访问 <http://localhost:8000/MyApp/>，你应该能够看见 "*Hello, world. You're at the polls index.*" ，这是你在 `index` 视图中定义的。


#### 函数 [include()](https://docs.djangoproject.com/zh-hans/2.1/ref/urls/#django.urls.include)

* 允许引用其它 URLconfs。每当 Django 遇到 :func：django.urls.include 时，它会截断与此项匹配的 URL 的部分，并将剩余的字符串发送到 URLconf 以供进一步处理。

* 何时使用 [include()](https://docs.djangoproject.com/zh-hans/2.1/ref/urls/#django.urls.include)，当包括其它 URL 模式时你应该总是使用 `include()` ， `admin.site.urls` 是唯一例外。

#### 函数 [path()](https://docs.djangoproject.com/zh-hans/2.1/ref/urls/#django.urls.path)
path() 具有四个参数，两个必须参数：`route` 和 `view`，两个可选参数：`kwargs` 和 `name`。现在，是时候来研究这些参数的含义了。

* 参数： [route](https://docs.djangoproject.com/zh-hans/2.1/intro/tutorial01/#path-argument-route)

  * `route` 是一个匹配 URL 的准则（类似正则表达式）。当 Django 响应一个请求时，它会从 `urlpatterns` 的第一项开始，按顺序依次匹配列表中的项，直到找到匹配的项。
  * 这些准则不会匹配 GET 和 POST 参数或域名。例如，URLconf 在处理请求 `https://www.example.com/myapp/` 时，它会尝试匹配 `myapp/` 。处理请求 `https://www.example.com/myapp/?page=3` 时，也只会尝试匹配 `myapp/`。

* 参数： [view](https://docs.djangoproject.com/zh-hans/2.1/intro/tutorial01/#path-argument-view)
  * 当 Django 找到了一个匹配的准则，就会调用这个特定的视图函数，并传入一个 [`HttpRequest`](https://docs.djangoproject.com/zh-hans/2.1/ref/request-response/#django.http.HttpRequest) 对象作为第一个参数，被“捕获”的参数以关键字参数的形式传入。

* 参数： [kwargs](https://docs.djangoproject.com/zh-hans/2.1/intro/tutorial01/#path-argument-kwargs)
  * 任意个关键字参数可以作为一个字典传递给目标视图函数。

* 参数： [name](https://docs.djangoproject.com/zh-hans/2.1/intro/tutorial01/#path-argument-name)
  * 为你的 URL 取名能使你在 Django 的任意地方唯一地引用它，尤其是在模板中。这个有用的特性允许你只改一个文件就能全局地修改某个 URL 模式。

### MVC框架

Django 是遵从MVC模式的Web框架，**MVC模式**将Controller、View、Model分离，使得应用分离，简化了后续的修改和扩展。

- 模型（Model） - 程序员编写程序应有的功能（实现算法等等）、数据库专家进行数据管理和数据库设计(可以实现具体的功能)。
- 视图（View） - 界面设计人员进行图形界面设计。
- 控制器（Controller）- 负责转发请求，对请求进行处理。

在Django中，有关数据处理的是Model，Views.py是View，urls.py 是Controller。



### 模板

前面编写了一个简单的视图，返回的是纯文本信息，但是Web框架提供了 HTML、CSS、JS 等技术以表现丰富的界面元素。

但我们如何将这些元素返回给请求者呢，Django 提供了 Templates 来支持便捷的返回界面。