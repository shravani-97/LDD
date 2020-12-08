#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<linux/sched.h>
#include<linux/wait.h>

#define SIZE 32

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DD");

static dev_t devnum;
static struct cdev _cdev;
struct  circ_buf cbuf;
static wait_queue_head_t _wq;

static int sample_open(struct inode *inodep,struct file *filep)
{
	printk("Sample Open function\n");
	return 0;
}
static int sample_close(struct inode *inodep,struct file *filep)
{
	printk("Sample Close function\n");
	return 0;
}
static ssize_t sample_read(struct file *filep,char __user *ubuff,size_t cnt,loff_t *offset)
{
	int i,ret;
        int mini;
        wait_event_interruptible(_wq,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)>0);
        mini=min(cnt,(size_t)CIRC_CNT(cbuf.head,cbuf.tail,SIZE));

	printk("size from user space for read:%d\n",(int)cnt);
        for(i=0;i<mini;i++)
	{
		ret =copy_to_user(ubuff+i,cbuf.buf+cbuf.tail,1);
                if(ret)
		{
			printk("Error Copying to user\n");
			return -EFAULT;
		}
		printk("Copied %c to use\n",ubuff[i]);
		cbuf.tail=(cbuf.tail+1)&(SIZE-1);
	}
	return i;
}
static ssize_t sample_write(struct file *filep,const char __user *ubuff,size_t cnt,loff_t *offset)
{
	int i,ret;
	printk("size sent from user space %d\n",(int)cnt);
	for(i=0;i<cnt;i++)
	{
		ret=copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1);
		if(ret)
		{
			printk("error copying\n");
			return -EFAULT;
		}
		printk("copied from user %c\t",cbuf.buf[cbuf.head]);
		cbuf.head=(cbuf.head+1)&(SIZE-1);
	}
	return i;
}
struct file_operations fops=
{
	.open=sample_open,
	.release=sample_close,
	.read=sample_read,
	.write=sample_write,
};
static int __init sample_init(void)
{
	int ret;
	devnum=MKDEV(42,0);
	ret=register_chrdev_region(devnum,1,"sample_dev");
	if(ret)
	{
		printk("kernel didn't grant us device num\n");
		return ret;
	}
	cdev_init(&_cdev,&fops);
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
	if(!cbuf.buf)
	{
		printk("Memory is not allocated\n");
		unregister_chrdev_region(devnum,1);
		return -1;
	}
	ret=cdev_add(&_cdev,devnum,1);
	if(ret)
	{
		printk("unable to add cdev to kernel\n");
		kfree(cbuf.buf);
		unregister_chrdev_region(devnum,1);
		return ret;
	}
	printk("Done Init\n");
		return 0;
}
static void __exit sample_exit(void)
{
	cdev_del(&_cdev);
	kfree(cbuf.buf);
	unregister_chrdev_region(devnum,1);
	printk("Good Bye\n");
}

module_init(sample_init);
module_exit(sample_exit);

