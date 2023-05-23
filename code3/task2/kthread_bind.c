// 任务二的 kthread_bind.c文件
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

static struct task_struct *bind_kthread = NULL;

static int MyPrintk(void *data)
{
    while (!kthread_should_stop())
    {
        int cpu = get_cpu();
        put_cpu();
        printk("kthread %s is running on cpu %d\n", current->comm, cpu);
        msleep(2000);
    }
    return 0;
}

static int __init init_kthread(void)
{
    bind_kthread = kthread_create(MyPrintk, NULL, "bind_kthread");
    kthread_bind(bind_kthread, 1);
    wake_up_process(bind_kthread);
    printk("kthread bind_kthread start\n");
    return 0;
}

static void __exit exit_kthread(void)
{
    if (bind_kthread)
    {
        kthread_stop(bind_kthread);
        printk("kill kthread bind_kthread\n");
    }
}

module_init(init_kthread);
module_exit(exit_kthread);
MODULE_LICENSE("GPL");