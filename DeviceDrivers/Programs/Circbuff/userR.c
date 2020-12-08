#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define SIZE 16
int main()
{
	int fd,ret;
	char buffR[SIZE];

	fd=open("/dev/sample0",O_RDWR);
	if(fd<0)
	{
		perror("Error opening\n");
		return fd;
	}
	ret=read(fd,buffR,SIZE);
	printf("Got:%s No of bytes read:%d\n",buffR,ret);
	return 0;
}

