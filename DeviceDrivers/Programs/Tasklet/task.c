#include<linux/module.h>
#include<linux/interrupt.h>

void tasklet_fun(unsigned long data)
{
	printk("Called from tasklet function %lu\n",data);
	return ;
}

struct tasklet_struct demo_tasklet;

static int __init task_init(void)
{
	tasklet_init(&demo_tasklet,tasklet_fun,1);
	//demo_tasklet.func=tasklet_fun;
	tasklet_disable(&demo_tasklet);
	tasklet_schedule(&demo_tasklet);
        return 0;
}

void task_exit(void)
{
	tasklet_enable(&demo_tasklet);
	tasklet_kill(&demo_tasklet);
	return;
}

module_init(task_init);
module_exit(task_exit);
