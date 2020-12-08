#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/semaphore.h>
#include "myioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DD");

#define SIZE 32
#define MAJORN 42
#define NMINORS 3

static dev_t devnum;
struct mydevice
{
	struct cdev _cdev;
	struct semaphore _sem;
	struct circ_buf cbuf;
	wait_queue_head_t _wq;
}_devs[NMINORS];

static int sample_open(struct inode *inodep,struct file *filep)
{
	struct mydevice *_devp=container_of(inodep->i_cdev,struct mydevice,_cdev);
	filep->private_data=_devp;
	printk("Sample Open Funtion\n");
	return 0;
}

static int sample_close(struct inode *inodep,struct file *filep)
{
	printk("Sample Close Function\n");
	return 0;
}

static ssize_t sample_read(struct file *filep,char __user *ubuff,size_t cnt,loff_t *offset)
{
	int i,ret;
	int mini;
	struct mydevice *_devp=filep->private_data;
	wait_event_interruptible(_devp->_wq,CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE)>0);
	mini=min(cnt,(size_t)CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE));
	printk("Size from userspace for read:%d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret=copy_to_user(ubuff+i,_devp->cbuf.buf+_devp->cbuf.tail,1);
                 if(ret)
		 {
			 printk("Error copying to user\n");
				 return -EFAULT;
		 }
		 printk("copied %c to user\n",_devp->cbuf.buf[_devp->cbuf.tail]);
		 _devp->cbuf.tail=(_devp->cbuf.tail+1)&(SIZE-1);
	}
	return i;
}

static ssize_t sample_write(struct file *filep,const char __user *ubuff,size_t cnt,loff_t *offset)
{
	int ret,i;
	struct mydevice *_devp=filep->private_data;
	down_interruptible(&_devp->_sem);
	printk("Size sent from userspace %d\n",(int)cnt);
	for(i=0;i<cnt;i++)
	{
		ret=copy_from_user(_devp->cbuf.buf+_devp->cbuf.head,ubuff+i,1);
		if(ret)
		{
			printk("Error Copying\n");
			return -EFAULT;
		}
		msleep(500);
		printk("Copied from USER %c\t",_devp->cbuf.buf[_devp->cbuf.head]);
		_devp->cbuf.head=(_devp->cbuf.head+1)&(SIZE-1);
	}
	up(&_devp->_sem);
	wake_up(&_devp->_wq);
	return i;
}
long sample_ioctl(struct file *filep,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case CMD_1:
		   printk("This is IOCTL CMD_1\n");
		   break;
		case CMD_2:
		   printk("This is IOCTL CMD_2\n");
		   break;
		default:
		   printk("Not Supported\n");
		   return -ENOTTY;
	}
	return 0;
}
struct file_operations fops=
{
	.open=sample_open,
	.release=sample_close,
	.read=sample_read,
	.write=sample_write,
	.unlocked_ioctl=sample_ioctl,
};

static int __init sample_init(void)
{
	int i,j,ret;
	dev_t tmpdev;
	devnum=MKDEV(MAJORN,0);
	ret=register_chrdev_region(devnum,NMINORS,"sample_dev");
	if(ret)
	{
		printk("Kernel didn't grant us device number\n");
		goto reg_out;
	}
	for(i=0;i<NMINORS;i++)
	{
		cdev_init(&_devs[i]._cdev,&fops);
		_devs[i].cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
		if(!_devs[i].cbuf.buf)
		printk("Memory is not allocated\n");
		goto cbuf_out;
	}
	sema_init(&_devs[i]._sem,1);
	init_waitqueue_head(&_devs[i]._wq);
}
for(j=0;j<NMINORS;j++)
{
	tmpdev=MKDEV(MAJOR(devnum),MINOR(devnum)+j);
	ret=cdev_add(&_devs[j]._cdev,tmpdev,1);
	if(ret)
	{
		printk("Unable to add cdev to kernel\n");
		goto cdev_out;
	}
}
printk("Done init\n");
return 0;

cdev_out:
  for(--j;j>=0;j--)
  cdev_del(&_devs[j]._cdev);
cbuf_out:
  for(--i;i>=0;i--)
  kfree(_devs[i].cbuf.buf);
reg_out:
  unregister_chrdev_region(devnum,NMINORS);
  return -1;
}

static void __exit sample_exit(void)
{
	int i;
	for(i=0;i<NMINORS;i++)
	{
		cdev_del(&_devs[i]._cdev);
		kfree(_devs[i].cbuf.buf);
	}
	unregister_chrdev_region(devnum,NMINORS);
	printk("Good Bye\n");
}

module_init(sample_init);
module_exit(sample_exit);
  
