# SVN 常用命令
## 环境（Windows + TortoiseSVN）

### SVN提交代码流程
* 编写代码 (编码规范)
* 完成自测
* 与最新代码merge
* 上传到自己的分支
* 主管/导师 审查代码
* 合并到 trunk

### 子分支同步主分支，更新自己的代码
* 1、桌面 右键 -> Reop-browser (主分支路径)
* 2、桌面 右键 -> Reop-browser (自己分支路径)；trunk 右键 -> delete
* 3、trunk 右键 -> copy to (自己分支路径)
* 4、保存自己的更改
* 5、update 本地代码
* 6、merge 代码
* 7、提交代码

### 直接切换分支
* 在目录中直接切换分支：switch --relocate svn://100.160.100.113:8000

### 合并分支
* 合并分支，根据主分支修改的log 修改 现在的提交，来查看是否需要上传（直接对比文件夹，避免需要打开双方路径和托动文件）

### 批量操作
* 批量增加：svn status|grep -E "^[?]+" |awk '{print $2}'|xargs svn add
* 批量增加：svn status | grep -E "^[?M]+" |awk '{print $2}'|xargs svn add
* 批量删除：svn status | grep -E "^[!]+"|awk '{print $2}'|xargs svn delete
*（delete 文件不存在：查看文件命名中是否含有空格，有则使用反斜线转义）
* 检查是否处理完：svn status | grep -E "^[!?]+"

### Error
* update: Tree conflict
      删除，重新update
* update：Conflict not set
      Resolve -> OK
      
