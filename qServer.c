#include<stdio.h>
#include "queue.h"
#include "qoperate.h"
#include <stdlib.h>

int main(){
pid_t pid;
switch(pid=fork()){
case 0:
serve();
break;
case -1:
warn("Fork error");
break;
default:
printf("PID for parent is %d\n",pid);
}
exit(pid!= -1 ? 0 : 1);
}
