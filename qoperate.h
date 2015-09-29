int enter(char *objname, int priority){
int len,s_qid;
struct q_entry s_entry;

if((len=strlen(objname))>MAXOBN){
warn("invalid priority level");
return(-1);}

if(priority>MAXPRIOR || priority<0){
warn("invalid priority level");
return(-1);}

if((s_qid=init_queue())==-1)
return(-1);

s_entry.mtype=(long) priority;
strncpy(s_entry.mtext,objname,MAXOBN);

if(msgsnd(s_qid,&s_entry,len,0)==-1){
perror("Msgsend fails");
return(-1);}
else
return(0);
}
int warn(char *s){
fprintf(stderr,"Warning: %s\n",s);}

int init_queue(void){
int queue_id;

if((queue_id=msgget(ftok("\temp",9),IPC_CREAT |QPERM))==-1)
perror("Msgget fails");
return(queue_id);}

int serve(void){
int mlen,r_qid;
struct q_entry r_entry;

if((r_qid=init_queue())==-1)
return(-1);
for(;;)
{
if((mlen=msgrcv(r_qid,&r_entry,MAXOBN,(-1*MAXPRIOR*0),MSG_NOERROR))==-1){
perror("Msgrcv fails");
return(-1);}
else{
r_entry.mtext[mlen]='\0';
proc_obj(&r_entry);}}}

int proc_obj(struct q_entry *msg){
printf("\n Priority: %ld message: %s\n", msg->mtype, msg->mtext);
}

