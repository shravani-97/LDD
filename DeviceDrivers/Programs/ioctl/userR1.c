#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define SIZE 16

int main()
{
	int fd,ret;
	char buffR[SIZE];
	fd=open("/dev/sample1",O_RDWR);
	if(fd<0)
	{
		perror("Error opening\n");
		return fd;
	}
	ret=read(fd,buffR,SIZE);
	printf("Got:%s No of butes read:%d\n",buffR,ret);
	return 0;
}
