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

int main(int argc,char **argv){
int fd,j,nwrite;
char buffer[BUFSIZE];
if(argc <2 ){
perror("how to use: ./fifosend 'message1' 'message2' ....\n");
exit(1);
}
if((fd=open(fifo, O_WRONLY | O_NONBLOCK)) <0)
perror("FIFO open fail");
for(j=1;j<argc;j++){
if(strlen(argv[j])>BUFSIZE){
printf("Message size is out of bound %s\n", argv[j]);
continue;
}
strcpy(buffer,argv[j]);
if((nwrite=write(fd,buffer,BUFSIZE-1))==-1)
perror("Fifo fail to write\n");
}
exit(0);
}
