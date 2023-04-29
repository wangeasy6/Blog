# Error
##### 错误或者报警的解决方法

### make编译，解决Clock skew detected
* 表示文件修改时间在编译时间之后，报警
* 查看是系统时区未修改
  * 执行tzselect命令-->选择Asia-->选择China-->选择east China - Beijing, Guangdong, Shanghai, etc-->然后输入1
  * 执行完tzselect命令选择时区后，时区并没有更改，只是在命令最后提示你可以执行TZ='Asia/Shanghai'; export TZ 并将这行命令添加到.profile中，然后退出并重新登录
