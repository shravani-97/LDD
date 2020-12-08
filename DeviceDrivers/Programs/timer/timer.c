#include<linux/module.h>
#include<linux/timer.h>
#include<linux/sched.h>
#include<linux/delay.h>

static struct timer_list ktimer;

void timer_callback(struct timer_list *t)
{
	int ret;
	printk("In timer callback\n");
	mdelay(1000);
//	ret=mod_timer(&ktimer,jiffies + msecs_to_jiffies(3000));
}

static int __init timerk_init(void)
{
	int retval;
	printk("Current process is %s\n",current->comm);
	timer_setup(&ktimer,timer_callback,0);
	retval=mod_timer(&ktimer,jiffies +msecs_to_jiffies(5000));
	if(retval)
		printk("Timer firing failed\n");
	return 0;
}

static void timerk_exit(void)
{
	int retval;
	retval=del_timer_sync(&ktimer);
	if(retval)
		printk("The timer in use..\n");
	return;
}

module_init(timerk_init);
module_exit(timerk_exit);

