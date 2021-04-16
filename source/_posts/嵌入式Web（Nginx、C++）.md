---
title: 嵌入式Web(Nginx、C++)
date: 2020-09-25 11:35:01
toc: true
categories:
  - Technology
tags:
  - C++
---

![Cover](/resources/embedded_web/cover.jpg)

在设备中嵌入Web，作为配置软件的接口是目前嵌入式设备的常见做法了。

由于种种原因，暂定为这个方案，虽然行业中使用这个方案的人不多，但还是记录一下，万一有一样的小伙伴呢。

实现方案： *树莓派3B+*、*Nginx*、*spawn-fcgi*、*C++*。

<!--more-->

<br/>

### 系统架构

![framework](/resources/embedded_web/framework.png)

### 软件安装

树莓派3B+，镜像版本：2020-05-27-raspios-buster-armhf.zip

**1.安装Nginx**

`sudo apt-get install nginx`

`nginx -v`："nginx version: nginx/1.14.2"

**2.安装fast-cgi库**

使用C++编写FCGI程序，需要安装fast-cgi库。

`sudo apt-get install libfcgi-dev`

**3.安装spawn-fcgi**

要启动cgi程序，需要安装spawn-fcgi。

`sudo apt-get install spawn-fcgi`

<br/>

### 配置

配置Nginx：

```shell
mkdir /home/pi/Web
vim /etc/nginx/nginx.conf
```

在http{}中添加：

```
http {
    # ...

    server
    {
        listen 8080 default_server;
        root /home/pi/Web;              # 网页根目录
        location ~\.cgi$
        {
            fastcgi_pass 127.0.0.1:8000;
            fastcgi_index index.cgi;
            include fastcgi.conf;
        }
    }
}
```

此配置项配置了server监听8080端口的http连接，使用`http://设备ip:8080`就能访问到我们的Web服务；

网页的根目录为创建的`/home/pi/Web`，网页静态文件存在这个目录下；

所有带有`.cgi`后缀的访问都转发给fastcgi，fastcgi的地址为`127.0.0.1:8000`；

`include fastcgi.conf`是添加引用“fastcgi.conf”的规则，这个规则如下：

在`/etc/nginx/`中添加文件[fastcgi.conf](/resources/embedded_web/fastcgi.conf)：

```
fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
fastcgi_param  SERVER_SOFTWARE    nginx;

fastcgi_param  QUERY_STRING       $query_string;
fastcgi_param  REQUEST_METHOD     $request_method;
fastcgi_param  CONTENT_TYPE       $content_type;
fastcgi_param  CONTENT_LENGTH     $content_length;

fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
fastcgi_param  REQUEST_URI        $request_uri;
fastcgi_param  DOCUMENT_URI       $document_uri;
fastcgi_param  DOCUMENT_ROOT      $document_root;
fastcgi_param  SERVER_PROTOCOL    $server_protocol;

fastcgi_param  REMOTE_ADDR        $remote_addr;
fastcgi_param  REMOTE_PORT        $remote_port;
fastcgi_param  SERVER_ADDR        $server_addr;
fastcgi_param  SERVER_PORT        $server_port;
fastcgi_param  SERVER_NAME        $server_name;
```

以上的`fastcgi_param`配置，配置了Nginx传递给FCGI程序的环境变量。

在C++中可以使用`FCGX_GetParam`函数获取到这些值。编辑一个用来测试的CGI程序，如下所示：

