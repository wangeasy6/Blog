# Android系统&四大组件
#####

### Android简介
* 基于Linux的开源系统
* 支持Java/Kotlin/C 开开发应用
* Java基础：《Thinking in java》
* XML基础：用于开发页面布局
* Gradle：用于编译构建Android程序

### 环境搭建
* JDK & Android SDK
* IDE Android Studio
* https://developer.android.google.cn/guide/components/activities/index.html
* https://classroom.udacity.com/courses/ud851
* 优达学城

### 平台架构
* Power
* Linux Kernel
* HAL
* Native C/C++ Lib & Android Runtime(5.0+,代替Dalvik VM)(AOT/JIT)
* Java API Framework
* APP

### 四大组件
* UI、操作响应、后台、数据操作、消息推送
* Activity(UI、事件响应)、Service(后台服务)、Conternt Provider(数据共享)、Broadcast Reveiver(消息推送)(耳机拔插)(通知机制)
* 后台线程可以与UI挂钩，Service剥离了UI(运行在主线程)
* Broadcast 分为动态广播和静态广播
