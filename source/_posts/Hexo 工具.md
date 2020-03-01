---
title: 方便的 Hexo 博客工具
date: 2018-07-18 16:57:00
toc: true
categories:
  - tech
---

* Hexo 是一个静态博客生成器，基于 NodeJs 和 GitHub。 
* 使用 Hexo 的原因如下：
  * 我喜欢用 MarkDown 来记笔记，Hexo 可以很方便的将 MarkDown 文件转主题网页，并发布。
  * 我习惯于 GitHub 管理文档，GitHub 对 Hexo 有深度的支持，可以节省服务器的开支。
  * 之前用过 WordPress，但是在线编辑和自己管理服务器都不是很让人舒心。
<!--more-->

<br/>
### 一、安装（基于Windows）

使用 Hexo 框架，整体思路是用电脑编辑了 md 文档，然后使用 hexo 生成静态页面，最后上传到 GitHub，由 GitHub 服务器提供网页解析。

0.原料
  * Windows 电脑 * 1
  * GitHub 账号 * 1

#### 1.安装 Git
  * [下载地址]( https://git-scm.com)
  * 安装步骤：双击下载好的exe文件，一路next就好啦
  * 安装好后，在桌面右键打开gitbash，查看版本： 
    * 命令：`git version`

#### 2.安装 NodeJs
Hexo 是基于 NodeJs 开发的。
   * [下载地址](https://nodejs.org/en/)(说明：LTS为长期支持版，Current为当前最新版) 
   * 安装步骤：下载好msi文件后，双击打开安装，也是一路next 
   * 查看版本：
     * 打开CMD：`node -v`

#### 3.安装 Hexo
   * 打开CMD：`npm i -g hexo`
   * 安装完成后，查看版本：
     * `hexo -v`

<br/>
### 二、配置

#### 1.GitHub配置

  * 没账号的先创建账号。
  * 在 GitHub 上创建一个repo，用于发布博客，如下：

  ![new-repo](/images/Hexo 工具/new-repo.png)

  ![create-repo](/images/Hexo 工具/create-repo.png)

  * 回到gitbash中，配置github账户信息（YourName和YourEail都替换成你自己的）：

  ![username](/images/Hexo 工具/username.png)
  ![email](/images/Hexo 工具/email.png)

  * 创建SSH
    在gitbash中输入：`ssh-keygen -t rsa -C "youremail@example.com`，生成ssh。
    然后按下图的方式找到 id_rsa.pub 文件的内容。

  ![ssh-key](/images/Hexo 工具/ssh.png)

  * 将上面获取的ssh放到github中：

  ![settings](/images/Hexo 工具/settings.png)

  * 添加一个 New SSH key ，title随便取，key就填刚刚那一段。
    ![ssh-key](/images/Hexo 工具/ssh-key.png)

  * 在gitbash中验证是否添加成功：`ssh -T git@github.com`

#### 2.初始化 Hexo 仓库

  * CMD：`hexo init`
  * 初始化完成之后打开所在的文件夹可以看到以下文件： 

  ![hexo-init](/images/Hexo 工具/hexo-init.png)

  * 解释： 
    * node_modules：是依赖包
    * public：存放的是生成的页面
    * scaffolds：命令生成文章等的模板
    * source：用命令创建的各种文章
    * themes：主题
    * _config.yml：整个博客的配置
    * db.json：source解析所得到的
    * package.json：项目所需模块项目的配置信息

#### 3.Hexo 初次使用

  * 执行 CMD:
```
  hexo clean
  hexo generate
  hexo server
```
  打开浏览器输入：http://localhost:4000

  接着你就可以看见一个默认 landscape 主题的博客了。

#### 4.上传到 GitHub

  * 先安装 Hexo 的 git上传工具：`npm install hexo-deployer-git  --save`
  * 用编辑器打开你的blog项目，修改 "_config.yml" 文件的发布配置(冒号之后都是有一个半角空格的)
```
  deploy:
    type: git
    repo: https://github.com/YourgithubName/c.git
    branch: master
```
  * 执行命令(建议每次都按照如下步骤部署)：

```
  hexo clean
  hexo generate
  hexo deploy
```
  deploy 的过程中需要输入你的GitHub username 及 passward 。

  上传完后，在浏览器中输入 [http://yourgithubname.github.io/YourRepository](http://yourgithubname.github.io/YourRepository) 就可以看到你的个人博客啦！

<br/>
### 三、使用

#### 1. 写文章

  * 新建文章：`hexo new "文章名"`，然后你就可以在 source/\_posts/ 路径下看到你创建的文章 —— “文章名.md”
  * 也可以使用 `hexo new draft "文章名"` 建一篇草稿，文章会先放到 source/\_drafts/，待写好之后再使用`hexo publish "文章名"`发布到  source/\_posts/
  * 可在开头添加文章信息，如：

  ```
  ---
  title: Hexo 工具使用小结
  date: 2018-07-18 16:57:00
  categories:
    - tech
  tags:
    - tools
    - Hexo
  ---
  ```

  `---` 是 Header 声明；
  title 是文章标题；
  date  是文章日期；
  categories 是文章分类，无需预先创建，可以有多个；
  tags  是文章标签，无需预先创建，可以有多个；

  * 重新生成并发布，在浏览器刷新就能看到你的文章了。

#### 2. 切换主题
* 在 GitHub 上先找到自己想要的主题
  * [hexo.io/themes](https://hexo.io/themes/)
* 下载主题到 Hexo 仓库，如下：
  `git clone https://github.com/iissnan/hexo-theme-next themes/next`
* 修改主题配置文件
  在 _config.yml 找到 theme，修改为：
  `theme: next`
* 重新生成，发布

#### 3. 编辑目录
* 编辑主题目录下的 _config.yml，而不是根目录下的 _config.yml 文件。
```
menu:
  关于: /about
  技术: /categories/tech
```
* 配置之后，重新发布。在目录中就会出现 “技术”、“关于” 这两项，
  链接分别会打开 [site]/about 、 [site]/categories/tech 页面，但是，这个时候这两个链接为空
  所以得创建:
    `hexo new page about`
  当我们在 文章 中声明了 categories 这一项，如前面声明的 tech，hexo 就会在 /categories 目录中创建一个目录为 tech，将所有类别声明中带有 tech 的 md 转为静态页面存放在此，所以 “技术” 这一栏会列出所有类别为 tech 的文章。
#### 4. 文章只显示一部分
* 这个只要在文章中加上 <!--more--> 标记 ，该标记以后部分就不在显示了，只有展开全部才显示，这是hexo定义的。 

#### 5. 避免 deploy 的时候删除 CNAME、README.md
* 将 CNAME、README.md 放到 /source 中，在这里更新在发布的时候能同步到 GitHub。

#### 6. 上传了图片链接不到
* 踩了个坑，在本地的时候使用了相对于文本的路径，如 ../images/xx.png
  传到 GitHub 上就无法链接了。
* 这个时候应该使用网站的相对路径，所有之前的图片文件夹都会被复制到网站的根目录
  所以，这是时候应该使用相对于网站的路径， /images/xx.png

### 参考

* https://www.cnblogs.com/visugar/p/6821777.html