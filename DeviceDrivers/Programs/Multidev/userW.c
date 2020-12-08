#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#define SIZE 16

int main()
{
	int ret,fd;
	char buffW[]="FromUser";
	fd=open("/dev/sample0",O_RDWR);
	if(fd<0)
	{
		perror("Error Opening\n");
		return fd;
	}
	ret=write(fd,buffW,sizeof(buffW));
	printf("Wrote %d bytes\n",ret);
	return 0;
}

