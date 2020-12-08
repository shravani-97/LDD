#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define SIZE 16
int main()
{
	int fd,ret;
	char buffW[]="FromUser";
        fd=open("/dev/sample1",O_RDWR);
	if(fd<0)
	{
		perror("Error opening\n");
		return fd;
	}
	ret=write(fd,buffW,sizeof(buffW));
	printf("Wrote %d byes\n",ret);
	return 0;
}

