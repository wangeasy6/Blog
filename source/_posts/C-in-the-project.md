---
title: 项目中的 C
date: 2023-05-08 22:00:00
toc: true
categories:
  - Technology
---

![C-in-the-project.png](/resources/Cover/C-in-the-project.png)

在项目中或者开源代码中，有很多很实用的 C 代码，总结在此，方便复用。

<!--more-->

<br/>

## UNUSED_VARIABLE

项目中对暂未使用的变量使用 UNUSED_VARIABLE (var); 来处理 "warning: unused parameter 'arg' [-Wunused-parameter]" 警告。

```c
#define UNUSED_VARIABLE(var) do { \
    var = var; \
} while(0)
```

## 编译时的参数检查

```c
#define THINGS_NUM 5
#if (THINGS_NUM < 0 || THINGS_NUM > 4)
#error "THINGS_NUM should be between 0 and 4(include 0 and 4)"
#endif
```

直接在宏的定义处检查其是否合法，如不合法，使用 `#error` 中止编译，打印出错误信息。

`#define` 定义一个宏，`#undef` 删除宏定义。

## 开关状态切换替代语句

使用条件运算符在两种状态之间切换：

```c
void onoff() {state = (state == ON) ? OFF : ON; }
```

使用异或和赋值运算替换上述代码：

```c
void onoff() {state ^= 1; }
```

<br/>

## C++调用C头文件声明

`extern "C"` 的作用是让 C++ 编译器将 `extern "C"` 声明的代码当作 C 语言代码处理，可以避免 C++ 因符号修饰导致代码不能和C语言库中的符号进行链接的问题。

在 C++ 中所有使用 C 实现的函数声明都需要使用如下声明包含。

```c
#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
```

<br/>

## const 使用总结

### 作用

1. 修饰变量，说明该变量不可以被改变：`const int a;`
2. 修饰指针，分为：
   - 指向常量的指针（pointer to const）：`const int* p`
   - 自身是常量的指针（常量指针，const pointer）：`int* const p`

### 修饰变量

```c
const int age = 10;
#define age 10;
```

**#define VS const 常量**

| 宏定义 #define         | const 常量     |
| ---------------------- | -------------- |
| 宏定义，相当于字符替换 | 常量声明       |
| 预处理器处理           | 编译器处理     |
| 无类型安全检查         | 有类型安全检查 |
| 不分配内存             | 要分配内存     |
| 存储在代码段           | 存储在数据段   |
| 可通过 `#undef` 取消   | 不可取消       |

### 修饰指针

```c
int a = 10;
const int b = 10;
// b = 20;
// error: assignment of read-only variable 'b'

const int* p1 = &a;   // p可变，*p不可变，此时不能用*p来修改值，但是p可以转向
p1 = &b;
// *p1 = 20;
// error: assignment of read-only location '*p1'

int* const p2 = &a;   // p不可变，*p可变，此时允许*p修改值，但是p不能转向
// p2 = &b;
// error: assignment of read-only variable 'p2'
*p2 = 20;

const int* const p3 = &a; // p不可变，*p也不可变
```

赋值时，const指针可以接受const和非const地址，但是非const指针只能接受非const地址。

```c
const int a = 10;
const int *p1= &a;  	// 合法
// int *p1 = &a;      		// 非法
```

<br/>

## 回调函数（callback）

简单示例：

```c
#include <stdio.h>

typedef int (* operation)(int i,int j);

int plus(int i,int j){
	printf("%s: ",__FUNCTION__ );
	return j+i;
}
int minus(int i,int j){
	printf("%s: ",__FUNCTION__ );
	return i-j;
}

int opr_cb(int i,int j,operation cb){
	return cb(i,j);
}
int main(){
	operation cb = plus;
	printf("%d\n",opr_cb(3,5,cb));
	return 0;
}
```

<br/>

## 使用系统工具获取文件MD5值的C++代码

程序依赖于系统命令md5sum。

```c++
#include <stdlib.h>
#include <fstream>

std::string md5_checksum(std::string file_path, std::string *result)
{
    std::string md5 = "";

    std::ifstream ifs(file_path);
    if( !ifs.good() )
    {
        *result = "file not exist.";
        return md5;
    }

    std::string cmd = "md5sum " + file_path + " | awk '{print $1}' > ./md5tmp";
    system(cmd.c_str());

    std::ifstream fin("md5tmp");
    std::getline(fin, md5);

    std::cout << "Get " << file_path << " MD5:" << md5 << std::endl;
    system("rm md5tmp");

    return md5;
}
```

<br/>

## Monitor 程序模板

```c
/*
* 守护进程
* 功能：用于检测应用程序的运行状态，当应用程序崩溃的时候，重启应用程序。
* 依赖：程序依赖于系统命令 ps 和 grep，不同的版本可能有细小的差别
* Tips: APP_NAME 字符串不能包含于 MONITOR_NAME
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_LEN            128
#define CHECK_TIMES           3
#define LOOP_CHECK_TIME_S     2
#define APP_NAME              "app"
#define MONITOR_NAME          "monitor"

static int run = 1;
static void signal_handler(int sig)
{
    printf("[Monitor] Get signal: %d\n", sig);
    run = 0;
}

int is_exist(const char *file_path, int mode)
{
    if(file_path == NULL)
        return 0;
    if(!(mode != F_OK || mode != X_OK || mode != R_OK || mode != W_OK))
        return 0;
    return !access(file_path, mode);
}


int is_run(char *app_name)
{
    FILE *fp=NULL;
    char buffer[BUFFER_LEN] = {0};

    snprintf(buffer, BUFFER_LEN, "ps -e | grep %s > monitor_tmp", app_name);
    system(buffer);
    memset(buffer, 0x0, BUFFER_LEN);

    fp = fopen("monitor_tmp","r");
    if (fp) {
        memset(buffer, 0, sizeof(buffer));
        fread(buffer, 1, sizeof(buffer), fp);
        fclose(fp);

        // printf("[Monitor] Read:%s\n", buffer);
        if (NULL != strstr(buffer, APP_NAME)) {
            return 1;
        }
    }
    return 0;
}


void restart_app(void)
{
    char restart_cmd[BUFFER_LEN] = {0};
    printf("[Monitor] Restart APP\n");

    if(is_exist(APP_NAME, X_OK)) {
        snprintf(restart_cmd, BUFFER_LEN, "./%s &", APP_NAME);
        system(restart_cmd);
    }
    return ;
}


int main(int argc,char **argv)
{
    int  check_n=1;

    if(is_run(MONITOR_NAME) == 1) {
        printf("[Monitor] Monitor Is Exist. Exit.\n");
        return 0;
    }
    printf("[Monitor] Monitoring...\n");

    signal(SIGINT, signal_handler);

    while (run) {
        if (is_run(APP_NAME) == 0) {              //Check
            printf("[Monitor] Check Main Thread Not Exist: %d\n",check_n++);
            if (check_n > CHECK_TIMES) {
                if(is_exist(APP_NAME, X_OK)) {
                    restart_app();
                } else {
                    printf("[Monitor] Check %s File Not Exist.\n", APP_NAME);
                    check_n = 1;
                }
            }
        } else {
            check_n = 1;
        }

        sleep(LOOP_CHECK_TIME_S);
    }

    printf("[Monitor] Exit.\n");
    return 0;
}
```

