#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#define BUFSIZE 20

char *fifo="fifofile";
int main(int argc,char *argv[])
{
int fd;
char buffer[BUFSIZE];

if (mkfifo(fifo,0666)==-1 && errno != EEXIST){
perror("MKFIFO has failed\n");
exit(1);
}
if((fd=open(fifo,O_RDWR))<0)
perror("FIFO fails to open");
for(;;)
{
if(read(fd,buffer,BUFSIZE-1)<0)
perror("FIFO reading failed");
printf("FIFO: Message :%s\n",buffer);
}
exit(0);
}
