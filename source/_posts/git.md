---
title: Git使用手记
date: 2018-08-01 22:04:12
toc: true
categories:
  - Technology
tag:
  - Tools
---

git 是 Linus Torvalds 除了 Linux 之外的另一杰作，是现如今使用最多的分布式版本管理工具。

<!--more-->

### Git仓库(CentOS)
  * 创建用户git，用来管理运行git服务
    * useradd <UserID>
    * passwd <UserID>
  * Linux配置无密码SSH登陆
    * gitClient: ssh-keygen –t rsa -C "user@email"生成密钥，在.ssh目录下，我们可以看到id_rsa和id_rsa.pub文件，id_rsa.pub为公钥（Windows目录：/c/Users/xxx/.ssh/id_rsa;如果是相同服务器地址重新添加，则需要删除原来的 known_hosts 对应的条目）
    * 在gitServer上我们首先查看/home/git/.ssh/authorized_keys，没有则创建
    * 公钥id_rsa.pub的内容追加到authroized_keys中
    * ssh -l git <ServerIP> ,验证是否可以自动登陆
  * 通过命令 sudo yum install git-core,安装git（gitServer）
  * 使用命令 git --bare init /home/git/myRep,初始化仓库
  * 在gitClient上，通过git clone命令进行克隆远程仓库
    * Git clone git@gitServer:/home/git/myRep


### 常用命令
* 查看远程路径
  `git remote -v`

* 本地提交到服务器
  `git push/ git push origin master` , 通常在commit之后

* 从服务器拉回本地，即更新本地文件
  `git pull`

* 将修改的文件恢复为最近提交版本或添加之前

`git checkout xx_file`

* 比较两次commit的差异

  `git diff fee9f9cf4b cdd577e352 xx_file`

* 修改最近一次未提交的commit Log信息
  `git commit --amend`  （如果你此时想要更新作者提交时间等也可以在amend之后加上 --date="$(date -R)"）

* 修改已push的commit信息

  `git commit --amend`

  `git push -f`（强制提交）

* 批量修改提交历史
  http://www.cnblogs.com/rickyk/p/3981083.html
  https://git-scm.com/book/en/v2/Git-Tools-Rewriting-History#Changing-Multiple-Commit-Messages

* 切换历史版本

  1、 查找历史版本

    使用`git log`命令查看所有的历史版本，获取你git的某个历史版本的id
    假设查到历史版本的id是fae6966548e3ae76cfa7f38a461c438cf75ba965。
  2、 恢复到历史版本

  `$ git reset --hard fae6966548e3ae76cfa7f38a461c438cf75ba965`

  3、 把修改推到远程服务器

  `$ git push -f -u origin master`


### 字符编码统一设置
1 设置git gui的界面编码
`git config --global gui.encoding utf-8`

2 设置 commit log 提交时使用 utf-8 编码，可避免服务器上乱码，同时与linux上的提交保持一致！
`git config --global i18n.commitencoding utf-8`

3 使得在 $ git log 时将 utf-8 编码转换成 gbk 编码，解决Msys bash中git log 乱码。
`git config --global i18n.logoutputencoding gbk`

4 使得 git log 可以正常显示中文（配合i18n.logoutputencoding = gbk)，在 /etc/profile 中添加：
  export LESSCHARSET=utf-8

  ```
  git config --global gui.encoding utf-8
  git config --global i18n.commitencoding utf-8
  git config --global i18n.logoutputencoding utf-8
  export LESSCHARSET=utf-8
  ```

### 记住密码
* `git config --global credential.helper store`

* 默认记住15分钟：
  `git config –global credential.helper cache`

* 自定义配置记住1小时：
  `git config credential.helper ‘cache –timeout=3600’`

### Error
* windows使用git时出现：warning: LF will be replaced by CRLF
  * 解决办法：(cmd)
      $ `rm -rf .git`  // 删除.git
      $ `git config --global core.autocrlf false`  //禁用自动转换
      然后重新执行：
      $ `git init`
      $ `git add .`
      </cmd>
      http://blog.csdn.net/unityoxb/article/details/20768687
* windows Git Bash : `git status` 中文乱码（如："\200\346\261\202"）
  * 解决办法：`git config --global core.quotepath false`
