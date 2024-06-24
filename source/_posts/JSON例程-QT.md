---
title: JSON例程-QT
date: 2024-5-31 22:37:00
toc: true
categories:
  - Technology
tags:
  - QT
---

<img src="/resources/JsonExample/json_qt.jpg" width="512" height="256"/>

补充一个 QT 版本的 JSON 例程。

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

### [QT 示例](/resources/JsonExample/json_example.py)

保存 JSON 示例为文件 `json_example.json`，放置在程序同一目录。

```c++
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void read()
{
    QString file_path ="./json_example.json";
    QFile json_fd(file_path);

    // 打开文件以供读取
    if (!json_fd.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file";
        return;
    }

    QTextStream in(&json_fd);
    QString json_str = in.readAll();
    json_fd.close();
    // qDebug().noquote() << json_str;

    // 将 JSON 字符串转换为 QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json_str.toUtf8());

    // 检查 JSON 是否有效
    if (jsonDoc.isNull()) {
        qDebug() << "Invalid JSON";
        return;
    }

    // 获取 QJsonObject
    QJsonObject jsonObj = jsonDoc.object();
    // 读取数据
    qDebug() << jsonObj["name"].isString() << jsonObj["name"].toString();
    QJsonArray jsonArray = jsonObj["personnel"].toArray();
    for (const QJsonValue &person : jsonArray) {
        qDebug() << person["name"].toString();
        qDebug() << person["height"].toDouble();
        qDebug() << person["is_married"].toBool();
        qDebug() << person["advice"].toString();
    }
}

void write()
{
    QString out_file_name = "output.json";
    QFile json_fd(out_file_name);

    // 打开文件以供读写
    if (!json_fd.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file";
        return;
    }

    // 创建 QJsonObject
    QJsonObject jsonObj;
    jsonObj["name"] = "体检报告";

    // 创建 QJsonArray
    QJsonArray person_list;

    QJsonObject person;
    person["name"] = "张三";
    person["height"] = 175;
    person["is_married"] = false;
    person["advice"] = QJsonValue();    // The default is to create a Null value.
    person_list.append(person);
    person["name"] = "李四";
    person["height"] = 172.5;
    person["is_married"] = true;
    person["advice"] = "随访观察";
    person_list.append(person);

    // 将 QJsonArray 添加到 QJsonObject
    jsonObj["personnel"] = person_list;

    // 创建 QJsonDocument
    QJsonDocument jsonDoc(jsonObj);

    // 将 QJsonDocument 转换为 JSON 字符串
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Indented);

    // 写入数据到文件
    if (json_fd.write(jsonData) == -1) {
        qDebug() << "Failed to write to file";
    } else {
        qDebug() << "Written successfully";
    }
    json_fd.close();
}

int main(int argc, char *argv[])
{
    read();
    write();
}

```

输出：

```
true "体检报告"
"张三"
175
false
""
"李四"
172.5
true
"随访观察"
Written successfully
```

