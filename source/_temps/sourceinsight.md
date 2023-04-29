# Source Insight
##### 代码编辑工具，基于Source Insight 4.

### sourceinsight4.0破解方法
source Insight V4.00.0080 破30天试用期办法：

第1步：去官方下载source insight V4.00.0080 (sourceinsight4080-setup.exe)
正常安装，然后联网申请一个30天的试用期license

第2步： 对source insight.exe进行打补丁，用vim -b二进制方式打开exe文件，输入":%!xxd"命令转换成16进制，用"/"找到"742a 83bc"字段，
		将“74”改成“eb”，保存，输入":%!xxd -r"恢复成原有格式，修改完成

第3步：修改试用license文件：

WIN XP路径 :
C:\Documents and Settings\All Users\Application Data\Source Insight\4.0
WIN7/WIN8/WIN10路径:
C:\ProgramData\Source Insight\4.0

在上面路径中找到 si4.lic 文件，将 Expiration="2017-XX-XX" 中的2017 改成2030 试用期将永不过期

(任何时候启动，都是显示有效期90天，实际不会过期)

### 配置
* 设置行号
      “View” - “Line numbers”
      or
      空白处右键 - “Line numbers”
* "Options" -> "File type options", "C/C++ Source File"
      Auto Indent，不勾选Indent Open Brace，Indent Close Brace。
      Screen Type 选择隶书
      Type width 输入对应的
      勾选 Visible tabs
* Options -> Preferences -> Files
		勾选 Remove extra white space when saving
* 设置默认GBK编码
    临时：在4.0界面File->Reload as Encoding…->选择gb2312；
    永久：Options->Preferences->Files最下面选择默认编码方式Default Encoding即可，但是需要重建一下你的项目。

### 快捷键
* CTRL+o 文件搜索框
* ALT+l 函数搜索框
* F3、F4 Str搜索上下跳转
