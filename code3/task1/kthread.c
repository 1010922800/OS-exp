//任务一的 kthread.c文件代码

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define BUF_SIZE 20

static struct task_struct *myThread = NULL;

static int print(void *data)
{
	while(!kthread_should_stop()){
		printk("kthread myThread is running.");
		msleep(2000);
	}
	return 0;
}

static int __init kthread_init(void)
{
	printk("Create kthread myThread.\n");
	myThread = kthread_run(print, NULL, "myThread");
	return 0;
}

static void __exit kthread_exit(void)
{
	printk("Kill kthread myThread.\n");
	if(myThread)
		kthread_stop(myThread);
}

module_init(kthread_init);
module_exit(kthread_exit);