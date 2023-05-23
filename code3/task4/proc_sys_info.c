#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/string.h>

static struct task_struct *cycleprint_kthread = NULL;
unsigned long long mem_total;
unsigned long long mem_free;
unsigned long long mem_used;
char info[256];

static void sleep_milli_sec(int n_milli_sec)
{
    long timeout = (n_milli_sec)*HZ / 1000;
    do
    {
        while (timeout > 0)
        {
            timeout = schedule_timeout(timeout);
        }
    } while (0);
}

static int load_kernel_info(void)
{
    struct file *fp_mem;
    loff_t pos;
    char *total_start, *free_start;
    fp_mem = filp_open("/proc/meminfo", O_RDONLY, 0);
    if (IS_ERR(fp_mem))
    {
        printk("Failed to open /proc/meminfo\n");
        return -1;
    }
    pos = 0;
    kernel_read(fp_mem, info, sizeof(info) - 1, &pos);
    info[sizeof(info) - 1] = '\0';
    total_start = strstr(info, "MemTotal:");
    if (total_start)
    {
        sscanf(total_start, "MemTotal: %llu", &mem_total);
        mem_total /= 1024;
    }

    free_start = strstr(info, "MemFree:");
    if (free_start)
    {
        sscanf(free_start, "MemFree: %llu", &mem_free);
        mem_free /= 1024;
    }
    mem_used = mem_total - mem_free;

    filp_close(fp_mem, NULL);
    return 0;
}

static int print_fun(void *data)
{
    unsigned long start_time = jiffies;
    unsigned long current_time, uptime;

    while (!kthread_should_stop())
    {
        current_time = jiffies;
        uptime = (current_time - start_time) * 1000 / HZ;
        printk("System Uptime: %lu milliseconds\n", uptime);
        printk("Memory Info:\n");

        load_kernel_info();

        printk("Total Memory: %llu MB\n", mem_total);
        printk("Free Memory: %llu MB\n", mem_free);
        printk("Used Memory: %llu MB\n", mem_used);

        sleep_milli_sec(3000);
    }
    return 0;
}

static int __init proc_parser_init(void)
{
    cycleprint_kthread = kthread_run(print_fun, NULL, "cycle_print_kthread");
    printk("cycle_print_kthread started\n");
    return 0;
}

static void __exit proc_parser_exit(void)
{
    if (cycleprint_kthread)
    {
        kthread_stop(cycleprint_kthread);
    }
    printk("cycle_print_kthread stopped\n");
}

module_init(proc_parser_init);
module_exit(proc_parser_exit);
MODULE_LICENSE("GPL");
