---
title: C Linux Time
date: 2023-09-06 21:54:00
toc: true
categories:
  - tech
tags:
  - C
---

在 Linux 中，存在 4 种时间类型，本篇以这 4 种类型为例，总结一下使用 C 语言的获取、计算方法。

同时，存在以下 3 种计算机系统中常见的关于时间的定义：

**UTC**：协调世界时，又称世界标准时间（Universal Time Coordinated）。是最主要的世界时间标准，其以原子时的秒长为基础，在时刻上尽量接近于格林威治标准时间。现行的协调世界时根据国际电信联盟的建议《Standard-frequency and time-signal emissions》(ITU-R TF.460-6)所确定。UTC基于国际原子时，并通过不规则的加入闰秒来抵消地球自转变慢的影响。闰秒在必要的时候会被插入到UTC中，以保证协调世界时（UTC）与世界时（UT1）相差不超过0.9秒。

**GMT**：格林尼治标准时间（旧译格林威治平均时间或格林威治标准时间；英语：Greenwich Mean Time）是指位于英国伦敦郊区的皇家格林尼治天文台的标准时间，因为本初子午线被定义在通过那里的经线。

**CST**：中国标准时间（China Standard Time），北京时间为 UTC+8，即东八区时间。

<!--more-->

<br/>

## 类型

### clock_t

原型：

```c
#include <bits/types.h>
typedef long clock_t;	/* Type of CPU usage counts.  */

#include <bits/time.h>
#define CLOCKS_PER_SEC  ((__clock_t) 1000000)
```

介绍：

`clock_t` 是一个以 微秒 为单位的整型数，用以计数 CPU 运行时间。



### time_t

原型：

```c
#include <bits/types.h>
typedef long time_t;      /* Seconds since the Epoch.  */
```

介绍：

`time_t` 是一个以 秒 为单位的整型数，用以表示自 格林威治时间1970年01月01日00时00分00秒 起至现在的总秒数。



### timeval

原型：

```c
#include <bits/types/struct_timeval.h>

struct timeval
{
    long tv_sec;              /* Seconds.  */
    long tv_usec;             /* Microseconds.  */
};

struct timezone
{
    int tz_minuteswest;         /* Minutes west of GMT.  */
    int tz_dsttime;             /* Nonzero if DST is ever in effect.  */
};
```

介绍：

`timeval` 是一个包含 毫秒 的 格林威治时间。



### tm

原型：

```c
#include <bits/types/struct_tm.h>

/* ISO C 'broken-down time' structure.  */
struct tm
{
  int tm_sec;                   /* Seconds.     [0-60] */
  int tm_min;                   /* Minutes.     [0-59] */
  int tm_hour;                  /* Hours.       [0-23] */
  int tm_mday;                  /* Day.         [1-31] */
  int tm_mon;                   /* Month.       [0-11] */
  int tm_year;                  /* Year - 1900.  */
  int tm_wday;                  /* Day of week. [0-6] */
  int tm_yday;                  /* Days in year.[0-365] */
  int tm_isdst;                 /* Dsylight saving time.         [-1/0/1]*/
};
```

介绍：

`tm`：是一个直接存储年月日的结构。

`broken-down time`：分解时间。

<br/>

## 操作

### 函数原型及说明

#### 睡眠

```c
#include <unistd.h>
unsigned int sleep(unsigned int seconds);
int usleep(unsigned long usec);
```



#### 操作函数

```c
#include <time.h>

// 取得从1970年1月1日至今的秒数
time_t time(time_t *t);

// 返回两个时间相差的秒数（time1 - time2）
double difftime(time_t time1, time_t time2);

// 将 time_t 转换为经过时区转换的时间，以字符串显示
char *ctime(const time_t *timep);

// 将 time_t 转换为没有经过时区转换 struct tm 结构的时间
struct tm* gmtime(const time_t *timep);

// 和gmtime类似，但是它是经过时区转换的时间
struct tm* localtime(const time_t *timep);

// 返回当前距离1970年的秒数和微妙数，后面的tz是时区，一般不用
int gettimeofday(struct timeval *tv, struct timezone *tz);

// 将 struct tm 转换为经过时区转换的时间，以字符串显示
char *asctime(const struct tm* timep);

// 将 struct tm 结构的时间转换为从1970年至今的秒数
time_t mktime(struct tm* timep);
```

<br/>

### clock_t

```c
#include <stdio.h>
#include <time.h>


int main()
{
	printf("Clocks pre sec: %ld\n", CLOCKS_PER_SEC);
	char unit[3] = "ms";
	if (CLOCKS_PER_SEC == 1000000)
		unit[0] = 'u';

	clock_t t1 = clock();
	printf("T1: %ld %s\n", t1, unit);

	clock_t t2 = clock();
	printf("T2: %ld %s\n", t2, unit);

	printf("T2 - T1: %ld %s\n", t2 - t1, unit);
}
```

输出：

```
Clocks pre sec: 1000000
T1: 487 us
T2: 537 us
T2 - T1: 50 us
```

备注：

