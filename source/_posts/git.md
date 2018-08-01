---
title: Git使用手记
date: 2018-08-01 22:04:12
categories:
  - tools
toc: true
---

git 是 Linus Torvalds 除了 Linux 之外的另一杰作，是现如今使用最多的多人协作版本管理工具
<!--more-->

### 常用命令
* 查看远程路径
`git remote -v`

* 本地提交到服务器
git push/ git push origin master , 通常在commit之后

* 从服务器拉回本地，即更新本地文件
git pull

* 回退commit
git reset --hard commit_id , 找到需要回退到的版本的 哈希值

### 字符编码统一设置
1 设置git gui的界面编码
git config --global gui.encoding utf-8

2 设置 commit log 提交时使用 utf-8 编码，可避免服务器上乱码，同时与linux上的提交保持一致！
git config --global i18n.commitencoding utf-8

3 使得在 $ git log 时将 utf-8 编码转换成 gbk 编码，解决Msys bash中git log 乱码。
git config --global i18n.logoutputencoding gbk

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
  * 解决办法：<cmd>
      $ rm -rf .git  // 删除.git  
      $ git config --global core.autocrlf false  //禁用自动转换    
      然后重新执行：
      $ git init
      $ git add .
      </cmd>
      http://blog.csdn.net/unityoxb/article/details/20768687
* windows Git Bash : git status 中文乱码（如："\200\346\261\202"）
  * 解决办法：git config --global core.quotepath false
