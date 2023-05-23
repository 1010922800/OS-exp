//任务四的proc_cpu.c文件
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/kthread.h>

static struct task_struct *cycleprint_kthread = NULL;
char cpu_load_info[5] = "";
static char *info = "hello, openEuler\n";

static int write_file(void) {
    char buf[20] = "";
    struct file *fp;
    loff_t pos;
    fp = filp_open("/home/kernel_file", O_RDWR | O_CREAT, 0644);
    if (IS_ERR(fp)) {
        printk("create file error\n");
        return -1;
    }
    pos = 0;
    kernel_write(fp, info, strlen(info), &pos);
    pos = 0;
    kernel_read(fp, buf, strlen(info), &pos);
    printk("%s", buf);
    filp_close(fp, NULL);
    return 0;
}

static void sleep_milli_sec(int n_milli_sec) {
    long timeout = (n_milli_sec) * HZ / 1000;
    do {
        while (timeout > 0) {
            timeout = schedule_timeout(timeout);
        }
    } while (0);
}

static int load_kernel_info(void) {
    struct file *fp_cpu;
    loff_t pos;
    char buf_cpu[10];
    fp_cpu = filp_open("/proc/loadavg", O_RDONLY, 0);
    if (IS_ERR(fp_cpu)) {
        printk("open proc file error\n");
        return -1;
    }
    pos = 0;
    kernel_read(fp_cpu, buf_cpu, sizeof(buf_cpu), &pos);
    strncpy(cpu_load_info, buf_cpu, 4);
    filp_close(fp_cpu, NULL);
    return 0;
}

static int print_fun(void *data) {
	//write to file
    write_file();
	//load proc file and print cpu_load info every 3 seconds
    while (!kthread_should_stop()) {
        if (load_kernel_info() != 0) {
            printk("error occurs when loading kernel info!\n");
            return -1;
        }
        printk("current cpu load: %s\n", cpu_load_info);
        sleep_milli_sec(3000);
    }
    return 0;
}

static int __init proc_parser_init(void) {
    cycleprint_kthread = kthread_run(print_fun, NULL, "cycle_print_kthread");
	printk("cycle_print_kthread started\n");
    return 0;
}

static void __exit proc_parser_exit(void) {
    if (cycleprint_kthread) {
        kthread_stop(cycleprint_kthread);
    }
    printk("cycle_print_kthread stopped\n");
}

module_init(proc_parser_init);
module_exit(proc_parser_exit);
MODULE_LICENSE("GPL");
