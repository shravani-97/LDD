#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 16

int main()
{
	int fd,ret;
	char buffR[SIZE];
	fd=open("/dev/sample1",O_RDWR);
        if(fd<0)
	{
		perror("Error Opening\n");
		return fd;
	}
	ret=read(fd,buffR,SIZE);
	printf("Got:%s No of bytes Read:%d\n",buffR,ret);
	return 0;
}

