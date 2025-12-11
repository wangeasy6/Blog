---
title: 手写HTTP报文
date: 2021-04-16 21:48:06
toc: true
categories:
  - tech
---

![51job.jpg](/resources/Cover/http_tcp.jpg)

在嵌入式系统中，经常使用TCP来模拟一个简易的HTTP Server，所以手动解析和填充HTTP包是常有的事儿。

在这里记录一下，一个简单的HTTP请求/应答报文是怎么样的。

<!--more-->

<br/>

### 客户端请求消息（Requst)

请求消息由以下4个部分组成：

* 请求行
* 报头
* 分割行
* 请求数据

其语法如下：

```
<method> <request-URL> <version>
<headers>

<body>
```

示例：

```http
POST /get_username.cgi HTTP/1.1
Host: www.wangeasy.com

{"id":"001"}
```

在实际收到的报文中，所有的换行都以`\r\n`表示。

<br/>

### 服务器响应消息（Response）

响应消息由以下4个部分组成：

- 状态行
- 报头
- 分割行
- 响应数据

其语法如下：

```
<version> <status-code> <reason-phrase>
<headers>

<body>
```

示例：

```http
HTTP/1.1 200 OK
Content-Type: application/json; charset=UTF-8

{"result":"Success","username":"Easy Wang"}
```

<br/>

### version

