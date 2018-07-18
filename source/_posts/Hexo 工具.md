---
title: Hexo 工具使用小结
date: 2018-07-18 16:57:00
categories:
  - tech
tags:
  - tools
---

* Hexo 是一个静态博客生成器，基于 NodeJs 和 GitHub。 
* 使用 Hexo 的原因有3
  * 我喜欢用 MarkDown 来记笔记，而 Hexo 用 MarkDown 作为源文件。
  * 我习惯于 GitHub 管理文档，而 GitHub 对 hexo 有深度的支持，而且节省了一个服务器。
  * 之前用过 WordPress，但是在线编辑和自己管理服务器都不是很让人舒心。

### 一、安装（Windows）

使用 hexo，我们是用自己的电脑编辑 md 文档，然后使用 hexo 生成静态页面，最后上传到 GitHub，由GitHub服务器解析。

1.安装 Git

   - [下载地址]( https://git-scm.com)

   - 安装步骤：双击下载好的exe文件，一路next就好啦

   - 安装好后，在任意文件浏览器位置右键打开gitbash，查看版本： 

     命令：`git version`

2.Hexo 是基于 NodeJs 的，所以第二步是：**安装 NodeJs**

   * [下载地址](https://nodejs.org/en/)(说明：LTS为长期支持版，Current为当前最新版) 
   * 安装步骤：下载好msi文件后，双击打开安装，也是一路next 
   * 查看版本：
     * 打开CMD：`node -v`

3.安装 hexo

   * 打开CMD：`npm i -g hexo`
   * 安装完成后，查看版本：
     * `hexo -v`



### 二、使用

1.初始化一个 Hexo 仓库

   * CMD：`hexo init`
   * 初始化完成之后打开所在的文件夹可以看到以下文件： 
   
   ![hexo-init](D:\Blog\source\images\hexo-init.png)

   * 解释一下： 
     * node_modules：是依赖包
     * public：存放的是生成的页面
     * scaffolds：命令生成文章等的模板
     * source：用命令创建的各种文章
     * themes：主题
     * _config.yml：整个博客的配置
     * db.json：source解析所得到的
     * package.json：项目所需模块项目的配置信息

2.搭桥到github

