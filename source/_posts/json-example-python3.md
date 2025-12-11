---
title: JSON例程 - Python3
date: 2021-10-27 23:27:29
toc: true
categories:
  - tech
tags:
  - Python
---

<img src="/resources/JsonExample/json_python.jpg" width="512" height="256"/>

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

### [Python3 示例](/resources/JsonExample/json_example.py)

```python
# encoding=utf-8

import json


def read():
    file_path ="./json_example.json"
    print("Read " + file_path)
    with open(file_path, encoding='utf-8') as file:
        file_content = file.read()
        print(file_content)

        result = json.loads(file_content)        # 将已编码的 JSON 字符串解码为 Python 对象
        print(type(result), result)
        print(type(result["name"]), result["name"])
        print(type(result["personnel"]), result["personnel"])

        for person in result["personnel"]:
            print(type(person["name"]), person["name"])
            print(type(person["height"]), person["height"])
            print(type(person["is_married"]), person["is_married"])
            print(type(person["advice"]), person["advice"])


def write():
    out_file_name = "output.json"
    obj = {}
    obj["name"] = "体检报告"

    personnel = []
    person = {}
    person["name"] = "张三"
    person["height"] = 175
    person["is_married"] = False
    person["advice"] = None
    personnel.append(person)

    person = {}
    person["name"] = "李四"
    person["height"] = 172.5
    person["is_married"] = True
    person["advice"] = "随访观察"
    personnel.append(person)

    obj["personnel"] = personnel

    content = json.dumps(obj, ensure_ascii=False, indent=4)  # 对象 转换为 str
    print("Write :\r\n" + content)
    with open(out_file_name, "w", encoding="utf-8") as file:
        file.write(content)
    print("to " + out_file_name)


if __name__ == '__main__':
    read()
    write()
```

输出：

```
Read ./json_example.json
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
<class 'dict'> {'name': '体检报告', 'personnel': [{'name': '张三', 'height': 175, 'is_married': False, 'advice': None}, {'name': '李四', 'height': 172.0, 'is_married': True, 'advice': '随访观察'}]}
<class 'str'> 体检报告
<class 'list'> [{'name': '张三', 'height': 175, 'is_married': False, 'advice': None}, {'name': '李四', 'height': 172.0, 'is_married': True, 'advice': '随访观察'}]
<class 'str'> 张三
<class 'int'> 175
<class 'bool'> False
<class 'NoneType'> None
<class 'str'> 李四
<class 'float'> 172.5
<class 'bool'> True
<class 'str'> 随访观察
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
to output.json
```

