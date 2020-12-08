#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include "myioctl.h"
#define SIZE 16
#define CMD_1_IO(MAGIC,1)
#define CMD_2_IOW(MAGIC,2,int)
int main()
{
	int fd,ret,status;
	char buffW[]="Fromuser";
	fd=open("/dev/sample0",O_RDWR);
	if(fd<0)
	{
		perror("Error opening\n");
		return fd;
	}
	ret=ioctl(fd,CMD_1);
	ret=write(fd,buffW,sizeof(buffW));
	printf("Wrote %d bytes\n",ret);
	return 0;
}

