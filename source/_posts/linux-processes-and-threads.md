---
title: Linux 进程与线程
toc: true
date: 2026-06-05 15:22:00
categories:
  - tech
tags:
  - C
  - 嵌入式
---

本篇总结了在 Linux 系统中，进程与线程的基本特性、通信方式以及相应的系统调用接口。

<!--more-->

<br/>

## 多进程 vs. 多线程

### 对比

| 对比维度       | 多进程                                           | 多线程                                                       | 总结     |
| -------------- | ------------------------------------------------ | ------------------------------------------------------------ | -------- |
| 数据共享、同步 | 数据共享复杂，需要用 IPC；数据是分开的，同步简单 | 因为共享进程数据，数据共享简单，但也是因为这个原因导致同步复杂 | 各有优势 |
| 内存、CPU      | 占用内存多，切换复杂，CPU 利用率低               | 占用内存少，切换简单，CPU 利用率高                           | 线程占优 |
| 创建销毁、切换 | 创建销毁、切换复杂，速度慢                       | 创建销毁、切换简单，速度很快                                 | 线程占优 |
| 编程、调试     | 编程简单，调试简单                               | 编程复杂，调试复杂                                           | 进程占优 |
| 可靠性         | 进程间不会互相影响                               | 一个线程挂掉将导致整个进程挂掉                               | 进程占优 |
| 分布式         | 适应于多核、多机分布式                           | 适应于多核分布式                                             | 进程占优 |

### 选择

* 需要频繁创建销毁的优先用线程
* 需要进行大量计算的优先使用线程
* 强相关的处理用线程，弱相关的处理用进程
* 可能要扩展到多机分布的用进程，多核分布的用线程

<br/>

## 进程

### 定义

**进程是对正在运行程序的一个抽象。是系统进行资源分配和调度的一个独立单位。**

- **动态性**：进程是“活的”，它有生命周期（创建、运行、阻塞、终止）。而程序是“死的”，只是一组静态的指令。
- **资源拥有者**：进程拥有独立的地址空间（代码段、数据段、堆、栈）和系统资源（打开的文件、信号量、网络连接等）。
- **独立性**：各个进程之间通常是隔离的，一个进程崩溃一般不会直接影响另一个进程（除非有特殊共享）。

### 为什么需要

1. **更高效**：为了解决单任务系统在等待 I/O 操作时的浪费，进程模型加**多道程序设计**（Multiprograming）被应用和发展，CPU 通过在各进程之间来回切换，大大提高了 CPU 的利用率。
2. **更安全**：隔离地址空间，防互相影响。
3. **更好管理**：对资源进行管理，可记录和回收。

### 进程之间私有和共享的资源

* 共享：代码段，公共数据，进程目录，进程 ID
* 私有：地址空间、堆、栈、全局变量、机器寄存器、文件描述符表

### 实现方式

为了实现进程模型，操作系统维护着一组数据结构，即**进程表**，也称**进程控制块**。包含程序计数器、堆栈指针、内存分配情况、打开文件状态、用户信息、调度信息。

**调度程序**决定应当运行哪个程序、何时运行及它应该运行多长时间。

进程有 3 种状态：运行态、就绪态、阻塞态。

### 管理接口

| 函数名称                 | 函数原型                                                     | 作用简述                                                     |
| ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `fork`                   | `pid_t fork(void)`                                           | 创建一个新的子进程。                                         |
| `vfork`                  | `pid_t vfork(void)`                                          | 创建一个几乎不复制父进程页表的新进程，并挂起父进程，直到子进程调用 `exec` 或 `exit`。用于提升 `fork` 后立刻 `exec` 的效率。 |
| `clone`                  | `int clone(int (*fn)(void *), void *child_stack, int flags, void *arg, ...)` | 比 `fork` 更通用、可定制性更强的进程创建接口。它允许通过 `flags` 参数精细控制资源（如内存、文件系统、信号等）是共享还是复制，是实现线程库（如 POSIX 线程）的核心机制。 |
| `execve`                 | `int execve(const char *filename, char *const argv[], char *const envp[])` | 在当前进程中执行一个可执行文件。通常与 `fork` 一起使用，在子进程中加载并运行全新的程序。 |
| `exit`                   | `void exit(int status)`                                      | 终止进程，并对 `stdio` 缓冲区进行刷新。                      |
| `_exit`                  | `void _exit(int status)`                                     | 终止进程，不刷新 `stdio` 缓冲区。                            |
| `wait`                   | `pid_t wait(int *status)`                                    | 暂停父进程，直到其任一子进程终止。                           |
| `waitpid`                | `pid_t waitpid(pid_t pid, int *status, int options)`         | 等待指定的子进程或设置非阻塞行为。                           |
| `wait3`                  | `pid_t wait3(int *wstatus, int options, struct rusage *rusage)` | 与 `waitpid`类似，但能额外返回子进程的资源使用统计信息。     |
| `wait4`                  | `pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage)` | 与 `waitpid` 类似，但能额外返回子进程的资源使用统计信息。    |
| `nanosleep`              | `int nanosleep(const struct timespec *req, struct timespec *rem)` | 使进程睡眠指定的高精度时间。                                 |
| `getpid`                 | `pid_t getpid(void)`                                         | 获取调用进程的ID。                                           |
| `getppid`                | `pid_t getppid(void)`                                        | 获取父进程的ID。                                             |
| `getpgid`                | `pid_t getpgid(pid_t pid)`                                   | 获取指定进程的进程组ID。                                     |
| `setpgid`                | `int setpgid(pid_t pid, pid_t pgid)`                         | 设置进程组ID，常用于作业控制。                               |
| `getsid`                 | `pid_t getsid(pid_t pid)`                                    | 获取会话领导者进程的ID。                                     |
| `setsid`                 | `pid_t setsid(void)`                                         | 创建一个新会话，调用进程成为新的会话组长。                   |
| `getpriority`            | `int getpriority(int which, int who)`                        | 获取进程、进程组或用户的Nice值。                             |
| `setpriority`            | `int setpriority(int which, int who, int prio)`              | 设置进程、进程组或用户的Nice值。                             |
| `nice`                   | `int nice(int inc)`                                          | 更简单的接口，改变调用进程的Nice值。                         |
| `getrlimit`              | `int getrlimit(int resource, struct rlimit *rlim)`           | 获取系统资源上限。                                           |
| `setrlimit`              | `int setrlimit(int resource, const struct rlimit *rlim)`     | 设置系统资源上限。                                           |
| `capget`                 | `int capget(cap_user_header_t hdrp, cap_user_data_t datap)`  | 获取进程的权能，用于细粒度的权限控制。                       |
| `capset`                 | `int capset(cap_user_header_t hdrp, const cap_user_data_t datap)` | 设置进程的权能，用于细粒度的权限控制。                       |
| `getrusage`              | `int getrusage(int who, struct rusage *usage)`               | 获取进程或其子进程的资源使用情况。                           |
| `prctl`                  | `int prctl(int option, unsigned long arg2, ...)`             | 对进程进行精细的特定控制，如设置名称、安全属性等。           |
| `ptrace`                 | `long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data)` | 用于实现调试器，允许一个进程观察和控制另一个进程的执行。     |
| `sched_setscheduler`     | `int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);` | 设置指定进程的调度策略和参数。                               |
| `sched_getscheduler`     | `int sched_getscheduler(pid_t pid);`                         | 获取指定进程的调度策略。                                     |
| `sched_yield`            | `int sched_yield(void);`                                     | 当前进程主动放弃CPU，进入就绪队列末尾。                      |
| `sched_setparam`         | `int sched_setparam(pid_t pid, const struct sched_param *param);` | 仅设置调度参数，不改变策略。                                 |
| `sched_getparam`         | `int sched_getparam(pid_t pid, struct sched_param *param);`  | 获取调度参数。                                               |
| `sched_get_priority_max` | `int sched_get_priority_max(int policy);`                    | 获取给定调度策略的最大静态优先级。                           |
| `sched_get_priority_min` | `int sched_get_priority_min(int policy);`                    | 获取给定调度策略的最小静态优先级。                           |
| `sched_rr_get_interval`  | `int sched_rr_get_interval(pid_t pid, struct timespec *tp);` | 获取进程在轮转调度下的时间片长度。                           |

