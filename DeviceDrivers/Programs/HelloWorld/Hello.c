#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");

int Hello_type=1;
module_param(Hello_type,int,S_IRUGO);

static int __init helloworld_init(void)
{
	printk(KERN_ALERT "In Init module...The module parameter is %d\n",Hello_type);
	printk(KERN_ALERT "Hello World\n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	printk(KERN_ALERT "In cleanup module.....The module parameter is %d\n",Hello_type);
	printk(KERN_ALERT "Good bye,world\n");

}

module_init(helloworld_init);
module_exit(helloworld_exit);