```C++
// test.cpp

#include <iostream>
#include "fcgio.h"
using namespace std;

const string ENV[] = {
    "REMOTE_ADDR", "REMOTE_PORT",
    "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
    "SCRIPT_NAME", "SERVER_ADDR", "SERVER_PORT"};

int main (int argc, char *argv[])
{
    streambuf *cin_streambuf = cin.rdbuf();
    streambuf *cout_streambuf = cout.rdbuf();
    streambuf *cerr_streambuf = cerr.rdbuf();

    FCGX_Init();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0)
    {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        cout << "Content-type:text/html\r\n\r\n";
        cout << "<html>\n";
        cout << "<head>\n";
        cout << "<title>CGI ENV</title>\n";
        cout << "</head>\n";
        cout << "<body>\n";
        cout << "<h1>CGI ENV</h1>\n";
        cout << "<table border = \"0\" cellspacing = \"2\">";

        for ( int i = 0; i < 8; i++ )
        {
           cout << "<tr><td>" << ENV[ i ] << "</td><td>";
           const char *value = FCGX_GetParam(ENV[i].c_str(), request.envp);
           if ( value != 0 )
           {
                cout << value;
           }
           else
           {
                cout << "Not Exist";
           }
           cout << "</td></tr>\n";
        }
        cout << "</table>\n";
        cout << "</body>\n";
        cout << "</html>\n";
    }

    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

   return 0;
}
```

编译：

```shell
g++ test.cpp -o test -lfcgi -lfcgi++
```

启动CGI服务：

```shell
spawn-fcgi -a 127.0.0.1 -p 8888 -f test  # -n 进入调试模式，可以看到程序的错误输出信息
```

成功时输出：

```
spawn-fcgi: child spawned successfully: PID: 7144
```

查看网络状态，确定FCGI程序是否正在运行：

```shell
sudo netstat -apn | grep 8888
```

成功时会输出如下语句：

```
tcp        0      0 127.0.0.1:8888          0.0.0.0:*               LISTEN      7144/./index
```

启动Nginx服务：

```
sudo nginx -s reload
```

Nginx常用命令：

```
sudo nginx
sudo nginx -s stop
sudo nginx -s quit
sudo nginx -s reload
sudo nginx -s reopen
```

现在打开浏览器，输入以下网址进行访问：

```
http://ip:8080/test.cgi
```

浏览器显示：

![test_cgi.jpg](/resources/embedded_web/test_env.jpg)

<br/>

### 异步获取、设置设备参数

实现功能：

* 异步获取、设置设备版本参数
* 上传小于256KB的JPG文件到服务器
* 文件MD5校验

项目所有文件[Web.tar.gz](/resources/embedded_web/Web.tar.gz)：

```shell
Web
├── cgi-bin
│   ├── makefile
│   ├── main.cpp
│   ├── urls.cpp
│   ├── urls.h
│   ├── char_md5.c
│   ├── char_md5.h
│   ├── md5.c
│   └── md5.h
├── index.html
├── js
│   ├── index.js
│   ├── date.format.js
│   └── spark-md5.js
```

makefile

```
all : main.cpp urls.cpp md5.c char_md5.c
	g++ $^ -o main -lfcgi -lfcgi++

.PHONY : run
run :
	spawn-fcgi -a 127.0.0.1 -p 8888 -f ./main -n

.PHONY : clean
clean :
	sudo killall main
```

main.cpp

```c++
#include <iostream>
#include <string.h>
#include "fcgio.h"
#include "urls.h"
using namespace std;

int main (int argc, char *argv[])
{
    streambuf *cin_streambuf = cin.rdbuf();
    streambuf *cout_streambuf = cout.rdbuf();
    //streambuf *cerr_streambuf = cerr.rdbuf();

    FCGX_Init();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0)
    {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        //fcgi_streambuf cerr_fcgi_streambuf(request.err);	    # 取消托管cerr，在调试的时候使用cerr可以直接输出

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        //cerr.rdbuf(&cerr_fcgi_streambuf);

        char *request_uri = FCGX_GetParam("REQUEST_URI", request.envp);
        char *query_str = FCGX_GetParam("QUERY_STRING", request.envp);
        char *content_len = FCGX_GetParam("CONTENT_LENGTH", request.envp);

        if ( strstr(request_uri, "get_version.cgi") )
        {
            getVersion();
            continue;
        }
        if ( strstr(request_uri, "set_version.cgi") )
        {
            setVersion(query_str);
            continue;
        }
        if ( strstr(request_uri, "post_image.cgi") )
        {
            postImage(query_str, content_len);
            continue;
        }

        cout << "Content-type:text/html\r\n\r\n";
        cout << "{\"result\":\"False\", \"message\":\"Protocol not supported\"}\n";
    }

    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    //cerr.rdbuf(cerr_streambuf);

    return 0;
}
```

