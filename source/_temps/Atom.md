# Atom
##### 来源于github上的开源文本编辑器，优点在于拥有众多的开源插件

### 安装
* 软件下载地址，选择相应的版本：https://github.com/atom/atom/releases/tag/v1.2.4

### 插件安装
* 安装有两种方法：
  * 1、直接在软件中Package -> System view -> install.
  * 2、在CMD下，输入apm install <软件名>，例：apm install activate-power-mode。回车出现done，表示成功。、
  * 3、直接在git上下载插件，放到packages目录。
  * 验证：.atom/packages目录下出现相应文件夹。
* 重新加载Atom，快捷键Ctrl+Shift+F5，或者Ctrl+Shift+p之后，输入Window:Reload，选择，回车。重载后插件安装完成。
* apm install失败的情况下：
  * 使用git下载插件包，进入插件包文件夹，npm install 安装


### 插件
* 浏览自己需要的插件:https://atom.io/packages
* default-encoding  自动识别文件编码，解决中午GBK编码需要手动选择问题
* activate-power-mode  酷炫的输入特效
* minimap-plus  代码缩略图
* simplified-chinese-menu  Atom的简体中文语言包
* vim-mode-plus  vim环境包
* pif-view  支持浏览pdf

### 设置
* 编辑设置：File -> Settings -> Editor
  * Soft Wrap 设置文本自动换行
  * Show Invisibales 设置空格、换行等可见
  * Soft Tab + Tab length 设置换行代表空格数（6/4）
  * simplified-chinese-menu 中文菜单

### 快捷键
* Ctrl+Shift+9    打开git
* Ctrl+Shift+p    全局命令
* Ctrl+Shift+F5   重新加载
* Ctrl+Shift+-/=  字体大小调整
