---
title: 项目中的 C
date: 2023-05-08 22:00:00
toc: true
categories:
  - tech
tags:
  - C
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

## C 输入输出格式化

### 输出

```c
// man 3 printf

#include <stdio.h>

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int dprintf(int fd, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

#include <stdarg.h>

int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vdprintf(int fd, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
```

所有适用于 printf 的 format 形式都适用于 fprintf、dprintf、sprintf、snprintf。

#### printf

**原型：**

```c
#include <stdio.h>
// @Return the number of characters printed, and if an error occurs, a negative value is returned
int printf( const char *format, ... );
```



**示例：**

```c
#include <stdio.h>
#include <string.h>

int main()
{
    char c = 'A';
    printf("char: %c\n", c);

    char str[] = "ABC";
    printf("str: %s len: %lu\n", str, strlen(str));
    // strlen() return size_t type, equals 'long unsigned int'
    printf("str(.5): %.*s\n", 5, str);
    printf("ptr: %p\n", str);

    short s_n = -1;
    printf("short: %d\n", s_n);

    unsigned short us_n = -1;
    printf("unsigned short: %d\n", us_n);

    int i_n = 10;
    printf("int: %d\n", i_n);
    printf("int(+): %+d\n", i_n);       // Display positive and negative signs
    printf("int(i): %i\n", i_n);        // Equivalent to %d in printf
    printf("int(4)  : %4i\n", i_n);     // Add space in front
    printf("int(04) : %04i\n", i_n);    // Add 0 in front
    printf("int(-4) : %-4i\n", i_n);    // '-' means left align
    i_n = 020;
    printf("int(o): %o\n", i_n);        // Octal output
    printf("int(#o): %#o\n", i_n);      // Add 0 at the beginning
    i_n = 0xFF00FF;
    printf("int(x): %x\n", i_n);        // Hexadecimal lowercase output
    printf("int(#x): %#x\n", i_n);      // Add 0x at the beginning
    printf("int(X): %X\n", i_n);        // Hexadecimal capital output
    printf("int(#X): %#X\n", i_n);      // Add 0X at the beginning

    int pn = 0;
    printf("already print n: %n", &pn);
    printf("%d\n", pn);

    unsigned int ui_n = 1;
    printf("unsigned int: %u\n", ui_n);

    long l_n = 1234567890l;
    printf("long: %ld\n", l_n);

    unsigned long ul_n = 1234567890ul;
    printf("unsigned long: %lu\n", ul_n);

    long long ll_n = -123450000000ll;
    printf("long long: %lld\n", ll_n);

    unsigned long long ull_n = 123450000000ull;
    printf("unsigned long long: %llu\n", ull_n);

    float f_n = 10.125;
    int width = 8, decimal = 2;
    printf("float: %f\n", f_n);
    printf("float(.2): %.2f\n", f_n);           // Specify the number of decimal places to display
    printf("float(*.*): %*.*f\n", width, decimal, f_n);   // Width and precision are dynamically passed in by parameters
    printf("float(%%%%): %.2f%%\n", f_n);       // Use %% to represent %

    double d_n = 10000000000;
    printf("double: %f\n", d_n);
    printf("int(e): %e\n", d_n);        // Scientific notation, use lowercase "e"
    printf("int(E): %E\n", d_n);        // Scientific notation, use capital "E"
    printf("int(g): %g\n", d_n);        // Use the shorter of %e or %f
    printf("int(G): %G\n", d_n);        // Use the shorter of %E or %f

    long double ld_n = 10000000000;
    printf("long double(f): %Lf\n", ld_n);
    printf("long double(g): %Lg\n", ld_n);
    
    return 0;
}
```

**输出：**

```
char: A
str: ABCDEFG len: 7
str(.5): ABCDE
ptr: 0x7ffeed38cdc0
short: -1
unsigned short: 65535
int: 10
int(+): +10
int(i): 10
int(4)  :   10
int(04) : 0010
int(-4) : 10  
int(o): 20
int(#o): 020
int(x): ff00ff
int(#x): 0xff00ff
int(X): FF00FF
int(#X): 0XFF00FF
already print n: 17
unsigned int: 1
long: 1234567890
unsigned long: 1234567890
long long: -123450000000
unsigned long long: 123450000000
float: 10.125000
float(.2): 10.12
float(*.*):    10.12
float(%%): 10.12%
double: 10000000000.000000
int(e): 1.000000e+10
int(E): 1.000000E+10
int(g): 1e+10
int(G): 1E+10
long double(f): 10000000000.000000
long double(g): 1e+10
```