这个时间差是程序运行的时间差，和真实时间有出入，当 sleep(1) 时，可能程序时间差只有几百微秒。

<br/>

### time_t

```c
#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    time_t t1;
    time(&t1);              /*获取time_t类型的当前时间*/

    sleep(2);
   
    time_t t2 = time(NULL); /*和time(&t1)效果等价，从效率上要差一点，推荐使用time(&t1)*/

    printf("%ld %ld\n", t1, t2);

    printf("%f\n", difftime(t1, t2));	// -2
    printf("%f\n", difftime(t2, t1));	// +2

	char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    struct tm *gm_time = gmtime(&t1);
    printf("gmtime:    %d-%d-%d %d:%d:%d %s %d %d\n",
    	gm_time->tm_year+1900, gm_time->tm_mon+1, gm_time->tm_mday,
		gm_time->tm_hour, gm_time->tm_min, gm_time->tm_sec,
		wday[gm_time->tm_wday], gm_time->tm_yday, gm_time->tm_isdst);

    struct tm *local_time = localtime(&t1);
    printf("localtime: %d-%d-%d %d:%d:%d %s %d %d\n",
    	local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday,
		local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
		wday[local_time->tm_wday], local_time->tm_yday, local_time->tm_isdst);

    printf("%s", ctime(&t1));

    return 0;
}
```

输出：

```
1693842496 1693842498
-2.000000
2.000000
gmtime:    2023-9-4 15:48:16 Mon 246 0
localtime: 2023-9-4 23:48:16 Mon 246 0
Mon Sep  4 23:48:16 2023
```

备注：

gmtime 为 GMT 时间；

localtime、ctime 为调整时区后的 UTC+8 时间。

<br/>

### timeval

```c
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval t1,t2;
    gettimeofday(&t1, NULL);

    sleep(2.8);
    usleep(1000);

    gettimeofday(&t2, NULL);
    
    printf("T1: %ld %ld\n", t1.tv_sec, t1.tv_usec);
    printf("T2: %ld %ld\n", t2.tv_sec, t2.tv_usec);

    double timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    printf("timeuse: %f us\n", timeuse);

    return 0;
}
```

输出：

```
T1: 1693844329 432130
T2: 1693844331 432903
timeuse: 2.300773 us
```

备注：

sleep 的浮点参数会隐式转换为整型，小数部分不起作用。

Linux 中的延时函数，精度低于 2ms。

<br/>

### tm

```c
#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    time_t t1;
    time(&t1);

    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    struct tm *gm_time = gmtime(&t1);
    printf("gmtime:    %d-%d-%d %d:%d:%d %s %d %d\n",
        gm_time->tm_year+1900, gm_time->tm_mon+1, gm_time->tm_mday,
        gm_time->tm_hour, gm_time->tm_min, gm_time->tm_sec,
        wday[gm_time->tm_wday], gm_time->tm_yday, gm_time->tm_isdst);

    printf("gmtime:    %s", asctime(gm_time));

    struct tm *local_time = localtime(&t1);
    printf("localtime: %d-%d-%d %d:%d:%d %s %d %d\n",
        local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday,
        local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
        wday[local_time->tm_wday], local_time->tm_yday, local_time->tm_isdst);

    printf("localtime: %s", asctime(local_time));

    printf("t1: %ld %ld %ld\n", t1, mktime(gm_time), mktime(local_time));

    return 0;
}
```

输出：

```
gmtime:    2023-9-6 13:52:14 Wed 248 0
gmtime:    Wed Sep  6 13:52:14 2023
localtime: 2023-9-6 21:52:14 Wed 248 0
localtime: Wed Sep  6 21:52:14 2023
t1: 1694008334 1694008334 1694008334
```

备注：

两个 struct tm 要比较大小，要先用 mktime 都转为 time_t 之后，再使用 difftime 比较。

<br/>

## 时区操作

### date 查看时间

```shell
$ date -R
Mon, 04 Sep 2023 23:59:27 +0800
```

根据 RFC 2822 format 所示，上面命令输出的 +0800 表示东八区。

### 设置系统时区

```sh
$ tzselect 
Please identify a location so that time zone rules can be set correctly.
Please select a continent, ocean, "coord", or "TZ".
1) Africa							     7) Europe
2) Americas							     8) Indian Ocean
3) Antarctica							     9) Pacific Ocean
4) Asia								    10) coord - I want to use geographical coordinates.
5) Atlantic Ocean						    11) TZ - I want to specify the timezone using the Posix TZ format.
6) Australia
```

使用 tzselect 命令选择时区，选择完成后会输出时区的写法，然后在.profile、.bash_profile 或者 /etc/profile 中设置正确的 TZ 环境变量并导出，重新登陆方可生效。

```
You can make this change permanent for yourself by appending the line
	TZ='Asia/Shanghai'; export TZ
to the file '.profile' in your home directory; then log out and log in again.

Here is that TZ value again, this time on standard output so that you
can use the /usr/bin/tzselect command in shell scripts:
Asia/Shanghai
```

