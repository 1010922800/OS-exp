//任务三的 kthread_sleep_monitor.c文件
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

struct task_struct *new_thread;
struct task_struct *wake_up_thread;

int myPrintk(void *argc)
{
	while (!kthread_should_stop())
	{
		printk("<<--- kthread %s PID is: %d\n", current->comm, current->pid);
		printk("kthread %s state is: %ld\n", current->comm, current->state);
		printk("wake_up_thread %s PID is :%d\n", wake_up_thread->comm, wake_up_thread->pid);
		printk("wake_up_thread %s state is :%ld --->>\n", wake_up_thread->comm, wake_up_thread->state);
		msleep(2000);
	}
	return 0;
}

static int __init wake_up_process_init(void)
{
	struct timespec current_time;
	long loop_end_ts;
	wake_up_thread = current;
	// create a new thread
	new_thread = kthread_create_on_node(myPrintk, NULL, -1, "new_thread");
	// wake up the new thread and run it
	wake_up_process(new_thread);
	current_time = current_kernel_time();
	loop_end_ts = current_time.tv_sec + 5;
	// make current thread run for 5 seconds
	while (current_time.tv_sec <= loop_end_ts)
	{
		current_time = current_kernel_time();
	}
	// make current thread sleep for some time
	schedule_timeout_uninterruptible(1000 * 5);
	// wake up current thread
	wake_up_process(current);
	return 0;
}

static void __exit wake_up_process_exit(void)
{
	if (new_thread)
	{
		kthread_stop(new_thread);
		printk("kill kthread new_thread\n");
	}
}

module_init(wake_up_process_init);
module_exit(wake_up_process_exit);
