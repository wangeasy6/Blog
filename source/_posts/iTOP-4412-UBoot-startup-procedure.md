---
title: iTOP-4412 UBoot 启动过程梳理
date: 2024-11-15 16:51:00
toc: true
categories:
  - Technology
tags:
  - 嵌入式
---

基于讯为源码（ [iTop4412_uboot_20151119.tar.gz](https://github.com/TOPEET-Develop/iTop4412_uboot_public_merge) ）的 UBoot 启动过程梳理。

* **Board:** iTOP-4412 POP 1G Elite（精英板）
* **Boot Loader:** u-boot-2010.03

<!--more-->

<br/>

### stage 1（汇编）

```assembly
# ENTRY(_start) @ u-boot.lds
_start:                                         # cpu/arm_cortexa9/start.S
    # b reset:                                  # cpu/arm_cortexa9/start.S
    # set the cpu to SVC32 mode and IRQ & FIQ disable
    # cache_init
    # Read booting information
    bl lowlevel_init:                           # board/samsung/smdkc210/lowlevel_init_POP.S
        # PS-Hold high
        bl mem_ctrl_asm_init:                   # cpu/arm_cortexa9/s5pc210/cpu_init_POP.S
        bl system_clock_init                    # board/samsung/smdkc210/lowlevel_init_POP.S
        bl 1f                                   # board/samsung/smdkc210/lowlevel_init_POP.S
        bl uart_asm_init                        # board/samsung/smdkc210/lowlevel_init_POP.S
        bl onenandcon_init                      # board/samsung/smdkc210/lowlevel_init_POP.S
    # get ready to call C functions
    beq after_copy:                             # cpu/arm_cortexa9/start.S
        stack_setup                             # cpu/arm_cortexa9/start.S
        clear_bss                               # cpu/arm_cortexa9/start.S
        clbss_l                                 # cpu/arm_cortexa9/start.S
        ldr pc, _start_armboot                  # cpu/arm_cortexa9/start.S
```

<br/>

### stage 2（C）

```c
// lib_arm/board.c

start_armboot()
{
    // Setup gd
    {   // init_sequence[]
        board_init();
        interrupt_init();
        env_init();
        init_baudrate();
        serial_init();
        console_init_f();
        off_charge();
        display_banner();
        print_cpuinfo();
        checkboard();
        dram_init();
        display_dram_config();
    }
    mem_malloc_init();
    mmc_initialize(gd->bd);
    env_relocate();
    serial_initialize();
    stdio_init();
    jumptable_init();
    enable_interrupts();
    board_late_init();
    recovery_preboot();
    for (;;) {
        main_loop ();
    }
}
```

<br/>

### main loop

```c
// common/main.c

static __inline__ int abortboot(int bootdelay)
{
    int abort = 0;
    printf("Hit any key to stop autoboot: %2d ", bootdelay);

    while ((bootdelay > 0) && (!abort)) {
        int i;

        --bootdelay;
        /* delay 100 * 10ms */
        for (i=0; !abort && i<100; ++i) {
            if (tstc()) {   /* we got a key press   */
                abort  = 1; /* don't auto boot  */
                bootdelay = 0;  /* no more delay    */
                (void) getc();  /* consume input    */
                break;
            }
            udelay(10000);
        }

        printf("\b\b\b%2d ", bootdelay);
    }

    putc('\n');

    return abort;
}

main_loop()
{
    bootdelay = strtol( getenv("bootdelay") );
    s = getenv ("bootcmd");

    if (bootdelay >= 0 && s && !abortboot (bootdelay)) {
        run_command (s, 0);
    }

    for (;;) {
        len = readline (CONFIG_SYS_PROMPT);
        flag = 0;
        if (len > 0)
            strcpy (lastcommand, console_buffer);
        else if (len == 0)
            flag |= CMD_FLAG_REPEAT;    // The enter key repeats the previous command

        run_command (lastcommand, flag);
    }
}
```

<br/>

### run command

```c
// common/command.c

cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len)
{
    cmd_tbl_t *cmdtp;
    for (cmdtp = table; cmdtp != table + table_len; cmdtp++) {
        if (strncmp (cmd, cmdtp->name, len) == 0) {
            return cmdtp;   /* match */
        }
    }
    return NULL;            /* not found or ambiguous command */
}

cmd_tbl_t *find_cmd (const char *cmd)
{
    int len = &__u_boot_cmd_end - &__u_boot_cmd_start;
    return find_cmd_tbl(cmd, &__u_boot_cmd_start, len);
}


// common/main.c

int run_command (const char *cmd, int flag)
{
    line[][] = split(cmd, ";");                     // 伪代码，主要作用是将读到的命令根据;号分行执行
    for (int i=0; i < len(line); i++) {
        parse_line(line[i]);                        // 伪代码，主要作用是将每行命令做解析，获取 参数个数-argc 和 参数列表-argv
        cmdtp = find_cmd(line[i][0]);
        /* found - check max args */
        (cmdtp->cmd) (cmdtp, flag, argc, argv);
    }
}
```

可以看到，`run_command` 的主要过程是在 `&__u_boot_cmd_end - &__u_boot_cmd_start` 之间匹配输入命令的字符串。

在头文件中有如下声明：

```c
// include/command.h

struct cmd_tbl_s {
    char        *name;      /* Command Name         */
    int     maxargs;        /* maximum number of arguments  */
    int     repeatable;     /* autorepeat allowed?      */
    /* Implementation function  */
    int     (*cmd)(struct cmd_tbl_s *, int, int, char *[]);
    char        *usage;     /* Usage message    (short) */
    char        *help;      /* Help  message    (long)  */
};

typedef struct cmd_tbl_s    cmd_tbl_t;

extern cmd_tbl_t  __u_boot_cmd_start;
extern cmd_tbl_t  __u_boot_cmd_end;

#define Struct_Section  __attribute__ ((unused,section (".u_boot_cmd")))

#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
cmd_tbl_t __u_boot_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage}
```

链接文件中又有如下声明：

```c
 // u-boot.lds
 
 __u_boot_cmd_start = .;
 .u_boot_cmd : { *(.u_boot_cmd) }
 __u_boot_cmd_end = .;
```

此声明意思如下：将声明 section 为 `.u_boot_cmd` 的变量都将被存放在 `.u_boot_cmd` 段中，`.u_boot_cmd` 段在 `__u_boot_cmd_start` 和 `__u_boot_cmd_end` 地址之间。

结合头文件声明，我们可以使用 `U_BOOT_CMD` 来"注册"运行命令。以 bdinfo 命令为例：

```c
// cmd_bdinfo.c

int do_bdinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    /* implementation */
}

U_BOOT_CMD(
	bdinfo,	1,	1,	do_bdinfo,
	"print Board Info structure",
	""
);
```

通过编译产生的 `System.map` 可以查到当前 U-Boot 支持的所有命令。

```
c3e40bb4 A __u_boot_cmd_start
c3e40bb4 D __u_boot_cmd_bdinfo
c3e40bcc D __u_boot_cmd_go
c3e40be4 D __u_boot_cmd_reset
c3e40bfc D __u_boot_cmd_bootm
c3e40c14 D __u_boot_cmd_boot
c3e40c2c D __u_boot_cmd_bootd
c3e40c44 D __u_boot_cmd_iminfo
c3e40c5c D __u_boot_cmd_coninfo
c3e40c74 D __u_boot_cmd_echo
c3e40c8c D __u_boot_cmd_bootelf
c3e40ca4 D __u_boot_cmd_bootvx
c3e40cbc D __u_boot_cmd_ext2ls
c3e40cd4 D __u_boot_cmd_ext2load
c3e40cec D __u_boot_cmd_ext2format
c3e40d04 D __u_boot_cmd_ext3format
c3e40d1c D __u_boot_cmd_fastboot
c3e40d34 D __u_boot_cmd_sdfuse
c3e40d4c D __u_boot_cmd_fatload
c3e40d64 D __u_boot_cmd_fatls
c3e40d7c D __u_boot_cmd_fatinfo
c3e40d94 D __u_boot_cmd_fatformat
c3e40dac D __u_boot_cmd_fdt
c3e40dc4 D __u_boot_cmd_help
c3e40ddc D __u_boot_cmd_question_mark
c3e40df4 D __u_boot_cmd_itest
c3e40e0c D __u_boot_cmd_loads
c3e40e24 D __u_boot_cmd_loadb
c3e40e3c D __u_boot_cmd_loady
c3e40e54 D __u_boot_cmd_max8997
c3e40e6c D __u_boot_cmd_mdmup
c3e40e84 D __u_boot_cmd_md
c3e40e9c D __u_boot_cmd_mm
c3e40eb4 D __u_boot_cmd_nm
c3e40ecc D __u_boot_cmd_mw
c3e40ee4 D __u_boot_cmd_cp
c3e40efc D __u_boot_cmd_cmp
c3e40f14 D __u_boot_cmd_crc32
c3e40f2c D __u_boot_cmd_base
c3e40f44 D __u_boot_cmd_loop
c3e40f5c D __u_boot_cmd_mtest
c3e40f74 D __u_boot_cmd_sleep
c3e40f8c D __u_boot_cmd_mmcinfo
c3e40fa4 D __u_boot_cmd_emmc
c3e40fbc D __u_boot_cmd_mmc
c3e40fd4 D __u_boot_cmd_fdisk
c3e40fec D __u_boot_cmd_movi
c3e41004 D __u_boot_cmd_saveenv
c3e4101c D __u_boot_cmd_editenv
c3e41034 D __u_boot_cmd_printenv
c3e4104c D __u_boot_cmd_setenv
c3e41064 D __u_boot_cmd_run
c3e4107c D __u_boot_cmd_source
c3e41094 D __u_boot_cmd_dnw
c3e410ac D __u_boot_cmd_version
c3e410c4 D __u_boot_cmd_imxtract
c3e410dc A __u_boot_cmd_end
```

