#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define NAME MyCharDevice

int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t  *offp);
ssize_t NAME_read(struct file *filp,  char __user *Ubuff, size_t count, loff_t  *offp);
//int NAME_flush(struct file *filp);

struct file_operations fops =
{
	.owner     = THIS_MODULE,
	.open      = NAME_open,
	.read      = NAME_read,
	.write     = NAME_write,
	.release   = NAME_release,
//	.flush     = NAME_flush,
};

struct cdev *my_cdev;
//Init module
static int __init CharDevice_init(void)
{
	int result;
	int MAJOR,MINOR;
	dev_t Mydev;
	Mydev=MKDEV(255,0);
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\n The major number is %d......The minor number is %d\n",MAJOR,MINOR);
	result=register_chrdev_region(Mydev,1,"MyCharDevice");
	if(result<0)
	{
		printk(KERN_ALERT "\n The region requested for is not obtainable\n");
		return(-1);
	}
	my_cdev = cdev_alloc();
	my_cdev->ops= &fops;

	result=cdev_add(my_cdev,Mydev,1);
	if(result<0)
	{
		printk(KERN_ALERT "\n The CharDevice has not been created....\n");
		unregister_chrdev_region(Mydev,1);
		return(-1);
	}
         return 0;
}

//Cleaup module

void __exit CharDevice_exit(void)
{
	dev_t Mydev;
	int MAJOR,MINOR;
	Mydev=MKDEV(255,0);
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nThe major number is %d......Theminor number is %d\n",MAJOR,MINOR);
	unregister_chrdev_region(Mydev,1);
	cdev_del(my_cdev);
	printk(KERN_ALERT "\n I have ungeistered the stuff that was allocated...Goodbye...\n");
	return;
}

//Open System CAall

int NAME_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\n This is open call....I have nothing to do...bu u have..HAHAHAHA...\n");
	return 0;
}

//Close System Call

int NAME_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\nThis is the release method of my Charecter Driver...Bye Dudes....\n");
	return 0;
}

//Write Functionality

ssize_t NAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[80];
	unsigned long result;
	ssize_t retval;
	//strcpy(Kbuff,Ubuff);
	result=raw_copy_from_user((char *)Kbuff,(char *)Ubuff,count);//get user data
	if(result==0)
	{
		printk(KERN_ALERT "\nMessage from the User....\n>>>> %s <<<< \n",Kbuff);
		printk(KERN_ALERT "\n Data Successfully Written...\n");
	        retval=count;
		return retval;
	}
	else if(result>0)
	{
                printk(KERN_ALERT "\nMessage from the User....\n>>>> %s <<<< \n",Ubuff);
		printk(KERN_ALERT "\nPart Data copied...\n");
		retval=(count-result);
                return retval;
         }
	else
	{
		printk(KERN_ALERT "\n Error Writing data..\n");
		retval=-EFAULT;
		return retval;
	}
}

//read functionality
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[]="This is some date from the kernel to the user.....User,ENJOY....";
	unsigned long result;
	ssize_t retval;
	//strcpy(Kbuff,Ubuff);
	result=raw_copy_to_user((char *)Ubuff,(char *)Kbuff,count);
	if(result==0)
	{
		printk(KERN_ALERT "\n Data successfully read..\n");
		retval=count;
		return retval;
	}
	else if(result > 0)
	{
		printk(KERN_ALERT "\n Part data available..\n");
		retval=(count-result);
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\n Error waiting data to user...\n");
		retval=-EFAULT;
		return retval;
	}
}

int NAME_flush(struct file *filp)
{
	printk("\n This is the close function of the File\n");
        return 0;
}

module_init(CharDevice_init);
module_exit(CharDevice_exit);

		


