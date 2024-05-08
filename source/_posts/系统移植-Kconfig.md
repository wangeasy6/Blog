---
title: 系统移植 - Kconfig
date: 2023-11-14 17:43:04
toc: true
categories:
  - Technology
tags:
  - 嵌入式
---

Kconfig 是最初为了 Linux 内核开发的基于选择的配置系统。作为一套先进的编译选项配置系统，不止 Linux 内核项目，U-Boot、BusyBox、Buildroot、KDE 等众多项目也在使用 Kconfig。

大多数用户通过图形菜单界面与 Kconfig 交互，通常通过运行 `make menuconfig` 来运行。在此界面中，用户选择所需要的选项，并保存配置文件，然后将其作为编译过程的输入。

菜单和配置选项由散布在源代码树中的 KConfig 文件定义。Kconfig 文件的格式在 [Kconfig Language](https://www.kernel.org/doc/html/next/kbuild/kconfig-language.html)  中有详细描述。

<!--more-->

<br/>

## 组成

Kconfig 系统实际上由两个部分组成，KConfigCore 和 KConfigGui。

KConfigCore 提供对配置文件本身的访问。包含了以下功能：

* 代码生成：在 XML 文件中描述配置，并使用 kconfig_compiler 生成读取和写入配置条目的类。
* 级联配置文件（全局设置被本地设置覆盖）。
* 可选的 shell 扩展支持。
* 锁定配置选项的能力。

KConfigGui 提供了一种将 交互层 与配置挂钩的方法，这样它们就能从配置中自动初始化，并自动将更改写入到各自的配置文件中。

KConfigGui 提供了众多的交互形式，如下所示：

| make xxx       | explain                                                      |
| -------------- | ------------------------------------------------------------ |
| config         | Update current config utilizing a line-oriented program      |
| nconfig        | Update current config utilizing a ncurses menu-based program |
| menuconfig     | Update current config utilizing a menu-based program         |
| xconfig        | Update current config utilizing a Qt-based frontend          |
| gconfig        | Update current config utilizing a GTK+ based frontend        |
| oldconfig      | Update current config utilizing a provided .config as base   |
| localmodconfig | Update current config disabling modules not loaded           |
| localyesconfig | Update current config converting local mods to core          |
| defconfig      | New config with default from Arch-supplied defconfig         |
| savedefconfig  | Save current config as ./defconfig (minimal config)          |
| allnoconfig    | New config where all options are answered with 'no'          |
| allyesconfig   | New config where all options are accepted with 'yes'         |
| allmodconfig   | New config selecting modules when possible                   |
| alldefconfig   | New config with all symbols set to default                   |
| randconfig     | New config with a random answer to all options               |
| listnewconfig  | List new options                                             |
| olddefconfig   | Same as oldconfig but sets new symbols to their default value without prompting |
| kvmconfig      | Enable additional options for KVM guest kernel support       |
| xenconfig      | Enable additional options for xen dom0 and guest kernel support |
| tinyconfig     | Configure the tiniest possible kernel                        |

具体支持使用 `make help` 查看：

```sh
linux-4.19.296$ make help

Configuration targets:
  config          - Update current config utilising a line-oriented program
  nconfig         - Update current config utilising a ncurses menu based program
  menuconfig	  - Update current config utilising a menu based program
  xconfig         - Update current config utilising a Qt based front-end
  gconfig         - Update current config utilising a GTK+ based front-end
  oldconfig       - Update current config utilising a provided .config as base
  localmodconfig  - Update current config disabling modules not loaded
  localyesconfig  - Update current config converting local mods to core
  defconfig       - New config with default from ARCH supplied defconfig
  savedefconfig   - Save current config as ./defconfig (minimal config)
  allnoconfig	  - New config where all options are answered with no
  allyesconfig	  - New config where all options are accepted with yes
  allmodconfig	  - New config selecting modules when possible
  alldefconfig    - New config with all symbols set to default
  randconfig	  - New config with random answer to all options
  listnewconfig   - List new options
  olddefconfig	  - Same as oldconfig but sets new symbols to their
                    default value without prompting
  kvmconfig       - Enable additional options for kvm guest kernel support
  xenconfig       - Enable additional options for xen dom0 and guest kernel support
  tinyconfig	  - Configure the tiniest possible kernel
  testconfig	  - Run Kconfig unit tests (requires python3 and pytest)
```

其中，`make menuconfig` 应该是使用最多的交互形式，可根据实际情况自行选择。



## 代码调用流程

Kconfig 相关的工具和源码主要位于内核源码中的 `scripts/kconfig/` 下。

以 `make menuconfig` 为例，这句话会调用 `scripts/kconfig/Makefile`

```makefile
menuconfig: $(obj)/mconf
	$< $(silent) $(Kconfig)
```

翻译一下：

```makefile
menuconfig: scripts/kconfig/mconf
	scripts/kconfig/mconf Kconfig
```

.mconf.o.cmd

```sh
cmd_scripts/kconfig/mconf.o := cc -Wp,-MD,scripts/kconfig/.mconf.o.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer    -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE   -c -o scripts/kconfig/mconf.o scripts/kconfig/mconf.c

source_scripts/kconfig/mconf.o := scripts/kconfig/mconf.c

deps_scripts/kconfig/mconf.o := \
    $(wildcard include/config/mode.h) \
    $(wildcard include/config/color.h) \
    $(wildcard include/config/.h) \
  /usr/include/stdc-predef.h \
  /usr/include/ctype.h \
  /usr/include/features.h \
  /usr/include/x86_64-linux-gnu/sys/cdefs.h \
  /usr/include/x86_64-linux-gnu/bits/wordsize.h \
  /usr/include/x86_64-linux-gnu/bits/long-double.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
  /usr/include/x86_64-linux-gnu/bits/types.h \
  /usr/include/x86_64-linux-gnu/bits/timesize.h \
  /usr/include/x86_64-linux-gnu/bits/typesizes.h \
  /usr/include/x86_64-linux-gnu/bits/time64.h \
  /usr/include/x86_64-linux-gnu/bits/endian.h \
  /usr/include/x86_64-linux-gnu/bits/endianness.h \
  /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
  /usr/include/errno.h \
  /usr/include/x86_64-linux-gnu/bits/errno.h \
  /usr/include/linux/errno.h \
  /usr/include/x86_64-linux-gnu/asm/errno.h \
  /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h \
  /usr/include/fcntl.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
  /usr/include/x86_64-linux-gnu/bits/stat.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl2.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/limits.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/syslimits.h \
  /usr/include/limits.h \
  /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
  /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
  /usr/include/x86_64-linux-gnu/bits/local_lim.h \
  /usr/include/linux/limits.h \
  /usr/include/x86_64-linux-gnu/bits/posix2_lim.h \
  /usr/include/x86_64-linux-gnu/bits/xopen_lim.h \
  /usr/include/x86_64-linux-gnu/bits/uio_lim.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
  /usr/include/stdlib.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
  /usr/include/x86_64-linux-gnu/bits/waitflags.h \
  /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
  /usr/include/x86_64-linux-gnu/bits/floatn.h \
  /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
  /usr/include/x86_64-linux-gnu/sys/types.h \
  /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-intn.h \
  /usr/include/endian.h \
  /usr/include/x86_64-linux-gnu/bits/byteswap.h \
  /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
  /usr/include/x86_64-linux-gnu/sys/select.h \
  /usr/include/x86_64-linux-gnu/bits/select.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
  /usr/include/x86_64-linux-gnu/bits/select2.h \
  /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
  /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
  /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
  /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
  /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
  /usr/include/alloca.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib.h \
  /usr/include/string.h \
  /usr/include/strings.h \
  /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
  /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
  /usr/include/signal.h \
  /usr/include/x86_64-linux-gnu/bits/signum.h \
  /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
  /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
  /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
  /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
  /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
  /usr/include/x86_64-linux-gnu/bits/sigaction.h \
  /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
  /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
  /usr/include/x86_64-linux-gnu/sys/ucontext.h \
  /usr/include/x86_64-linux-gnu/bits/sigstack.h \
  /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
  /usr/include/x86_64-linux-gnu/bits/sigthread.h \
  /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
  /usr/include/unistd.h \
  /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
  /usr/include/x86_64-linux-gnu/bits/environments.h \
  /usr/include/x86_64-linux-gnu/bits/confname.h \
  /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
  /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
  /usr/include/x86_64-linux-gnu/bits/unistd.h \
  /usr/include/x86_64-linux-gnu/bits/unistd_ext.h \
  /usr/include/locale.h \
  /usr/include/x86_64-linux-gnu/bits/locale.h \
  scripts/kconfig/lkc.h \
    $(wildcard include/config/prefix.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/y.h) \
  scripts/kconfig/expr.h \
    $(wildcard include/config/config.h) \
  /usr/include/assert.h \
  /usr/include/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
  /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
  /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
  /usr/include/x86_64-linux-gnu/bits/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/stdio2.h \
  scripts/kconfig/list.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/stdbool.h \
  /usr/include/libintl.h \
  scripts/kconfig/lkc_proto.h \
  scripts/kconfig/lxdialog/dialog.h \
  /usr/include/curses.h \
  /usr/include/ncurses_dll.h \
  /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h \
  /usr/include/stdint.h \
  /usr/include/x86_64-linux-gnu/bits/wchar.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
  /usr/include/wchar.h \
  /usr/include/x86_64-linux-gnu/bits/types/wint_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h \
  /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h \
  /usr/include/x86_64-linux-gnu/bits/wchar2.h \
  /usr/include/unctrl.h \
  /usr/include/curses.h \

scripts/kconfig/mconf.o: $(deps_scripts/kconfig/mconf.o)

$(deps_scripts/kconfig/mconf.o):

```

.mconf.cmd

```sh
cmd_scripts/kconfig/mconf := cc  -o scripts/kconfig/mconf scripts/kconfig/mconf.o scripts/kconfig/zconf.tab.o scripts/kconfig/lxdialog/checklist.o scripts/kconfig/lxdialog/util.o scripts/kconfig/lxdialog/inputbox.o scripts/kconfig/lxdialog/textbox.o scripts/kconfig/lxdialog/yesno.o scripts/kconfig/lxdialog/menubox.o  -Wl,-Bsymbolic-functions -lncursesw -ltinfo

```

实际运行时，可以看到以上的编译和启动过程：

```sh
linux-4.19.296$ make menuconfig ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
  UPD     scripts/kconfig/.mconf-cfg
  HOSTCC  scripts/kconfig/mconf.o
  HOSTCC  scripts/kconfig/lxdialog/checklist.o
  HOSTCC  scripts/kconfig/lxdialog/inputbox.o
  HOSTCC  scripts/kconfig/lxdialog/menubox.o
  HOSTCC  scripts/kconfig/lxdialog/textbox.o
  HOSTCC  scripts/kconfig/lxdialog/util.o
  HOSTCC  scripts/kconfig/lxdialog/yesno.o
  HOSTLD  scripts/kconfig/mconf
scripts/kconfig/mconf  Kconfig
```

至此，我们大致知道了，Kconfig 是一个源码位于 `scripts/kconfig`，现编译运行的，基于 [lxdialog](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/scripts/kconfig/lxdialog?id=HEAD)（Linux 维护的一个精简的[dialog](https://invisible-island.net/dialog/)， 基于 [ncurses](https://en.wikipedia.org/wiki/Ncurses#cite_note-ncurses_Raymond-11) 字符图形界面库） 的工具。（所以需要安装依赖 `sudo apt-get install libncurses5-dev`）

对于系统移植来说，源码分析到这个程度已经够了，我们的重点是使用，而对于项目中想要使用这套选择系统的小伙伴可以自行研究。



## 语法

有关配置文件语法的描述，见 [Kconfig Language](https://www.kernel.org/doc/html/next/kbuild/kconfig-language.html)，或 linux 项目中的 `Documentation/kbuild/kconfig-language.txt`。

整理如下：

* 从根目录的 `Kconfig` 文件开始解析

* `#`：单行注释符

* `mainmenu "str"`：声明菜单标题，居中显示在界面的最上方

* `comment "str"`：声明注释，显示在第一行

* `source "dir"`：用于链接其它源文件

* `menu "str"`：声明菜单入口，`endmenu` 声明菜单终止，一般会在其后加上 `# "str"` 注释是哪个入口的结束。通过 `menu` 的多级嵌套，实现子菜单-子子菜单的功能。子选项只有在其父选项可见时可见 。

  ![menuconfig_mainmenu](/resources/porting/menuconfig_mainmenu.png)

* `config` 声明选项，同一选项可以被多次定义，但是每次定义只能包含单个输入提示，类型需要保持一致。具体格式如下：

  ```
  config <宏>
  	<type> "输入提示" [if <条件>]
  	depends on <宏> [宏运算]
  	default <值> [if <条件>]
  	[default <值> if <条件>]
  	[range <symbol> <symbol> [if <expr>]]
  	help
  		<str>
  ```

  * type：类型必须声明。类型可选项：tristate、string、bool、hex、int。输入提示可以显示表示，所以以下两种写法是等价的：

    ```
    	bool "Networking support"
    ```

    and

    ```
    	bool
    	prompt "Networking support"
    ```

  * 在类型前加上 def_，快速声明类型和默认值，语法：`def_bool/def_tristate <value> [if <expr>]`

  * 输入提示可以根据条件显示

  * 依赖关系：可以通过 `depends on`、`select`、`imply` 来表示。A `depends on` B，那么只有在 B 选中才能选 A。`select` 又叫反向依赖，同样的关系可以用 B `select` A 表示。`imply` 是弱反向依赖，用于指示关系，但是依赖项不影响选项的可选项。

  * 选项可以具有任意数量的默认值。如果多个默认值是可见的，默认使用第一个定义的值

  * `tristate`，可以设置为 `y`、`m` 或 `n`。

  * `range` 仅当类型为 int 或 hex 时可用，边界都为闭区间（可相等）

* `menuconfig`：和 `config` 类似， 唯一不同的是该选项除了能设置y/m/n外，还可以实现菜单效果（能回车进入该项内部）。常用格式有2种：

  ```
  menuconfig M
  if M
      config C1
      config C2
  endif
  ```

  or

  ```
  menuconfig M
  config C1
      depends on M
  config C2
      depends on M
  ```

  

* `choice/endchoice`：列表选项。具体格式如下：

  ```
  choice [宏]
  	prompt "提示说明"
  	depends on <宏> [宏运算]
  	default <值> [if <条件>]
  	[default <值> if <条件>]
  	help
  		<str>
  
  config ...
  
  config ...
  
  endchoice
  ```

* `if/endif` 块。具体格式如下：

  ```
  if <expr>
  	...
  endif
  ```

  

## 界面操作

### 选项开头显示说明

| 括号 | 说明                                    |
| ---- | --------------------------------------- |
| [ ]  | 可设置为 内置或不包含                   |
| < >  | 可设置为 内置、模块化、不包含           |
| { }  | 可设置为 内置、模块化（由其它选项选择） |
| - -  | 由其它选项选择                          |

*、M、空格 被包含在括号内，分别代表 内置、模块化、不包含。

括号 ( ) 用于包含 数字/字符 输入，或者作为单选框，(X) 代表被选中。

### 菜单操作说明

| 操作           | 说明                                                         |
| -------------- | ------------------------------------------------------------ |
| 上下方向键     | 上下选中选项                                                 |
| Page Up/Down   | 翻页                                                         |
| Tab/左右方向键 | 在下方选项（`<Select>` `<Exit>` `<Help>` `<Save>` `<Load>`）中进行选择 |
| Enter          | 执行下方选项，默认用于进入（`<Select>`）子选项（带有 `--->`/`----` 的选项） |
| 字母           | 高亮的字母是快捷键（字母相同时会在同一字母选项中滚动），大小写通用 |
| y/Y            | 包含此选项                                                   |
| n/N            | 不包含此选项                                                 |
| m/M            | 模块化                                                       |
| Esc Esc        | 返回                                                         |
| h/?            | 帮助信息                                                     |
| /              | 搜索                                                         |
| 空格           | 在可选项中循环切换。在单选菜单中用于选择                     |

### 帮助界面操作说明

| 操作         | 按键                                                   |
| ------------ | ------------------------------------------------------ |
| 上下左右移动 | 方向键：up/down/left/right 或者使用 VI 操作键：h/j/k/l |
| 上下翻页     | Page Up/Down 或者 b/Space 或者 u/d                     |
| 退出         | E/X/q/Enter/Esc Esc                                    |



## 参考链接

* [Kernel Build System — The Linux Kernel documentation](https://docs.kernel.org/kbuild/index.html)

* [Exploring the Linux kernel: The secrets of Kconfig/kbuild | Opensource.com](https://opensource.com/article/18/10/kbuild-and-kconfig)
* [KConfig - KConfig (kde.org)](https://api.kde.org/frameworks/kconfig/html/)
* [Kconfig 语法 - fluidog - 博客园 (cnblogs.com)](https://www.cnblogs.com/fluidog/p/15176680.html)

