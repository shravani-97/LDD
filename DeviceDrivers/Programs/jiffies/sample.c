#include<linux/module.h>
#include<linux/sched.h>
#include<linux/jiffies.h>

static __init int jif_init(void)
{
	unsigned long future_jiffy;
	future_jiffy = jiffies + HZ*5;
	printk("Current val is %lu and future val %lu\n",jiffies,future_jiffy);
	while(time_before(jiffies,future_jiffy))
	{
		schedule();
	}
	printk("Current val is %lu and future val %lu\n",jiffies,future_jiffy);
	return 0;
}

static void __exit jif_exit(void)
{
	printk("End\n");
}

module_init(jif_init);
module_exit(jif_exit);
