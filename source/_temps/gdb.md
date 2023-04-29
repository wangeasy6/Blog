### GDB 调试
##### 在Linux下的调试工具

### 编译，启动
* 编译的时候带 -g 参数，gdb <exec> 启动
### 默认
* 直接按回车键，默认执行上一条命令
### 显示
* list/l [n]            打印代码（头文件、宏定义等预处理指令默认不显示，默认一次显示10行）
      l [n,m]           打印 n-m 行
      l [func]          显示某个函数（默认会显示该函数前四行）
      l <文件名>:<函数名>
      l <文件名>:<行号>
### 设置断点
* break/b <行号>/<函数名>      设置断点
      info breakpoints/b      显示已设置断点
      disable <行号>/<函数名>  关闭断点
      enable <行号>/<函数名>   打开断点
      delete/d <行号>/<函数名>   删除断点
### 运行
* run/r                 执行程序
      countinue/c       程序在断点停止之后，继续执行
      next/n            执行下一跳语句,显示的当前行是未执行的
      step/s            跳转到函数内部执行
### 打印变量
* print/p         <变量>/<寄存器>               显示变量
      p {var1,var2}     显示多个变量，但是此处变量类型要一致
      p &var1           打印变量的地址
      p \*var1           打印地址的数据值
* display/disp    <变量>/<寄存器>               显示变量，每执行一次都会显示
      disp {var1,var2}  显示多个变量，但是此处变量类型要一致
      undisplay                        删除锁定的变量
### 信号管理
* signal/sign <信号>         向程序发送指定的信号
      SIGHUP | SIGINT | SIGQUIT | SIGILL | SIGKILL | SIGSEGV
      挂起      中断       退出     非法     杀死       段错误
### 多进程调试
* attach <进程号>
### 段错误调试
* ulimit -c 1024  修改core缓冲区大小，默认为0，当程序段错误的时候会产生一个core文件。ulimit -a 查看设置是否成功。
* gdb <程序名> <core>      载入core文件，gdb输出程序加载过程和出错的位置。
      gdb还提供了where、up、down帮助调试core文件。（where显示出错语句的调用过程，up和down沿着出错位置上下查看）

### 内核调试技术
* printk()，不受中断
* /proc     虚拟文件系统
* KDB 调试工具
