//任务四的proc_parser.c文件
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/kthread.h>

static struct task_struct *cycleprint_kthread = NULL;
struct task_struct *tmp_p;
char tmp_cpu_load[5] = "";
char tmp_mem_total[20] = "";
char tmp_mem_free[20] = ""; 
static char* info = "赵文浩, 23020201153860\n";

static int write_file(void)
{
    char buf[20] = "";
    struct file* fp;
    loff_t pos;
    fp = filp_open("/home/student_info", O_RDWR | O_CREAT, 0644);
    if (IS_ERR(fp))
    {
        printk("Create file error\n");
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
	long timeout = (n_milli_sec)*HZ/1000;
	do {
    while(timeout > 0) {
		    timeout = schedule_timeout(timeout);
    }
	} while(0);
}

static int atoi(char a[], int n) 
{
	int ans = a[0] - '0';
	int i = 1;
	for(;i < n; i++) 
		ans = ans * 10 + (a[i] - '0');
	return ans;
}
static int load_kenerl_info(void) 
{
	struct file *fp_cpu, *fp_mem;
    loff_t pos;
	char buf_cpu[10];
	char buf_mem[100]; 
	int i = 0;
	int j = 0;
	fp_cpu = filp_open("/proc/loadavg", O_RDONLY, 0);
    if (IS_ERR(fp_cpu)) 
	{
        printk("open load file error\n");
        return -1;
    }
	fp_mem = filp_open("/proc/meminfo", O_RDONLY, 0);
    if (IS_ERR(fp_mem)) 
	{
        printk("open mem file error\n");
        return -1;
	}
    pos = 0;
    kernel_read(fp_cpu, buf_cpu, sizeof(buf_cpu), &pos);
	pos = 0;
    kernel_read(fp_mem, buf_mem, sizeof(buf_mem), &pos);
	strncpy(tmp_cpu_load, buf_cpu, 4);
	for(; i <= 99; i++)
	{
		if (buf_mem[i] >= '0' && buf_mem[i] <= '9')
		{	
			tmp_mem_total[j] = buf_mem[i];
			j++;
		}
		if (buf_mem[i] == '\n')
		{
			i++;
			break;
		}
	} 
	for(j = 0; i <= 99; i++)
	{
		if (buf_mem[i] >= '0' && buf_mem[i] <= '9')
		{
			tmp_mem_free[j] = buf_mem[i];
			j++;
		}
		if (buf_mem[i] == '\n')
			break;
	} 	
	filp_close(fp_cpu, NULL);
    filp_close(fp_mem, NULL);
    return 0;
}

static int print_fun(void *data) 
{
	write_file();
	while(!kthread_should_stop()) 
	{
		if(load_kenerl_info() != 0)
		{
			printk("read meminfo or loadarvg file error!\n"); 
			return -1;
		}
		for_each_process(tmp_p) 
		{
			if(tmp_p->state == 0) 
				printk("p name:%s, pid is:%d\n", tmp_p->comm, tmp_p->pid);
		}
		printk("the cpu load is: %s\n", tmp_cpu_load);	
		printk("total memory is: %s KB\n", tmp_mem_total);
		printk("free memory is: %s KB\n", tmp_mem_free);	
		printk("occupy memory is: %d KB\n", atoi(tmp_mem_total, strlen(tmp_mem_total)) - atoi(tmp_mem_free, strlen(tmp_mem_free)));
		sleep_milli_sec(3000);
	}
	return 0;
}

static int __init proc_parser_init(void)
{
	cycleprint_kthread = kthread_run(print_fun, NULL, "cycle_print_kthread");
	return 0;
}

static void __exit proc_parser_exit(void)
{
    if(cycleprint_kthread)
	{
		printk("stop thread: cycle_print_kthread\n");
		kthread_stop(cycleprint_kthread);
	}
	printk("proc_parser exit\n");
}
 
module_init(proc_parser_init);
module_exit(proc_parser_exit);
MODULE_LICENSE("GPL");
