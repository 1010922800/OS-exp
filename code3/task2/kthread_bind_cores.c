#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

static struct task_struct *bind_kthread[4];

static int MyPrintk(void *data)
{
    while (!kthread_should_stop())
    {
        int cpu = get_cpu();
        put_cpu();
        printk("kthread bind_kthread_%d %s is running on cpu %d\n", cpu + 1, current->comm, cpu);
        msleep(2000);
    }
    return 0;
}

static int __init init_kthread(void)
{
    int i;
    int number = num_online_cpus();
    for (i = 0; i < number; i++)
    {
        bind_kthread[i] = kthread_create(MyPrintk, NULL, "bind_kthread_%d", i + 1);
        kthread_bind(bind_kthread[i], i);
        wake_up_process(bind_kthread[i]);
        printk("kthread bind_kthread_%d start\n", i);
    }
    return 0;
}

static void __exit exit_kthread(void)
{
    int i;
    int number = num_online_cpus();
    for (i = 0; i < number; i++)
    {
        if (bind_kthread[i])
        {
            kthread_stop(bind_kthread[i]);
            printk("kill kthread bind_kthread_%d\n", i);
        }
    }
}

module_init(init_kthread);
module_exit(exit_kthread);
MODULE_LICENSE("GPL");
