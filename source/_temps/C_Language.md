# C语言
##### C语言总结

* system()函数中无法执行含有管道的命令，例如：ls -l /temp/dmx_all.ts | awk '{print $5}' > ts_file_size.out
* 查看.so文件中的函数名：nm -D *.so