<br/>

### 进程间通信

#### 为什么需要

* 有需要多个进程协作完成的任务，它们之间可以通过数据传递或消息同步实现协作。

* 对于系统中共享但使用时独占的资源（如打印机、修改配置文件）进行操作时，需要通信以避免冲突。

* 在某些高级应用中，一个进程可能需要完全控制另一个进程的执行，例如调试器（Debugger）。

#### 侧重点

因为进程地址空间独立，无法像线程那样直接读取对方内存，必须借助**操作系统内核**或**外部介质**。

#### [管道](#管道（pipe）)

* **本质**：内核维护的一块**环形缓冲区**，半双工，借助文件系统接口访问。
* **分类**：Linux 管道相关的接口主要分为三类：[匿名管道（pipe）](#匿名管道（pipe）)、[命名管道（fifo）](#命名管道（fifo）)、[标准流管道 （popen/pclose）](#标准流管道 （popen/pclose）)。
* **优点**：使用简单；天然支持流式数据；父子进程间通信方便。
* **缺点**：只能单向；匿名管道仅限有亲缘关系的进程；缓冲区有限，不适合复杂控制。
* **典型场景**：Shell 命令 `|` 管道传递。

#### [信号量](#信号量（Semaphore）)

* **本质**：是一个**计数器**，可以用来控制多个线程对共享资源的并发访问。[函数接口集合跳转
* **优点**：不传输数据，只做同步与互斥，效率高，可跨进程。
* **缺点**：本身不传数据，必须与其它 IPC 配合使用，使用不当易死锁。
* **典型场景**：控制共享资源数。

#### [信号](#信号（signal）)

* **本质**：是一种内核向进程发送的**异步事件通知机制**，相当于软中断。
* **优点**：开销极小，响应快，适合异常处理（如 `SIGKILL`、`SIGSEGV`）。
* **缺点**：信息量少（仅信号编号），不可靠（可能丢失），处理逻辑受限。只能给所在进程组中的其他进程发送信号。
* **典型场景**：强制 kill 进程。

#### [消息队列](#消息队列（Message Queue）)

* **本质**：是消息的链表，存放在内核中并由消息队列标识符标识，按类型/优先级读写。
* **优点**：可以实现任意进程间的通信；支持结构化消息、异步通信，无需双方同时存在。
* **缺点**：数据拷贝开销大，受内核缓存限制，不适合大数据量或操作频繁的场合。
* **典型场景**：解耦模块，日志。

#### [共享内存](#共享内存（shared memory）)

* **本质**：映射一段能被多个进程访问的内存，免去了数据在内核和用户空间之间的拷贝。
* **优点**：无数据拷贝，速度最快，适合高频大数据交换。
* **缺点**：不提供同步，必须配合信号量/互斥锁，编程复杂度高。
* **典型场景**：视频流传输、大数据。

#### 套接字

* **本质**：一种可用于本地或不同计算机间的基于字节流的通信方式。
* **优点**：
  1. 传输数据为字节级，传输数据可自定义，数据量小效率高
  2. 传输数据时间短，性能高
  3. 适合于客户端和服务器端之间信息实时交互
  4. 可以加密，数据安全性强
* **缺点**：协议栈开销大，编程复杂，性能低于共享内存。
* **典型场景**：分布式系统。

<br/>

## 线程

### 定义

**线程是进程中的一个执行单元，是系统调度和执行的最小单位。**

- **依附于进程**：线程不能独立存在，必须存在于进程内部。
- **轻量级**：线程几乎不拥有系统资源，它只拥有一点必不可少的运行资源（如程序计数器、寄存器集、栈）。 
- **共享资源**：同一进程内的所有线程**共享**该进程的代码段、数据段、堆以及系统资源。

### 为什么需要

1. **并行运行且拥有共享同一地址空间和所有可用数据**。（对于某些应用十分必要，例如 Word 需要对同一个文件实现并行的交互、格式排版、定时保存等功能）
2. **更轻量化**：可以更快地创建、销毁。（10~100倍）
3. **非计算密集型的优势**：如果存在大量的计算和大量的 I/O 处理，多线程可以加快执行速度。
4. **多CPU系统中的优势**：可实现真正的并行。

### 线程之间私有和共享的资源

* 共享：同一进程的：地址空间，堆，全局变量（静态变量），根目录/工作目录，已打开的文件描述符，信号处理函数，报警信号。
* 私有：线程栈，寄存器（程序计数器）。

### 实现方式

线程的实现主要有三种方式，其中**内核级线程（KLT）**是现代通用操作系统（如Windows、Linux、macOS）的主流选择，而**用户级线程（ULT）**则常见于某些高性能并发库或协程实现中。

| **实现方式**          | **核心思想**                                                 | **优点**                                         | **缺点**                                     |
| :-------------------- | :----------------------------------------------------------- | :----------------------------------------------- | :------------------------------------------- |
| **内核级线程 (KLT)**  | **由操作系统内核直接管理**线程的创建、调度和销毁。           | 真正的并行（多核）；一个线程阻塞不影响其他线程。 | 开销大（模式切换）；需要内核支持。           |
| **用户级线程 (ULT)**  | **线程的管理在用户态完成**，内核不知道线程的存在，只看到进程。 | 极快（无需内核态切换）；调度灵活。               | 无法利用多核；一个线程阻塞导致整个进程阻塞。 |
| **混合实现 (Hybrid)** | **用户线程映射到内核线程**，结合了两者的优点。               | 既灵活又可以利用多核。                           | 实现复杂。                                   |

**内核级线程 (Kernel-Level Threads)**

**运行机制**：

- 内核维护**进程控制块（PCB）**和**线程控制块（TCB）**。
- 操作系统内核负责调度这些TCB。对于内核来说，线程是CPU调度的最小单位。
- 应用程序通过系统调用（如 `clone()` 在Linux中，`CreateThread` 在Windows中）来创建线程。

**优点**：

- **多核并行**：内核可以将同一进程的不同线程同时调度到不同的CPU核心上运行。
- **非阻塞**：如果进程中的一个线程被I/O阻塞，内核可以立刻调度该进程下的另一个线程继续运行，或者调度其他进程的线程。

**缺点**：

- **开销较大**：线程的创建、终止和切换都需要进行**用户态到内核态的转换**（模式切换），消耗CPU周期。
- **局限性**：需要操作系统内核提供支持。

**用户级线程 (User-Level Threads)**

**运行机制**：

- 内核只知道进程（PCB），不知道线程。
- 线程的管理（创建、销毁、调度、同步）由一个**用户空间的线程库**（Run-time System）来完成。
- 线程的切换不需要陷入内核，只是在用户态修改程序计数器和栈指针。

**优点**：

- **极速**：切换速度极快，仅相当于一次函数调用。
- **灵活**：调度算法可以由应用程序自己定制。

**缺点**：

- **无法并行**：由于内核只给进程分配一个CPU时间片，所以该进程的所有线程只能轮流占用这一个时间片，**无法利用多核CPU**。
- **脆弱**：如果一个线程发起系统调用（如读取文件）被阻塞，内核会认为整个进程阻塞，导致该进程的所有线程都无法运行。

**混合实现 (Hybrid Model)**

许多现代系统（如某些版本的Solaris，以及现在的很多语言运行时）采用这种方式。

- **机制**：用户空间创建多个用户线程，内核提供多个内核线程。用户线程通过“多路复用”的方式映射到内核线程上。
- **效果**：既减少了内核线程的数量（节省开销），又能在一定程度上实现并行和防止阻塞。

### 管理接口

要使用 `pthread` 库，需要包含头文件 `<pthread.h>`，并在编译时链接 `pthread` 库（例如 `gcc -pthread` 或 `gcc -lpthread`）。

一览表：

| 函数名称                 | 函数原型                                                     | 作用简述                                                     |
| :----------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `pthread_create`         | `int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);` | 创建一个新的线程，新线程从 `start_routine` 函数开始执行。    |
| `pthread_self`           | `pthread_t pthread_self(void);`                              | 获取调用线程自身的线程ID。                                   |
| `pthread_equal`          | `int pthread_equal(pthread_t t1, pthread_t t2);`             | 比较两个线程ID是否相等。                                     |
| `pthread_join`           | `int pthread_join(pthread_t thread, void **rval_ptr);`       | 阻塞调用线程，直到指定的 `thread` 线程终止，并回收其资源。   |
| `pthread_detach`         | `int pthread_detach(pthread_t thread);`                      | 将线程标记为分离状态（detached）。线程终止时会自动释放资源，无需其他线程调用 `pthread_join`。 |
| `pthread_exit`           | `void pthread_exit(void *rval_ptr);`                         | 终止调用线程，并可通过 `rval_ptr` 返回一个值。               |
| `pthread_cancel`         | `int pthread_cancel(pthread_t thread);`                      | 向目标线程发送一个取消请求。                                 |
| `pthread_setcancelstate` | `int pthread_setcancelstate(int state, int *oldstate);`      | 控制当前线程是否响应取消请求。                               |
| `pthread_setcanceltype`  | `int pthread_setcanceltype(int type, int *oldtype);`         | 控制线程的取消方式是**推迟**（到达取消点才响应）还是**异步**（立即响应）。 |
| `pthread_testcancel`     | `void pthread_testcancel(void);`                             | 为线程创建一个取消点。如果线程的取消状态是启用的，调用此函数时便会响应取消请求。 |
| `pthread_cleanup_push`   | `void pthread_cleanup_push(void (*routine)(void*), void *arg);` `void pthread_cleanup_pop(int execute);` | 一对宏，用于在线程退出时自动执行清理操作（如释放锁、内存等）。`push` 添加清理函数，`pop` 移除并可选地执行它。 |
| `pthread_attr_init`      | `int pthread_attr_init(pthread_attr_t *attr);`               | 初始化线程属性对象，并设置为默认值。                         |
| `pthread_attr_destroy`   | `int pthread_attr_destroy(pthread_attr_t *attr);`            | 销毁线程属性对象，释放其占用的资源。                         |
| `pthread_key_create`     | `int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));` | 创建一个新的线程私有数据键（key），所有线程最初都与一个 `NULL` 值关联。 |
| `pthread_key_delete`     | `int pthread_key_delete(pthread_key_t key);`                 | 销毁一个已有的线程私有数据键。                               |
| `pthread_setspecific`    | `int pthread_setspecific(pthread_key_t key, const void *value);` | 为当前线程的某个 `key` 绑定一个值。                          |
| `pthread_getspecific`    | `void *pthread_getspecific(pthread_key_t key);`              | 获取当前线程中与 `key` 关联的值。                            |

<br/>

### 线程间通信

#### 为什么需要

多线程在同一进程了并行干活，不是为了“同时跑”，而是为了“一起把一件事更快做完”。它们要交接数据、要等彼此、要决定谁能改什么，就需要线程间通信。

#### 侧重点

由于线程共享进程的内存空间，通信本身**成本极低**（直接读写全局变量即可），但难点在于**同步（Synchronization）**，防止竞态条件。

#### 共享变量

同一进程下的线程共享数据段、堆内存、打开的资源。所以其主要的通信方式是通过共享变量（静态变量、堆上的变量）搭配锁机制来交换信息。

#### 锁机制

包括：[互斥锁/量（mutex）](#互斥锁/量（mutex）)、[自旋锁（spin lock）](#自旋锁（spin lock）)、[读写锁（reader-writer lock）](#读写锁（reader-writer lock）)、[条件变量（condition）](#条件变量（condition）)。

##### 互斥锁

- **本质**：**保证临界区互斥访问的睡眠锁**，未获取到锁的线程会阻塞休眠。
- **优点**：使用简单，能有效防止数据竞争，自动释放 CPU 资源。
- **缺点**：频繁加锁解锁有上下文切换开销，可能引发优先级反转。
- **典型场景**：保护临界区。

##### 自旋锁

- **本质**：**忙等锁**，未获取到锁的线程会在 CPU 上空转等待。
- **优点**：无上下文切换，响应极快，适合短临界区和中断上下文。
- **缺点**：浪费 CPU 资源，长时间持有会导致严重性能损耗。
- **典型场景**：短时间持锁。

##### 读写锁

- **本质**：**读共享、写独占的锁**，允许多个读者同时访问。
- **优点**：在读多写少场景下并发度高，减少锁竞争。
- **缺点**：实现复杂，写者可能饥饿，锁管理开销大于互斥锁。
- **典型场景**：配置读取。

##### 条件变量

- **本质**：**线程等待特定条件成立的阻塞机制**，必须与互斥锁配合使用。
- **优点**：解决“忙等”问题，实现高效的生产者-消费者模型。
- **缺点**：使用复杂，存在虚假唤醒风险，必须循环检查条件。
- **典型场景**：生产者-消费者模型。

#### [信号量](#信号量（semaphore）)

* **本质**：一个**记录可用资源数量的计数器**，可实现同步与互斥。
* **优点**：功能强大，既可控制并发数，也可用于线程执行顺序控制。
* **缺点**：PV 操作分散难维护，易导致死锁或逻辑错误。
* **典型场景**：控制共享资源数。

#### [信号](#信号（signal）)

* **本质**：**进程级异步事件通知**，线程继承进程的信号处理机制。
* **优点**：开销极小，响应快，适合异常处理（如 `SIGKILL`、`SIGSEGV`）。
* **缺点**：信息量少（仅信号编号），不可靠（可能丢失），处理逻辑受限。只能给所在进程组中的其他进程发送信号。
* **典型场景**：强制 kill 进程。

#### [屏障](#屏障（barrier）)

* **本质**：**让一组线程在某个汇合点全部到达后才继续执行的同步点**。
* **优点**：非常适合并行计算中的分阶段任务同步（如 OpenMP）。
* **缺点**：只能用于固定数量的线程同步，灵活性较差。
* **典型场景**：多线程分阶段计算。

<br/>

## 系统调用接口集合

### 管道（pipe）

Linux 管道相关的接口主要分为三类：匿名管道（pipe）、命名管道（fifo）、标准流管道 （popen/pclose）。

**对比**：

| 特性         | 匿名管道 (`pipe`)        | 命名管道 (`FIFO`)               | 标准流管道 (`popen`)          |
| :----------- | :----------------------- | :------------------------------ | :---------------------------- |
| **适用关系** | 有亲缘关系的进程         | 任意进程（需文件路径访问）      | 当前进程与 Shell 命令         |
| **存在形式** | 内核中的临时对象         | 文件系统中的特殊文件            | 进程间通信的流                |
| **创建方式** | 系统调用 `pipe()`        | 系统调用 `mkfifo()`             | 函数调用 `popen()`            |
| **操作接口** | 文件 I/O：`read`/`write` | 文件 I/O：`open`/`read`/`write` | 标准 I/O 流：`fread`/`fwrite` |
| **主要用途** | 简单的父子进程通信       | 复杂、多进程的本地通信          | 执行并获取 Shell 命令的输出   |

**编程细节**：

- **单向性**：管道是单向的，如需双向通信，必须创建两个管道。
- **阻塞行为**：默认情况下，`read` 会等待数据，`write` 会等待空间。
- **关闭管理**：**必须关闭未使用的管道端口**。例如，在父子进程通信中，一个进程应关闭其不用的读端或写端，以避免意外的阻塞和资源浪费。这通常也是最容易出错的地方。
- **异常情况**：如果所有写端关闭，`read` 会返回 EOF；如果读端关闭后进程仍尝试 `write`，进程会收到 `SIGPIPE` 信号。
- **原子性**：当向管道写入的数据量小于 `PIPE_BUF` 字节时，`write` 是原子的，不会与其他进程的写入交错。

##### 匿名管道（pipe）

是一种无名的、单向的数据通道，通过内核缓冲区实现数据传输。它主要用于**有亲缘关系**的进程间（如父子、兄弟进程）通信，并由`fork`调用继承。

| 函数名称 | 函数原型                               | 作用简述                                                     |
| :------- | :------------------------------------- | :----------------------------------------------------------- |
| `pipe`   | `int pipe(int pipefd[2]);`             | 创建一个匿名管道，成功时返回的 `pipefd` 数组包含两个文件描述符：`pipefd[0]` 用于读，`pipefd[1]` 用于写。 |
| `pipe2`  | `int pipe2(int pipefd[2], int flags);` | 是 `pipe` 的增强版，通过 `flags` 参数（如 `O_CLOEXEC`, `O_DIRECT` 等）对管道设置更多属性，比 `pipe` 更灵活可控。 |

##### 命名管道（fifo）

命名管道通过在文件系统中创建一个特殊的、具有路径名的文件，实现了**无亲缘关系**的进程间通信。进程可以像打开普通文件一样使用`open`来打开它。

| 函数名称 | 函数原型                                         | 作用简述                                                     |
| :------- | :----------------------------------------------- | :----------------------------------------------------------- |
| `mkfifo` | `int mkfifo(const char *pathname, mode_t mode);` | 创建一个命名管道（FIFO）文件。之后使用 `open`、`read`、`write` 等标准文件 I/O 函数进行操作。 |
| `unlink` | `int unlink(const char *pathname);`              | 从文件系统中删除指定的文件，包括 FIFO 文件，使管道连接随之消失。 |

##### 标准流管道 （popen/pclose）

这是一个高级封装，用于执行 Shell 命令并与之通信。它负责创建管道、启动子进程等操作，简化了与 Shell 交互的流程。

| 函数名称 | 函数原型                                              | 作用简述                                                     |
| :------- | :---------------------------------------------------- | :----------------------------------------------------------- |
| `popen`  | `FILE *popen(const char *command, const char *type);` | 创建一个管道，启动 Shell 执行 `command` 命令，并返回一个可用于读 (`"r"`) 或写 (`"w"`) 的 `FILE*` 流。 |
| `pclose` | `int pclose(FILE *stream);`                           | 关闭由 `popen` 打开的流，并等待其启动的进程结束，返回该进程的终止状态 |

##### 相关操作函数

这些是通用的文件操作函数，同样适用于管道描述符。

| 函数名称 | 函数原型                                                | 作用简述                                                     |
| :------- | :------------------------------------------------------ | :----------------------------------------------------------- |
| `read`   | `ssize_t read(int fd, void *buf, size_t count);`        | 从文件描述符 `fd`（管道的读端）读取数据。                    |
| `write`  | `ssize_t write(int fd, const void *buf, size_t count);` | 向文件描述符 `fd`（管道的写端）写入数据。                    |
| `close`  | `int close(int fd);`                                    | 关闭一个文件描述符，如管道读端或写端。                       |
| `fcntl`  | `int fcntl(int fd, int cmd, ... /* arg */);`            | 可对管道文件描述符执行多种控制操作，如设置为非阻塞模式 (`O_NONBLOCK`)。 |

<br/>

### 信号（signal）

信号处理的核心路径是：通过 **`sigemptyset/sigfillset/sigaddset`** 构建信号集，使用 **`sigprocmask`** 屏蔽信号以防竞态，用 **`sigsuspend`** 等待信号，最后用 **`sigaction`** 设置处理逻辑。`sigaction` 是现代 POSIX 标准推荐的处理接口，相较于古老的 `signal` 函数，它能提供更精细的控制和更好的可移植性。

#### POSIX 定义的信号

| 信号    | 说明                                                         |
| ------- | ------------------------------------------------------------ |
| SIGABRT | 终止进程并强制产生核心转储                                   |
| SIGALRM | 警报器超时（由 `alarm()`函数触发定时）                       |
| SIGFPE  | 发生了浮点错误（比如 除 0、浮点溢出等）                      |
| SIGINT  | 终端中断信号（如按下 `Ctrl+C`），默认请求进程终止            |
| SIGKILL | 强制杀死进程的信号（无法被捕获/忽略，收到后立即终止进程）    |
| SIGSEGV | 段错误（进程访问无效内存地址，如空指针解引用、内存越界）     |
| SIGTERM | 请求进程终止的信号（进程可捕获并做清理后退出，“优雅终止”）   |
| SIGCHLD | 子进程状态变化时（终止、停止、继续等），父进程收到此信号     |
| SIGCONT | 继续执行之前被停止（如 `SIGSTOP`）的进程                     |
| SIGSTOP | 停止进程执行（无法被捕获/忽略，进程暂停至收到 `SIGCONT`）    |
| SIGHUP  | 终端断开连接时发送给控制进程；也用于守护进程“重新加载配置”   |
| SIGPIPE | 向已关闭写端的管道/socket写入数据时产生，默认终止进程        |
| SIGUSR1 | 用户自定义信号 1（程序可自由定义用途，如重载配置、触发操作） |
| SIGUSR2 | 用户自定义信号 2（同理，供用户自定义功能使用）               |

#### 信号集操作

| 函数名称      | 函数原型                                            | 作用简述                               |
| :------------ | :-------------------------------------------------- | :------------------------------------- |
| `sigemptyset` | `int sigemptyset(sigset_t *set);`                   | 初始化信号集，使其**不包含任何信号**。 |
| `sigfillset`  | `int sigfillset(sigset_t *set);`                    | 初始化信号集，使其**包含所有信号**。   |
| `sigaddset`   | `int sigaddset(sigset_t *set, int signum);`         | 向信号集中**添加**一个信号。           |
| `sigdelset`   | `int sigdelset(sigset_t *set, int signum);`         | 从信号集中**删除**一个信号。           |
| `sigismember` | `int sigismember(const sigset_t *set, int signum);` | **检查**一个信号是否在指定的信号集中。 |

#### 信号屏蔽与等待

`sigprocmask`用于控制进程的信号屏蔽字（阻塞信号集），`sigsuspend`则是等待信号的原子操作。

| 函数名称      | 函数原型                                                     | 作用简述                                                     |
| :------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `sigprocmask` | `int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);` | 用于**检查或更改**调用进程的信号屏蔽字。参数`how`可以是`SIG_BLOCK`（将`set`中的信号添加到屏蔽集）、`SIG_UNBLOCK`（从屏蔽集中移除`set`中的信号）或`SIG_SETMASK`（将屏蔽字直接设置为`set`）。 |
| `sigpending`  | `int sigpending(sigset_t *set);`                             | 获取调用进程当前**被阻塞的、处于未决状态**的信号集。         |
| `sigsuspend`  | `int sigsuspend(const sigset_t *mask);`                      | 以**原子操作**替换信号掩码并挂起进程，直到捕捉到一个信号。它先将调用线程的信号屏蔽字替换为`mask`，然后挂起执行，直到一个信号递达且其动作是调用信号处理函数或终止进程。此函数常用来解决经典的信号竞态条件问题。 |

`sigprocmask`在多线程程序中行为未定义，应使用`pthread_sigmask`。

#### 信号发送

| 函数名称 | 函数原型                                    | 作用简述                                                     |
| :------- | :------------------------------------------ | :----------------------------------------------------------- |
| `kill`   | `int kill(pid_t pid, int sig);`             | 向**任何进程或进程组**发送信号，是`kill`命令的基础。`pid`参数用于指定目标进程或进程组。 |
| `raise`  | `int raise(int sig);`                       | 向**调用进程自身**发送一个信号。等价于 `kill(getpid(), sig)`。 |
| `killpg` | `int killpg(pid_t pgrp, int sig);`          | 向**指定进程组**中的每个进程发送信号，在Linux中较为常见。    |
| `alarm`  | `unsigned int alarm(unsigned int seconds);` | 设置一个定时器，在`seconds`秒后向调用进程发送`SIGALRM`信号。 |
| `pause`  | `int pause(void);`                          | 挂起调用进程，直到任何信号被捕获。                           |

#### 信号处理

| 函数名称    | 函数原型                                                     | 作用简述                                                     |
| :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `signal`    | `void (*signal(int signum, void (*handler)(int)))(int);`     | 设置一个信号的处理函数。它是一个较老的接口，在不同Unix平台行为有差异，**不建议在可移植程序中使用**。 |
| `sigaction` | `int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);` | 用于**检查或修改**与指定信号相关联的处理动作。它比`signal`更复杂，但功能更强大、行为更统一，是推荐使用的**信号处理接口**。 |

sigaction vs. signal: 为什么推荐使用 sigaction？

| 特性         | `sigaction`                                                  | `signal`                                                   |
| :----------- | :----------------------------------------------------------- | :--------------------------------------------------------- |
| **可移植性** | **POSIX标准**，行为在所有现代Unix-like系统上一致。           | **不推荐**用于可移植程序，其行为在不同Unix系统上可能不同。 |
| **信号掩码** | 可指定信号处理期间自动阻塞的信号集。                         | 不可控，行为因系统而异。                                   |
| **标志位**   | 提供`sa_flags`进行精细控制，如`SA_RESTART`（自动重启被中断的系统调用）等。 | 无。                                                       |
| **信息获取** | 通过`sa_sigaction`可获得发送信号的进程PID等额外信息。        | 只能获取信号编号。                                         |

<br/>

### 消息队列（Message Queue）

Linux 的消息队列主要分为 **System V** 和 **POSIX** 两类。它们的基本思想都是在内核中维护一个消息链表，但函数接口和设计理念有所不同。

####  对比

| 特性                | System V 消息队列                              | POSIX 消息队列                                              |
| :------------------ | :--------------------------------------------- | :---------------------------------------------------------- |
| **主要接口**        | `msgget`, `msgsnd`, `msgrcv`, `msgctl`         | `mq_open`, `mq_send`, `mq_receive`, `mq_close`, `mq_unlink` |
| **队列标识**        | 整型标识符 (`msqid`)                           | 文件描述符 (`mqd_t`)                                        |
| **队列名称**        | 整型键值 (`key_t`)，通常由 `ftok` 生成         | 字符串路径名，必须以 `/` 开头，如 `/myqueue`                |
| **消息优先级/类型** | **按类型 (`msgtyp`) 精确读取**，提供最大灵活性 | **严格按优先级排序**，`mq_receive` 总是返回优先级最高的消息 |
| **异步通知**        | 不支持                                         | 通过 `mq_notify` 支持，可实现事件驱动编程                   |
| **资源清理**        | 无引用计数，需手动调用 `msgctl` 删除           | **引用计数**，所有引用关闭后才真正删除，更安全              |
| **可移植性**        | 极佳，几乎所有 Unix 系统都支持                 | 较好，Linux 2.6.6 及以后的内核支持                          |

 **注意事项**

- **消息的原子性**：发送和接收消息在系统调用内部是原子的，保证了消息内容的完整性。
- **内核限制**：消息队列资源受内核参数限制，如 `msgmax`(单条消息最大字节数)和 `msgmnb`(单个队列最大字节数)。
- **清理管理**：System V 队列必须显式删除，否则会随内核持续存在；POSIX 队列在删除后，也要确保所有描述符都已关闭，资源才能真正释放。

#### System V 消息队列

System V 消息队列是一套经典的 IPC 机制，函数接口相对固定，使用一个整型的标识符 (`msqid`) 来引用队列。

| 函数名称 | 函数原型                                                     | 作用简述                                                     |
| :------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `ftok`   | `key_t ftok(const char *pathname, int proj_id);`             | 根据指定的路径名和项目标识符生成一个 System V IPC 键值 (`key_t`)。 |
| `msgget` | `int msgget(key_t key, int msgflg);`                         | 创建或获取一个消息队列，成功返回队列标识符 (`msqid`)。       |
| `msgsnd` | `int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);` | 将一条消息添加到消息队列中。                                 |
| `msgrcv` | `ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);` | 从消息队列中接收一条消息，成功返回接收到的消息正文长度。     |
| `msgctl` | `int msgctl(int msqid, int cmd, struct msqid_ds *buf);`      | 对消息队列执行控制操作，如查询状态、设置属性或删除队列。     |

**关键特性详解**

- **通过 `msgtyp` 按类型接收**：这是 System V 消息队列最强大的特性。`msgrcv` 函数的 `msgtyp` 参数让你能精确地读取特定消息:
  - `msgtyp == 0`: 返回队列中的第一条消息。
  - `msgtyp > 0`: 返回队列中**类型等于 `msgtyp`** 的第一条消息。
  - `msgtyp < 0`: 返回队列中**类型小于等于 `msgtyp` 绝对值**的消息中，类型值最小的那条。
- **内核持续性**: 消息队列随内核持续存在。即使所有使用它的进程都退出了，队列和其中的消息仍会保留，除非被显式删除或系统重启。

#### POSIX 消息队列

POSIX 消息队列是较新的标准，接口设计更简洁，行为更接近文件操作。

| 函数名称                    | 函数原型                                                     | 作用简述                                   |
| :-------------------------- | :----------------------------------------------------------- | :----------------------------------------- |
| `mq_open`                   | `mqd_t mq_open(const char *name, int oflag, ... /* mode_t mode, struct mq_attr *attr */);` | 创建或打开一个 POSIX 消息队列。            |
| `mq_close`                  | `int mq_close(mqd_t mqdes);`                                 | 关闭一个已打开的消息队列描述符。           |
| `mq_unlink`                 | `int mq_unlink(const char *name);`                           | 根据名称从系统中删除一个消息队列。         |
| `mq_send`                   | `int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);` | 向队列发送一条消息。                       |
| `mq_receive`                | `ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);` | 从队列中接收一条消息。                     |
| `mq_getattr` / `mq_setattr` | `int mq_getattr(mqd_t mqdes, struct mq_attr *attr);` `int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);` | 获取或修改队列的属性。                     |
| `mq_notify`                 | `int mq_notify(mqd_t mqdes, const struct sigevent *sevp);`   | 当队列从空变为非空时，向进程注册异步通知。 |

<br/>

### 共享内存（shared memory）

这套机制主要分为两套实现：经典的 **System V** 和更现代的 **POSIX**。

#### 对比

| 特性         | System V 共享内存                         | POSIX 共享内存                                       |
| :----------- | :---------------------------------------- | :--------------------------------------------------- |
| **接口哲学** | IPC专用接口 (`shmget`, `shmat` 等)        | 基于文件描述符，与`mmap`结合，更像文件操作。         |
| **标识方式** | 使用整数键值 (`key_t`) 和标识符 (`shmid`) | 使用字符串名称 (`/somename`) 和文件描述符。          |
| **大小调整** | 大小在`shmget`时固定，无法后续更改。      | 可通过`ftruncate`随时调整大小，更灵活。              |
| **查看命令** | 使用 `ipcs -m` 命令查看。                 | 在 `/dev/shm` 目录下作为文件存在，用 `ls` 即可查看。 |
| **可移植性** | 历史悠久，几乎在所有UNIX系统上都可用。    | POSIX标准，是现代Linux/UNIX系统的通用方案。          |

**注意事项**

- **竞态管理**：共享内存本身**不提供任何同步机制**。当多个进程并发读写同一块内存时，必须使用额外的同步手段（如**互斥锁、信号量**）来防止数据损坏和竞态条件。
- **系统限制**：System V 共享内存有诸多内核限制，例如单个段的最大大小 (`SHMMAX`)，可通过 `sysctl kernel.shmmax` 查看。
- **清理管理**：System V 共享内存的生命周期通常依附于内核，即使所有进程都退出，它也可能依然存在（除非系统重启或主动调用 `shmctl` 删除）。POSIX 共享内存的对象则存在于 `/dev/shm` 中，不主动 `unlink` 会一直占用内存。
- **资源释放**：使用 `shmdt` 或 `munmap` 只是解除当前进程与内存的关联，并不实际释放内存。必须通过 `shmctl(IPC_RMID)` 或 `shm_unlink` 来真正删除共享内存对象。

#### System V 共享内存

ystem V 共享内存通过 `shmget()` 创建，再用 `shmat()` 附接到进程，是历史悠久、功能完备的方案。

| 函数名称 | 函数原型                                                   | 作用简述                                                     |
| :------- | :--------------------------------------------------------- | :----------------------------------------------------------- |
| `ftok`   | `key_t ftok(const char *pathname, int proj_id);`           | 生成一个System V IPC键值 (`key_t`)，用于唯一标识共享内存段。 |
| `shmget` | `int shmget(key_t key, size_t size, int shmflg);`          | 创建或获取一个共享内存段，成功返回共享内存标识符 (`shmid`)。 |
| `shmat`  | `void *shmat(int shmid, const void *shmaddr, int shmflg);` | 将共享内存段附加（映射）到调用进程的地址空间，返回映射后的内存地址指针。 |
| `shmdt`  | `int shmdt(const void *shmaddr);`                          | 将共享内存段与当前进程分离（detach）。注意，这只是断开关联，并不删除内存段。 |
| `shmctl` | `int shmctl(int shmid, int cmd, struct shmid_ds *buf);`    | 对共享内存段执行控制操作，如获取状态、修改权限、删除段等。   |

#### POSIX 共享内存

POSIX 共享内存遵循“一切皆文件”的哲学，通过 `shm_open()` 创建一个文件描述符，再用 `mmap()` 映射到进程空间，接口更接近普通文件操作。

| 函数名称     | 函数原型                                                     | 作用简述                                                     |
| :----------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `shm_open`   | `int shm_open(const char *name, int oflag, mode_t mode);`    | 创建并打开一个新的，或打开一个已存在的POSIX共享内存对象，返回一个文件描述符。 |
| `ftruncate`  | `int ftruncate(int fd, off_t length);`                       | 设置共享内存对象的大小。新创建的对象大小为0，需要调用此函数调整。 |
| `mmap`       | `void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);` | 将共享内存对象映射到调用进程的地址空间。这是使用共享内存的关键步骤。 |
| `munmap`     | `int munmap(void *addr, size_t length);`                     | 解除进程地址空间中的内存映射。                               |
| `shm_unlink` | `int shm_unlink(const char *name);`                          | 删除一个共享内存对象。当所有进程都解除映射后，对象才会被真正销毁。 |

<br/>

### 互斥锁/量（mutex）

提供了以排他方式防止数据结构被并发修改的方法。

| 函数名称                | 函数原型                                                     | 作用简述                                                 |
| :---------------------- | :----------------------------------------------------------- | :------------------------------------------------------- |
| `pthread_mutex_init`    | `int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);` | 初始化一个互斥锁。                                       |
| `pthread_mutex_lock`    | `int pthread_mutex_lock(pthread_mutex_t *mutex);`            | 锁定互斥锁，如果锁已被占用，则阻塞等待。                 |
| `pthread_mutex_trylock` | `int pthread_mutex_trylock(pthread_mutex_t *mutex);`         | 尝试锁定互斥锁，如果锁已被占用，则立即返回错误，不阻塞。 |
| `pthread_mutex_unlock`  | `int pthread_mutex_unlock(pthread_mutex_t *mutex);`          | 释放调用线程持有的互斥锁。                               |
| `pthread_mutex_destroy` | `int pthread_mutex_destroy(pthread_mutex_t *mutex);`         | 销毁互斥锁，释放其占用的资源。                           |

### 读写锁（reader-writer lock）

允许多个线程同时读共享数据，而对写操作是互斥的。

| 函数名称                 | 函数原型                                                     | 作用简述                                         |
| :----------------------- | :----------------------------------------------------------- | :----------------------------------------------- |
| `pthread_rwlock_init`    | `int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);` | 初始化一个读写锁。                               |
| `pthread_rwlock_rdlock`  | `int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);`       | 以读模式锁定读写锁。                             |
| `pthread_rwlock_wrlock`  | `int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);`       | 以写模式锁定读写锁。                             |
| `pthread_rwlock_unlock`  | `int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);`       | 释放调用线程持有的读写锁（无论是读锁还是写锁）。 |
| `pthread_rwlock_destroy` | `int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);`      | 销毁读写锁，释放其占用的资源。                   |

### 自旋锁（spin lock）

与互斥锁类似，都是为了保护共享资源。互斥锁是当资源被占用，申请者进入睡眠状态；而自旋锁则循环检测保持者是否已经释放锁。

| 函数名称               | 函数原型                                                     | 作用简述                                                     |
| :--------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `pthread_spin_init`    | `int pthread_spin_init(pthread_spinlock_t *lock, int pshared);` | 初始化一个自旋锁。                                           |
| `pthread_spin_lock`    | `int pthread_spin_lock(pthread_spinlock_t *lock);`           | 锁定自旋锁。如果锁已被占用，会忙等待（循环检查），直到获得锁。 |
| `pthread_spin_unlock`  | `int pthread_spin_unlock(pthread_spinlock_t *lock);`         | 释放自旋锁。                                                 |
| `pthread_spin_destroy` | `int pthread_spin_destroy(pthread_spinlock_t *lock);`        | 销毁自旋锁，释放其占用的资源。                               |

### 条件变量（condition）

可以以原子的方式阻塞进程，直到某个特定条件为真为止。对条件的测试是在互斥锁的保护下进行的。条件变量始终与互斥锁一起使用。

| 函数名称                 | 函数原型                                                     | 作用简述                                                     |
| :----------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `pthread_cond_init`      | `int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);` | 初始化一个条件变量。                                         |
| `pthread_cond_wait`      | `int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);` | 原子地解锁互斥锁，并等待条件变量。被唤醒后，会重新锁定互斥锁。 |
| `pthread_cond_timedwait` | `int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);` | 功能与 `pthread_cond_wait` 类似，但可设置等待的超时时间。    |
| `pthread_cond_signal`    | `int pthread_cond_signal(pthread_cond_t *cond);`             | 唤醒一个正在等待该条件变量的线程。                           |
| `pthread_cond_broadcast` | `int pthread_cond_broadcast(pthread_cond_t *cond);`          | 唤醒所有正在等待该条件变量的线程。                           |
| `pthread_cond_destroy`   | `int pthread_cond_destroy(pthread_cond_t *cond);`            | 销毁条件变量，释放其占用的资源。                             |

<br/>

### 信号量（semaphore）

* Linux 信号量主要分为 POSIX 信号量和 System V 信号量两大体系，它们的设计哲学和使用方式有所不同。
* POSIX 信号量是现代 Linux 推荐的接口，其设计符合现代编程习惯，通常更简单易用；System V 信号量是更古老的接口，适用于需要管理一组相关信号量的复杂场景。
* POSIX 信号量又分为**有名**/**无名**两种，主要服务于进程/线程间的同步与互斥。

这是所有信号量的核心操作，无论类型如何，都离不开它们。

| 函数名称        | 函数原型                                                     | 作用简述                                                     |
| :-------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `sem_wait`      | `int sem_wait(sem_t *sem);`                                  | **P 操作**。原子地将信号量值减1，若值为0则阻塞等待，直到信号量大于0。 |
| `sem_trywait`   | `int sem_trywait(sem_t *sem);`                               | P 操作的非阻塞版本。若无法立即减1，会立即返回错误，不会阻塞。 |
| `sem_timedwait` | `int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);` | P 操作的超时版本。若在指定超时时间内仍无法获取资源，则返回错误。 |
| `sem_post`      | `int sem_post(sem_t *sem);`                                  | **V 操作**。原子地将信号量值加1，并唤醒一个正在等待该信号量的线程或进程。 |
| `sem_getvalue`  | `int sem_getvalue(sem_t *sem, int *sval);`                   | 获取信号量的当前值，并通过 `sval` 参数返回。                 |

#### POSIX 有名信号量

有名信号量通过一个类似文件路径的名字在进程间共享，常用于不同进程间的同步。

| 函数名称     | 函数原型                                                     | 作用简述                                                     |
| :----------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `sem_open`   | `sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);` | 创建或打开一个有名信号量。`name` 通常以斜杠'/'开头，如 `"/mysem"`。 |
| `sem_close`  | `int sem_close(sem_t *sem);`                                 | 关闭有名信号量，释放进程与信号量的关联。                     |
| `sem_unlink` | `int sem_unlink(const char *name);`                          | 从系统中删除有名信号量，当所有引用它的进程都关闭后，信号量被销毁。 |

#### POSIX 无名信号量

无名信号量通常用于同一进程内的线程间同步，或者通过共享内存用于相关进程间。

| 函数名称      | 函数原型                                                     | 作用简述                                                     |
| :------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `sem_init`    | `int sem_init(sem_t *sem, int pshared, unsigned int value);` | 初始化一个无名信号量。`pshared=0` 表示线程间共享；非0表示进程间共享，需配合共享内存使用。 |
| `sem_destroy` | `int sem_destroy(sem_t *sem);`                               | 销毁一个无名信号量，释放其占用的资源。                       |

#### System V 信号量

System V 信号量是较早期的IPC机制，其设计更复杂，它以**信号量集**（一个或多个信号量的集合）为单位进行操作。

| 函数名称 | 函数原型                                                   | 作用简述                                                     |
| :------- | :--------------------------------------------------------- | :----------------------------------------------------------- |
| `semget` | `int semget(key_t key, int nsems, int semflg);`            | 创建或获取一个信号量集。`key` 是IPC键，类似文件路径。        |
| `semop`  | `int semop(int semid, struct sembuf *sops, size_t nsops);` | 对一个或多个信号量执行一组操作（PV操作）。`sembuf`结构体定义了操作类型。 |
| `semctl` | `int semctl(int semid, int semnum, int cmd, ...);`         | 对信号量集执行控制操作，如初始化、删除、获取值等。第四个参数根据 `cmd` 变化。 |

<br/>

### 屏障（barrier）

屏障允许每个线程等待，直到所有的合作线程都达到某一点，然后从该点继续执行。

| 函数名称                  | 函数原型                                                     | 作用简述                                                     |
| ------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `pthread_barrier_init`    | `int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned count);` | 初始化一个屏障，并指定需要等待的线程数量 `count`。           |
| `pthread_barrier_wait`    | `int pthread_barrier_wait(pthread_barrier_t *barrier);`      | 调用线程在屏障处等待，直到等待的线程数量达到初始化时指定的 `count`。 |
| `pthread_barrier_destroy` | `int pthread_barrier_destroy(pthread_barrier_t *barrier);`   | 销毁屏障，释放其占用的资源。                                 |

<br/>

## 参考

* 《现代操作系统：原书第 5 版 ... 》 —— 北京：机械工业出版社，2025.3.
* 《The Linux programming interface : a Linux and UNIX system programming handbook / by Michael Kerrisk.》
