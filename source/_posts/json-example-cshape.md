---
title: JSON例程 - C#
date: 2021-10-28 16:58:29
toc: true
categories:
  - tech
---

<img src="/resources/JsonExample/json_cshape.jpg" width="512" height="256"/>

JSON（JavaScript Object Notation）是一种轻量级的数据交换格式。

它基于 ECMAScript (欧洲计算机协会制定的js规范)的一个子集，采用完全独立于编程语言的文本格式来存储和表示数据。

简洁和清晰的层次结构使得 JSON 成为理想的数据交换语言。

<!--more-->

<br/>

### [JSON示例](/resources/JsonExample/json_example.json)

```json
{
    "name":"体检报告",
    "personnel":[
        {
            "name":"张三",
            "height": 175,
            "is_married":false,
            "advice":null
        },
        {
            "name":"李四",
            "height":172.5,
            "is_married":true,
            "advice":"随访观察"
        }
    ]
}
```

此示例包含了所有 JSON 元素，包括：对象（object）、数组（array）、字符串（*string*）、数值(number)、`true`、`false`、 `null`。

### [C# 示例](/resources/JsonExample/JsonExampleCShape.zip)

1、 新建项目，控制台应用，框架`.NET Framework 4.5.1`，名称 JsonExampleCShape，确定

2、 管理 NuGet 程序包，搜索安装 Newtonsoft.Json (当前13.0.1版本)

3、 写程序，如下：

   ```c#
   using Newtonsoft.Json;
   using Newtonsoft.Json.Linq;
   using System;
   using System.Collections.Generic;
   using System.IO;
   using System.Linq;
   using System.Text;
   using System.Threading.Tasks;
   
   namespace JsonExampleCShape
   {
       class Program
       {
           static void Main(string[] args)
           {
               Read();
               Write();
               Console.ReadLine();
           }
   
           private static void Read()
           {
               JObject json = JsonConvert.DeserializeObject<JObject>(File.ReadAllText("json_example.json"));
               Console.WriteLine(json.GetType());
               Console.WriteLine(json);
   
               Console.WriteLine(json["name"].GetType());
               Console.WriteLine(json["name"]);
   
               Console.WriteLine(json["personnel"].GetType());
               Console.WriteLine(json["personnel"]);
   
               foreach ( JObject person in json["personnel"] )
               {
                   Console.WriteLine(person["name"].GetType());
                   Console.WriteLine(person["name"]);
                   Console.WriteLine(person["height"].GetType());
                   Console.WriteLine(person["height"]);
                   Console.WriteLine(person["is_married"].GetType());
                   Console.WriteLine(person["is_married"]);
                   Console.WriteLine(person["advice"].GetType());
                   Console.WriteLine(person["advice"]);
               }
           }
   
           private static void Write()
           {
               JObject obj = new JObject();
               obj["name"] = "体检报告";
   
               JArray personnel = new JArray();
               JObject person = new JObject();
               person["name"] = "张三";
               person["height"] = 175;
               person["is_married"] = false;
               person["advice"] = null;
               personnel.Add(person);
   
               person = new JObject();
               person["name"] = "李四";
               person["height"] = 172.5;
               person["is_married"] = true;
               person["advice"] = "随访观察";
               personnel.Add(person);
   
               obj["personnel"] = personnel;
   
               string content = JsonConvert.SerializeObject(obj, Formatting.Indented);
               File.WriteAllText("output.json", content);
   
               Console.WriteLine("Write :\r\n" + content + "\r\nTo output.json");
           }
       }
   }
   ```

   启动，输出：

   ```
   Newtonsoft.Json.Linq.JObject
   {
     "name": "体检报告",
     "personnel": [
       {
         "name": "张三",
         "height": 175,
         "is_married": false,
         "advice": null
       },
       {
         "name": "李四",
         "height": 172.5,
         "is_married": true,
         "advice": "随访观察"
       }
     ]
   }
   Newtonsoft.Json.Linq.JValue
   体检报告
   Newtonsoft.Json.Linq.JArray
   [
     {
       "name": "张三",
       "height": 175,
       "is_married": false,
       "advice": null
     },
     {
       "name": "李四",
       "height": 172.5,
       "is_married": true,
       "advice": "随访观察"
     }
   ]
   Newtonsoft.Json.Linq.JValue
   张三
   Newtonsoft.Json.Linq.JValue
   175
   Newtonsoft.Json.Linq.JValue
   False
   Newtonsoft.Json.Linq.JValue
   
   Newtonsoft.Json.Linq.JValue
   李四
   Newtonsoft.Json.Linq.JValue
   172.5
   Newtonsoft.Json.Linq.JValue
   True
   Newtonsoft.Json.Linq.JValue
   随访观察
   Write :
   {
     "name": "体检报告",
     "personnel": [
       {
         "name": "张三",
         "height": 175,
         "is_married": false,
         "advice": null
       },
       {
         "name": "李四",
         "height": 172.5,
         "is_married": true,
         "advice": "随访观察"
       }
     ]
   }
   To output.json
   ```

   可以看到，使用Newtonsoft.Json解析的值全部是JValue，在运算时还需要转换为C#的标准数据类型。

   除了JObject之外，C#的Json模块还支持非常多的内建对象的序列化。

### 参考链接

* [Json.NET Documentation](https://www.newtonsoft.com/json/help/html/Introduction.htm)

