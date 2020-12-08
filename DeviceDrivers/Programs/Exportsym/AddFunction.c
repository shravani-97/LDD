#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/init.h>

//MODULE_LICENSE("GPL");

int HelloWorld_type=1;
static int HelloWorld_add(int a,int b);
static int __init hello_init(void)
{
	printk(KERN_ALERT "In Init module.....The module Parameters is %d\n",HelloWorld_type);
	printk(KERN_ALERT "Hello World\n");
	return 0;
}
int HelloWorld_add(int a,int b)
{
	printk(KERN_ALERT "\n In function HelloWorld_add of module...ADD\n");
	return(a+b);
}

EXPORT_SYMBOL(HelloWorld_add);

static void __exit hello_exit(void)
{
	printk(KERN_ALERT " In cleanup module...the module parameter is %d\n",HelloWorld_type);
	printk(KERN_ALERT " Goodbye World\n");
}

module_init(hello_init);
module_exit(hello_exit);
module_param(HelloWorld_type,int,S_IRUGO);
