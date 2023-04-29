# 新建用户
#####

### Ubuntu 14
* 新建:
  （1） sudo -r useradd -m -s /bin/bash <User_Name> // -r 参数建立系统用户
  （2） sudo useradd -g root <User_Name> //这一行的命令是让你的刚刚建立的 用户 划分到 root 权限组下
  （3） sudo passwd <User_Name> //设置你刚刚建立的密码
  （4） 赋予root权限，添加到sudo用户组：usermod -G sudo -a <User_Name>

* 上下、删除不可用
   是因为Bash链接不对，可通过 echo $SHELL 命令查看
   修改：sudo usermod -s /bin/bash <UserName>

* 删除：
  在root用户下：userdel -r newuser
  在普通用户下：sudo userdel -r newuser
  //  加上-r的选项，在删除用户的同时一起把这个用户的宿主目录和邮件目录删除

### Debian 7 64bit
* 新建：
  （1） adduser <name>