**格式修饰符：**

- -：左对齐。
- +：输出符号（正数前面加上“+”）。
- #：八进制前缀（0）、十六进制前缀（0x 或 0X）或浮点数小数点（.）。
- 0：用 0 在左侧填充数据输出的空白，而不是默认的空格字符。
- m.n：m 是指定的最小宽度，n 是指定的精度。
- \*：用来接收动态传入的宽度和精度。例如，%\*.\*f 表示输出浮点数，宽度和精度由后面带两个 int 类型的参数动态传入。

### 输入

```c
// man 3 scanf

#include <stdio.h>

int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);

#include <stdarg.h>

int vscanf(const char *format, va_list ap);
int vsscanf(const char *str, const char *format, va_list ap);
int vfscanf(FILE *stream, const char *format, va_list ap);
```



#### scanf

**原型：**

```c
#include <stdio.h>
int scanf( const char *format, ... );
```

scanf()函数根据由 *format*(格式)指定的格式从**stdin**(标准输入)读取,并保存数据到其它参数。

scanf()读取匹配 *format*(格式)字符串的输入。当读取到一个控制字符，它把值放置到下一个变量。空白(tabs，空格等等)会跳过。非空白字符和输入匹配，然后丢弃。如果是一个在%符号和控制符间的数量，那么只有指定数量的字符转换到变量中。如果scanf()遇到一个字符集(用%[]控制字符表示)，那么在括号中的任意字符都会读取到变量中。scanf()的返回值是成功赋值的变量数量,  发生错误时返回EOF。



**示例：**

