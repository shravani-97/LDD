#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	int FileDesc;
	char Ubuff[]="This is the user buffer sending data to the kernel...";
	char Kbuff[100];
	FileDesc=open(" /dev/MyCharDevice",O_RDWR,0777);
	if(FileDesc<0)
	{
		printf("\n Error Opening Device\n");
		exit(1);
	}
	write(FileDesc,Ubuff,sizeof(Ubuff));
	
	read(FileDesc,Kbuff,50);
	
	printf("\n The data read from the kernel is\n>>> %s <<<<\n",Kbuff);
	close(FileDesc);
}
