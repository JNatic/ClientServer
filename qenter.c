#include<stdio.h>
#include<stdlib.h>
#include "queue.h"
#include "qoperate.h"

main(int argc,char **argv){
int priority;
if(argc!=3){
fprintf(stderr,"How to use: %s message priority\n",argv[0]);
exit(1);}

if((priority=atoi(argv[2]))<=0||priority > MAXPRIOR){
warn("Invalid Priority");
exit(2);}

if(enter(argv[1],priority)<0){
warn("Enter fails");
exit(3);
exit(0);
}
}
