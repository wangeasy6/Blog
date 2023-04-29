---
title: 项目中的 C
toc: true
categories:
  - Technology
---

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

直接在宏的定义处检查其是否合法，如不合法，使用 #error 中止编译，打印出错误信息。

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

## C++调用C头文件声明

所有使用 C 实现的函数声明都需要使用如下声明包含。

```c
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
```



## const 使用总结



## 回调函数（callback）





## cJSON



## 主模式下的串口协议调试模板





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