使用 sscanf 编写示例，可以直接使用 str 作为输入，免于手动输入，更加直观。

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int ret = 0;

    // 读入1个字符
    char c;
    sscanf("c", "%c", &c);
    printf("%c\n", c);              // c

    int i;
    // 读入十进制整数
    sscanf("10", "%d", &i);
    printf("%d\n", i);              // 10

    // 读入八进制整数
    sscanf("10", "%o", &i);
    printf("%d\n", i);              // 8

    // 读入十六进制整数
    sscanf("1A", "%x", &i);         // %x == %X，不区分字母的大小写
    printf("%d\n", i);              // 26

    // 根据字符自行判断 十进制，八进制，十六进制
    sscanf("0x10", "%i", &i);
    printf("0x10(%%i): %d\n", i);   // 0x10(%i): 16

    unsigned int ui;
    sscanf("-1", "%u", &ui);
    printf("%%u: %u\n", ui);         // %u: 4294967295

    // 读入一个字符串，遇空格、制表符或换行符结束
    char str[32] = {0};
    sscanf("1234\t", "%s", str);
    printf("%s\n", str);            // 1234

    // 指定输入个数
    sscanf("1234", "%3d", &i);
    printf("%%3d: %d\n", i);        // %3d: 123

    // 获取已读入字符个数
    sscanf("1234", "%3d%n", &i, &ui);
    printf("%%3d: %d %%n: %d\n", i, ui);        // %3d: 123 %n: 3

    // 输入一个小数
    float f;
    sscanf("1234", "%f", &f);       // %f == %F
    printf("%%f: %g\n", f);         // %f: 1234

    sscanf("1e10", "%E", &f);       // %e == %E，不区分 e 的大小写
    printf("%%E: %g\n", f);         // %E: 1e+10

    sscanf("1e10", "%g", &f);       // %g == %G，不区分 e 的大小写
    printf("%%g: %g\n", f);         // %g: 1e+10

    sscanf("10000000000", "%g", &f);
    printf("%%g: %g\n", f);         // %g: 1e+10

    // 匹配输入字符格式
    int channel, stream;
    ret = sscanf("Channel=1&Stream=2", "Channel=%d&Stream=%d", &channel, &stream);
    // ret = sscanf("", "Channel=%d&Stream=%d", &channel, &stream);     // Error
    if(ret == EOF)                                                      // Error handle
    {
        printf("sscanf error.\n");
        return -1;
    }
    if(ret == 2)
    {
        printf("Channel=%d Stream=%d\n", channel, stream);              // Channel=1 Stream=2
    }

    // 扫描字符集合
    sscanf("Channel=1&Stream=2", "[A-Za-z=]%d[&A-Za-z=]%d",
        &channel, &stream);                                             // 跳过所有 &、a-z、= 的字符
    printf("Channel=%d Stream=%d\n", channel, stream);                  // Channel=1 Stream=2

    // 输入带空格的字符串
    sscanf("I Love You", "%s",str);
    printf("%s\n",str);                     // I
    sscanf("I Love You", "%[^\n]",str);
    printf("%s\n",str);                     // I Love You

    /*
    * m 用于免申请内存的字符输入，可修饰 %s，%c，%[]
    * 使调用者无需申请存放输入的缓冲，而是由 scanf() 去申请缓存，并将缓存的地址赋给指针参数
    * 调用者需要在之后不再需要该缓冲的时候进行 free
    */
    char *p = NULL;
    sscanf("Easy", "%ms", &p);
    printf("%s\n", p);          // Easy
    free(p);

    // 类型修饰符，读取到了指定的最大宽度或读取到不匹配的字符后就会停止
    unsigned short int s_n = -1;
    unsigned long int l_n = -1;
    printf("s_n: %u l_n:%lu\n", s_n, l_n);          // s_n: 65535 l_n:18446744073709551615
    
    sscanf("65536", "%hd%n", &s_n, &ui);
    printf("%%hd: %u %%n: %d\n", s_n, ui);          // %hd: 0 %n: 5
    
    sscanf("18446744073709551616", "%ld%n", &l_n, &ui);
    printf("%%ld: %lu %%n: %d\n", l_n, ui);         // %ld: 9223372036854775807 %n: 20
    
    sscanf("abc", "%hhd%n", &c, &ui);
    printf("%%hhd: %c %%n: %d\n", c, ui);           // %hhd: c %n: 5
    
    sscanf("abc", "%c%n", &c, &ui);
    printf("%%c: %c %%n: %d\n", c, ui);             // %c: a %n: 1

    return 0;
}
```

**格式字符说明：**

%a,%A 读入一个浮点值(仅C99有效) 
%c 读入一个字符
%d 读入十进制整数
%i 读入十进制，八进制，十六进制整数
%o 读入八进制整数
%x,%X 读入十六进制整数
%c 读入一个字符
%s 读入一个字符串，遇空格、制表符或换行符结束。
%f,%F,%e,%E,%g,%G 用来输入实数，可以用小数形式或指数形式输入。
%p 读入一个指针
%u 读入一个无符号十进制整数
%n 至此已读入值的等价字符数
%[] 扫描字符集合
%% 读%符号

**类型修饰符：**

| 类型修饰符 | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| h          | 表明转换符将是d,i,o,u,x,X或n的其中一个，而且其后的参数是一个指向short int或unsigned short int（而不是int）的指针。 |
| hh         | 类似h，但是其后的参数是一个指向signed char 或 unsigned char的指针 |
| j          | 类似h，但是其后的参数是一个指向intmax_t 或 uintmax_t的指针   |
| t          | 类似h，但是其后的参数是一个指向ptrdiff_t的指针               |
| z          | 类似h，但是其后的参数是一个指向size_t的指针                  |
| l          | 1、表明转换符将是d,i,o,u,x,X或n的其中一个，而且其后的参数是一个指向long int或unsigned long int（而不是int）的指针2、表明转换符将是e, f,或g的其中一个，而且其后的参数是一个指向double（而不是float）的指针3、指定两个l，和L一致。 |
| L          | 1、表明转换符将是e, f, 或 g的其中一个，而且其后的参数是一个指向long double的指针2、表明转换符是d, i, o, u,或x的其中一个，而且其后的参数是一个指向long double的指针 |

根据示例，类型修饰符在实现中会有具体的表现，需要在实际环境中测试。

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

程序依赖于系统命令 md5sum。

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

