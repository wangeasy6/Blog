---
title: Use MQTT by Python
toc: true
---

MQTT是OASIS（结构化信息标准促进组织）为物联网（IOT）制定的消息传输协议，目前[阿里云](https://help.aliyun.com/document_detail/86706.html?spm=a2c4g.11174283.2.44.3a8b1668RY0TdQ)、[腾讯云](https://cloud.tencent.com/document/product/634/14630)、[小米IOT](https://iot.mi.com/new/doc/embedded-development/overview)都在使用这个协议，用于平台和设备之间的消息传输。

基于Python，我们来看一下MQTT有哪些特性，能实现哪些功能，如何实现的。

<!--more-->
<br/>

## MQTT机制

MQTT（Message Queuing Telemetry Transport，消息队列遥测传输协议），是一种基于发布/订阅（publish/subscribe）模式的"轻量级"通讯协议，该协议构建于TCP/IP协议上。

实现MQTT协议需要客户端和服务器端，在通讯过程中，MQTT协议中有三种身份：发布者（Publish）、代理（Broker）（服务器）、订阅者（Subscribe）。其中，消息的发布者和订阅者都是客户端，消息代理是服务器，消息发布者可以同时是订阅者。

MQTT传输的消息分为：主题（Topic）和负载（payload）两部分：

- （1）Topic，可以理解为消息的类型，订阅者订阅（Subscribe）后，就会收到该主题的消息内容（payload）；
- （2）payload，可以理解为消息的内容，是指订阅者具体要使用的内容。

一个使用MQTT协议的应用程序或者设备，它总是建立到服务器的网络连接。客户端可以：

- （1）发布其他客户端可能会订阅的信息；
- （2）订阅其它客户端发布的消息；
- （3）退订或删除应用程序的消息；
- （4）断开与服务器连接。

MQTT服务器以称为"消息代理"（Broker），可以是一个应用程序或一台设备。它是位于消息发布者和订阅者之间，它可以：

- （1）接受来自客户的网络连接；
- （2）接受客户发布的应用信息；
- （3）处理来自客户端的订阅和退订请求；
- （4）向订阅的客户转发应用程序消息。

<br/>

## MQTT使用

目前，已经有了[许多MQTT的软件实现](https://mqtt.org/software/)，Broker/Client、不同语言的、基于不同版本的。

我们在这里选用[Eclipse Paho Python](https://github.com/eclipse/paho.mqtt.python)的Client和[腾讯云](https://cloud.tencent.com/document/product/634/14630)的Broker。

### 订阅和发布

```python
# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import json
import time

HOST = "host"
PORT = 1883
client_id = "client_id"
user_name = "user_name"
possword = "possword"

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("gate/door_1/event", 1)


def on_message(client, userdata, msg):
    print("Receive Message:\r\n主题:  "+msg.topic+"\r\n消息:  "+str(msg.payload.decode('utf-8')))


def on_subscribe(client, userdata, mid, granted_qos):
    print("On Subscribed: qos = %d" % granted_qos)


def on_publish(client, userdata, mid):
    print("On Publish: %s" % userdata)


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection %s" % rc)


data = {
    "timestamp": time.time(),
    "message":"Test"
}
param = json.dumps(data)
client = mqtt.Client(client_id)
client.username_pw_set(user_name, possword)	       # 设置user_name和possword
client.on_connect = on_connect
client.on_message = on_message
client.on_subscribe = on_subscribe
client.on_disconnect = on_disconnect
client.connect(HOST, PORT, 120)
client.publish("topic", payload=param, qos=1)      # 发布消息
client.loop_forever()
```





### 在平台配置设备





## MQTT的优缺点



