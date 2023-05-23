#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define BUF_SIZE 20

static struct task_struct *stuIdThread = NULL;
static char stu_id[BUF_SIZE];

static int print(void *data)
{
    int i = 0;
    while (!kthread_should_stop())
    {
        if (i < strlen(stu_id))
        {
            printk("Index %d of student ID : %c.\n", i, *(stu_id + i));
            i++;
            msleep(3000);
        }
        else
        {
            printk("All digits of student ID have been printed\n");
            msleep(5000);
        }
    }
    return 0;
}

static int __init kthread_init(void)
{
    printk("Create kthread stuIdThread.\n");
    stuIdThread = kthread_run(print, NULL, "stuIdThread");
    return 0;
}

static void __exit kthread_exit(void)
{
    printk("Kill kthread stuIdThread.\n");
    if (stuIdThread)
        kthread_stop(stuIdThread);
}

module_param_string(stu_id, stu_id, 20, 0644);

module_init(kthread_init);
module_exit(kthread_exit);