urls.h

```c++
#ifndef __URLS__
#define __URLS__

void getVersion();
void setVersion(char *query_str);
void postImage(char *query_str, char *content_len);

#endif
```

urls.cpp

```C++
#include <iostream>
#include <fstream>
#include <string.h>
#include "char_md5.h"
using namespace std;

string version = "1.0";


void getVersion()
{
    cout << "Content-type:text/html\r\n\r\n";
    cout << "{\"result\":\"True\", \"message\":\"Success\", " <<  \
        "\"version\":\"" << version << "\"}\n";
}


void setVersion(char *query_str)
{
    cout << "Content-type:text/html\r\n\r\n";
    if(!query_str)
    {
        cout << "{\"result\":\"False\", \"message\":\"参数有误\"}\n";
        return;
    }

    version = query_str;

    cout << "{\"result\":\"True\", \"message\":\"Success\"}\n";
}


void postImage(char *query_str, char *content_len)
{
    cout << "Content-type:text/html\r\n\r\n";
    if(!content_len || !query_str)
    {
        cout << "{\"result\":\"False\", \"message\":\"参数有误\"}\n";
        return;
    }

    // 接收&存储 照片
    int len = atoi(content_len);
    char md5_str[MD5_CHAR_LEN];
    if (len > 0)
    {
        char img[len];
        for (int i=0; i < len; i ++)
        {
            img[i] = cin.get();
        }
        if (img)
        {
            //cout << "Conten Len :" << len << endl;
            //cout << "Image Len:" << strlen(img) << endl;
            const char* fileName = "./image.jpg";
            ofstream f(fileName, ios::out | ios::binary);
            f.write(img, len);
            f.close();

            Compute_string_md5((unsigned char *)img, len, md5_str);
        }
    }
    else
    {
        cout << "{\"result\":\"False\", \"message\":\"image数据为空\"}\n";
        return;
    }

    string md5sum = query_str;
    if(strcmp(md5_str, query_str) != 0)
    {
        cout << "{\"result\":\"False\", \"message\":\"文件MD5校验有误\"}\n";
        return;
    }

    cout << "{\"result\":\"True\", \"message\":\"Success\"}\n";
}
```

index.html

```html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>嵌入式Web</title>
  <script src="/js/date.format.js"></script>
  <script src="/js/index.js"></script>
  <script src="/js/spark-md5.js"></script>
  </head>

  <body>
    <div align="center">
      <div align="center" style="height: 60px;">
        <label style="font-weight: bold;">设备版本号:</label>
        <input  type="text" name="txt_software_ver" id="txt_software_ver"  width="50" maxlength="50" size="10" style="height:10"height="10" onFocus="if(this.value=='*'){this.value='';}" value='' />
        <input type="button" value="获取" onclick="getVersion()" style="background-color:transparent"/>
        <input type="button" value="设置" onclick="setVersion()" style="background-color:transparent"/>
      </div>

      <div align="center" style="height: 60px;">
        <input type="file" value="选择照片" id="file" name="file" onchange="selectImage(this);" style="background-color:transparent"/>
        <input type="button" value="上传" onclick="postImage()" style="background-color:transparent"/>
      </div>

    </div>
  </body>

<script type="text/javascript">
var image = '';
var img_md5 = '';

function selectImage(file) {
    if (!file.files || !file.files[0]) {
        image = "";
        return;
    }
    var reader = new FileReader();
    reader.onload = function (e) {
        if (e.target.result.byteLength > 256*1024)
        {
          window.alert("文件超出256KB");
          document.getElementById('file').value = "";
        }
        else
        {
          image = e.target.result;
        }
    }
    reader.readAsArrayBuffer(file.files[0]);
}


document.getElementById("file").addEventListener("change", function() {
    var blobSlice = File.prototype.slice || File.prototype.mozSlice || File.prototype.webkitSlice,
        file = this.files[0],
        chunkSize = 2097152, // 每次读取2MB
        chunks = Math.ceil(file.size / chunkSize),
        currentChunk = 0,
        spark = new SparkMD5.ArrayBuffer(),
        frOnload = function(e){
            spark.append(e.target.result);
            currentChunk++;
            if (currentChunk < chunks)
                loadNext();
            else
                img_md5 = spark.end();
        },
        frOnerror = function () {
            window.alert("MD5获取出错了");
        };
    function loadNext() {
        var fileReader = new FileReader();
        fileReader.onload = frOnload;
        fileReader.onerror = frOnerror;
        var start = currentChunk * chunkSize,
            end = ((start + chunkSize) >= file.size) ? file.size : start + chunkSize;
        fileReader.readAsArrayBuffer(blobSlice.call(file, start, end));
    };
    loadNext();
});
</script>
</html>
```