目前的HTTP存在以下几个版本，点击链接可以跳转到各个协议版本的[RFC说明](https://www.ietf.org/rfc/)：

* [HTTP/1.0](https://www.ietf.org/rfc/rfc1945.html)
* [HTTP/1.1](https://www.ietf.org/rfc/rfc2616.html)
* [HTTP/2](https://www.ietf.org/rfc/rfc7540.html)

<br/>

### method

**HTTP/1.0：**

* GET —— GET方法指定URI获取对应资源，GET 请求一般（标准）不包含主体。
* HEAD —— HEAD方法与GET相同，但服务器的返回不能包含任何实体。该方法通常用于测试超文本链接的有效性、可访问性和最近的修改。
* POST —— POST方法用于请求目标服务器接受请求中包含的实体。

**HTTP/1.1：**

* OPTIONS —— OPTIONS方法表示对请求URI标识的请求/响应链上可用的通信选项的请求。
* GET
* HEAD
* POST
* PUT —— PUT方法要求将所附实体存储在提供的请求URI下。
* DELETE —— DELETE方法请求源服务器删除由请求URI标识的资源。
* TRACE ——TRACE方法允许客户端查看在请求链的另一端收到的内容，并将该数据用于测试或诊断信息。TRACE请求不得包含实体。
* CONNECT —— CONNECT方法可以动态切换为隧道的代理使用（例如SSL隧道）。

<br/>

### headers

[HTTP/1.0 Request Header Fields](https://www.ietf.org/rfc/rfc1945.html#section-5.2)：

* [Authorization](https://www.ietf.org/rfc/rfc1945.html#section-10.2)
* [From](https://www.ietf.org/rfc/rfc1945.html#section-10.8)
* [If-Modified-Since](https://www.ietf.org/rfc/rfc1945.html#section-10.9)
* [Referer](https://www.ietf.org/rfc/rfc1945.html#section-ection-10.13)
* [User-Agent](https://www.ietf.org/rfc/rfc1945.html#section-ection-10.15)

[HTTP/1.0 Response Header Fields](https://www.ietf.org/rfc/rfc1945.html#section-6.2):

* [Location](https://www.ietf.org/rfc/rfc1945.html#section-10.11)
* [Server](https://www.ietf.org/rfc/rfc1945.html#section-10.14)
* [WWW-Authenticate](https://www.ietf.org/rfc/rfc1945.html#section-10.16)

[HTTP/1.0 Entity Header Fields](https://www.ietf.org/rfc/rfc1945.html#section-7.1):

* [Allow](https://www.ietf.org/rfc/rfc1945.html#section-10.1)
* [Content-Encoding](https://www.ietf.org/rfc/rfc1945.html#section-10.3)
* [Content-Length](https://www.ietf.org/rfc/rfc1945.html#section-10.4)
* [Content-Type](https://www.ietf.org/rfc/rfc1945.html#section-10.5)
* [Expires](https://www.ietf.org/rfc/rfc1945.html#section-10.7)
* [Last-Modified](https://www.ietf.org/rfc/rfc1945.html#section-10.10)

<br/>

[HTTP/1.1 Request Header Fields](https://www.ietf.org/rfc/rfc2616.html#section-5.3)：

* [Accept](https://www.ietf.org/rfc/rfc2616.html#section-14.1)
* [Accept-Charset](https://www.ietf.org/rfc/rfc2616.html#section-14.2)
* [Accept-Encoding](https://www.ietf.org/rfc/rfc2616.html#section-14.3)
* [Accept-Language](https://www.ietf.org/rfc/rfc2616.html#section-14.4)
* [Authorization](https://www.ietf.org/rfc/rfc2616.html#section-14.8)
* [Expect](https://www.ietf.org/rfc/rfc2616.html#section-14.20)
* [From](https://www.ietf.org/rfc/rfc2616.html#section-14.22)
* [Host](https://www.ietf.org/rfc/rfc2616.html#section-14.23)(必须包含)
* [If-Match](https://www.ietf.org/rfc/rfc2616.html#section-14.24)
* [If-Modified-Since](https://www.ietf.org/rfc/rfc2616.html#section-14.25)
* [If-None-Match](https://www.ietf.org/rfc/rfc2616.html#section-14.26)
* [If-Range](https://www.ietf.org/rfc/rfc2616.html#section-14.27)
* [If-Unmodified-Since](https://www.ietf.org/rfc/rfc2616.html#section-14.28)
* [Max-Forwards](https://www.ietf.org/rfc/rfc2616.html#section-14.31)
* [Proxy-Authorization](https://www.ietf.org/rfc/rfc2616.html#section-14.34)
* [Range](https://www.ietf.org/rfc/rfc2616.html#section-14.35)
* [Referer](https://www.ietf.org/rfc/rfc2616.html#section-14.36)
* [TE](https://www.ietf.org/rfc/rfc2616.html#section-14.39)
* [User-Agent](https://www.ietf.org/rfc/rfc2616.html#section-14.43)

[HTTP/1.1 Response Header Fields](https://www.ietf.org/rfc/rfc2616.html#section-6.2):

* [Accept-Ranges](https://www.ietf.org/rfc/rfc2616.html#section-14.5)
* [Age](https://www.ietf.org/rfc/rfc2616.html#section-14.6)
* [ETag](https://www.ietf.org/rfc/rfc2616.html#section-14.19)
* [Location](https://www.ietf.org/rfc/rfc2616.html#section-14.30)
* [Proxy-Authenticate](https://www.ietf.org/rfc/rfc2616.html#section-14.33)
* [Retry-After](https://www.ietf.org/rfc/rfc2616.html#section-14.37)
* [Server](https://www.ietf.org/rfc/rfc2616.html#section-14.38)
* [Vary](https://www.ietf.org/rfc/rfc2616.html#section-14.44)
* [WWW-Authenticate](https://www.ietf.org/rfc/rfc2616.html#section-14.47)

[HTTP/1.1 Entity Header Fields](https://www.ietf.org/rfc/rfc2616.html#section-7.1):

* [Allow](https://www.ietf.org/rfc/rfc2616.html#section-14.7)
* [Content-Encoding](https://www.ietf.org/rfc/rfc2616.html#section-14.11)
* [Content-Language](https://www.ietf.org/rfc/rfc2616.html#section-14.12)
* [Content-Length](https://www.ietf.org/rfc/rfc2616.html#section-14.13)
* [Content-Location](https://www.ietf.org/rfc/rfc2616.html#section-14.14)
* [Content-MD5](https://www.ietf.org/rfc/rfc2616.html#section-14.15)
* [Content-Range](https://www.ietf.org/rfc/rfc2616.html#section-14.16)
* [Content-Type](https://www.ietf.org/rfc/rfc2616.html#section-14.17)
* [Expires](https://www.ietf.org/rfc/rfc2616.html#section-14.21)
* [Last-Modified](https://www.ietf.org/rfc/rfc2616.html#section-14.29)

<br/>

### status-code

* [HTTP/1.0](https://www.ietf.org/rfc/rfc1945.html#section-6.1.1)
* [HTTP/1.1](https://www.ietf.org/rfc/rfc2616.html#section-6.1.1)
