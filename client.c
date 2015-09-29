/*	client.c file
	
	TTP3121 TCP/IP  (Trimester 1, 2015/16) 
	Team Members: 1. Soon Wei Yang	  1122701186
		      2. Chong Rong Sheng 1121116031 */

#include "inet.h"
#define MSGSIZE 1024

main(int argc, char *argv[]){
int sockfd;
int rec= 0;
char message[MSGSIZE+1];
struct sockaddr_in server_address;
char makename[20];
char removename[20];
static struct sigaction act;

void catchin(int);

act.sa_handler = catchin;
sigfillset(&(act.sa_mask));

sigaction(SIGINT, &act, (void *) 0);


if(argc <= 1){
printf("The way to use: %s WARNING,Please Run For Example Given Beside [example: ./client 192.168.1.1]\n", argv[0]); exit(1); }

bzero( (char*) &server_address, sizeof(server_address) );
server_address.sin_family = AF_INET ;
server_address.sin_port = htons (SERV_TCP_PORT);
inet_pton (AF_INET, argv[1], &server_address.sin_addr);

if( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
perror("Client: socket() error \n");
exit(1); }

if(connect (sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) <0 ) {
perror("Client: connect() error\n");
exit(1);}


	char *buf; 
	buf=(char *)malloc(10*sizeof(char)); 
	buf=getlogin(); 

	
	char path[20]; 
	strcpy(path, "/home/"); 
	strcat(path, buf); 
	strcat(path, "/FILE_OF_CLIENT/"); 

	
	struct stat s; 
	if(stat(path, &s) == -1){ 
	mkdir(path, 0700); }


    do{ bzero( message, sizeof(message));
	recv(sockfd, message, MSGSIZE, 0);
	printf("\n%s\n", message); 
	gets(message);
	send(sockfd,message, MSGSIZE, 0);


if(!strcmp(message, "1"))
{
	bzero( message, sizeof(message));
	recv(sockfd, message, MSGSIZE, 0);
	printf("\n%s\n", message); 
	gets(message);
	send(sockfd,message, MSGSIZE, 0);
	
   	char filename[20];
    	strcpy(filename, "/home/"); 
    	strcat(filename, buf); 
    	strcat(filename, "/FILE_OF_CLIENT/");
    	strcat(filename, message);
    
    	FILE *fp;
    	fp = fopen(filename, "ab"); 

		if(NULL == fp)
    	{printf("Error opening file");}
    
    	bzero( message, sizeof(message));
    	rec = recv(sockfd, message, MSGSIZE, 0);
    	fwrite(message,1,rec,fp);
   
}


else if(!strcmp(message, "2")){

	DIR *dir;
	struct dirent *ent;

	char directoryName[20];
   	strcpy(directoryName, "/home/"); 
    	strcat(directoryName, buf); 
    	strcat(directoryName, "/FILE_OF_CLIENT/");

	if ((dir = opendir (directoryName)) != NULL) {
		
		printf("\n[List of files in Client Directory]\n");
  		// print all the files and directories within directory 
  		while ((ent = readdir (dir)) != NULL) {

		printf("%s\n", ent->d_name);    }

 		closedir (dir); }
	
	printf("\nPlease enter a file name for example like-> (aa.txt)\n");
	
	bzero( message, sizeof(message));
	gets(message);
	send(sockfd,message, MSGSIZE, 0);

	char filename[20];
	strcpy(filename, "/home/"); 
	strcat(filename, buf); 
	strcat(filename, "/FILE_OF_CLIENT/");
	    
	strcat(filename, message);

	FILE *fp;
    	fp = fopen(filename, "r"); 

	bzero( message, sizeof(message));
	int nread = fread(message,1,256,fp);
	send(sockfd, message, nread, 0);	
}

else if(!strcmp(message, "3")){
	printf("Type the directory name that you want to create: ");
	scanf("%s", makename);

	/* set the path/name of the directory that want to create */ 
	char createDirectory[20]; 
	strcpy(createDirectory, "/home/"); 
	strcat(createDirectory, buf); 
	strcat(createDirectory, "/"); 
	strcat(createDirectory, makename);

	/* Check the path exist or not, if not, create one */ 
	struct stat s; 
	if(stat(createDirectory, &s) == -1){ 
	mkdir(createDirectory, 0700); } 
}

else if(!strcmp(message, "4")){
	printf("Type the directory name that you want to delete: ");
	scanf("%s", removename);

	/* set the path of the directory that want to delete */ 
	char deleteDirectory[20]; 
	strcpy(deleteDirectory, "/home/"); 
	strcat(deleteDirectory, buf); 
	strcat(deleteDirectory, "/"); 
	strcat(deleteDirectory, removename);

	/* select all the files inside the directory that want to delete */
	char selectSubDirectory[50];
	strcpy(selectSubDirectory, "exec rm -r ");
	strcat(selectSubDirectory, "/home/"); 
	strcat(selectSubDirectory, buf); 
	strcat(selectSubDirectory, "/"); 
	strcat(selectSubDirectory, removename);
	strcat(selectSubDirectory, "/*"); 

	/* see the path exist or not, delete it if the path exist*/ 
	struct stat s; 
	if(stat(deleteDirectory, &s) != -1){
	system(selectSubDirectory);
	rmdir(deleteDirectory); } 
}

}while (strcmp(message, "/q"));
close(sockfd);
}

void catchin(int signo){
	printf("\n[ ^C INTERRUPT.................]\n");
}