index.js

```javascript
function getVersion()
{
    var xml_http;
    if (window.XMLHttpRequest)
        xml_http = new XMLHttpRequest();     // code for IE7+, Firefox, Chrome, Opera, Safari
    else
        xml_http = new ActiveXObject("Microsoft.XMLHTTP");// code for IE6, IE5

    if (xml_http!=null)
    {
        xml_http.onreadystatechange=function()
        {
            if (xml_http.readyState==4 && xml_http.status==200)
            {
                var temp = xml_http.responseText;
                console.log(temp);
                var obj = JSON.parse(temp);
                if(obj.result=="True" && obj.version)
                    document.getElementById("txt_software_ver").value = obj.version;
                else
                    window.alert("获取失败");
            }
        }
        xml_http.open("GET","/get_version.cgi",true);
        xml_http.send();
    }
    else
    {
        alert("Your browser does not support XMLHTTP.");
    }
}


function setVersion()
{
    var post_string = '/set_version.cgi?' + document.getElementById("txt_software_ver").value;
    var xml_http;
    if (window.XMLHttpRequest)
        xml_http = new XMLHttpRequest();     // code for IE7+, Firefox, Chrome, Opera, Safari
    else
        xml_http = new ActiveXObject("Microsoft.XMLHTTP");// code for IE6, IE5

    xml_http.onreadystatechange=function()
    {
        if (xml_http.readyState==4 && xml_http.status==200)
        {
            var temp = xml_http.responseText;
            console.log(temp);
            var obj = JSON.parse(temp);
            window.alert(obj.result=="True"?"设置成功":("设置失败" + "\r\n" + obj.message));
        }
    }
    xml_http.open("POST",post_string,true);
    xml_http.send();
}


function postImage()
{
    if(image == "")
    {
        window.alert("请先选择照片");
        return;
    }

    var xml_http;
    if (window.XMLHttpRequest)
        xml_http = new XMLHttpRequest();     // code for IE7+, Firefox, Chrome, Opera, Safari
    else
        xml_http = new ActiveXObject("Microsoft.XMLHTTP");// code for IE6, IE5

    var post_string = '/post_image.cgi?' + img_md5;
    console.log(post_string);
    xml_http.onreadystatechange=function()
    {
        if (xml_http.readyState==4 && xml_http.status==200)
        {
            var temp = xml_http.responseText;
            console.log(temp);
            var obj = JSON.parse(temp);
            window.alert(obj.result=="True"?"上传成功":("上传失败" + "\r\n" + obj.message));
        }
    }
    xml_http.open("POST", post_string, true);
    xml_http.send(image);
}
```

<br/>

### 参考

* [Nginx org](http://nginx.org)
* https://github.com/chinaran/Compute-file-or-string-md5.git