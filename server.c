/*	server.c file
	
	TTP3121 TCP/IP  (Trimester 1, 2015/16) 
	Team Members: 1. Soon Wei Yang	  1122701186
		      2. Chong Rong Sheng 1121116031 */

#include "inet.h"
#define MSGSIZE 1024 /*buffer size length cannot more than 1024*/
#define MAXSIZE 2048

main(){
	int sockfd;// to create socket
	char message[MSGSIZE];
	int nsockfd;
	int clilen;
	char message1[MAXSIZE+1];
	int n;
	int rec = 0;
	struct sockaddr_in server_address, client_address;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	  { perror("Server: socket() error\n");
	    exit(1); };

	/* Get Server's IP address */
	FILE *file;
	char returnData[64];
	char *token;
	char a[1000];	
	file = popen("/sbin/ifconfig eth0", "r");
	while (fgets(returnData, 64, file) != NULL)
	{strcat(a, returnData);}

	
	token = strtok(a, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " ");
	token = strtok(NULL, " addr:");

	/* print the IP address token */
	printf("Server IP address: %s\n", token);

	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(token);
	server_address.sin_port = htons(SERV_TCP_PORT);

	pclose(file);

	if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
	perror("Server: bind() error\n");
	printf("\nConnection.......  .......... [bind]\n");

	char *buf; 
	buf=(char *)malloc(10*sizeof(char)); 
	buf=getlogin(); 
	
	char path[30]; 
	strcpy(path, "/home/");
	strcat(path, buf); 
	strcat(path, "/FILE_OF_SERVER"); 
	 
	struct stat s; 
	if(stat(path, &s) == -1){ 
	mkdir(path, 0700); }

	listen(sockfd, 5);
	int p[2];
	if(pipe(p) == -1)
	{
		perror("Fail to create pipe");
		exit(1);
	} 

	for(;;)

{
	 clilen = sizeof(client_address);
	 nsockfd = accept(sockfd, (struct sockaddr *)&client_address, &clilen);

	
	char *clientaddress = inet_ntoa(client_address.sin_addr);
	char *individual_address; 
	write(p[1], &clientaddress, sizeof(clientaddress));
	

	if(fork() == 0){
	  close(sockfd);
	  strcpy(message1,"** Hello, welcome to the server. ** \n\nPress\n1.Send File\t2.Download File\n3.Create Directory\t4.Delete Directory(include sub directory)\n[type /q to quit] : ");
	  send(nsockfd, message1, MAXSIZE, 0);


	if(nsockfd > 0)
		
	  read(p[0],&individual_address,sizeof(individual_address));
	  printf("\nClient %s connected now.\n", individual_address);
	  
	
	  
	do { n=recv(nsockfd, message, MSGSIZE, 0);
	 
	      if(!strcmp(message, "1"))
	{	
		bzero( message, sizeof(message));
		strcat(message,"[List of files in Server Directory]\nPlease choose a file from above the list to download:\n\n");
		
		DIR *dir;
		struct dirent *ent;

		char directoryName[30];
		strcpy(directoryName, "/home/");
    		strcat(directoryName, buf); 
    		strcat(directoryName, "/FILE_OF_SERVER/");
	
		if ((dir = opendir (directoryName)) != NULL) {
		
			
 			while ((ent = readdir (dir)) != NULL) {

			strcat(message, ent->d_name);
			strcat(message, "\n");
 			}
 		 closedir (dir);
		send(nsockfd, message, MAXSIZE, 0);
		 
		}
		
  		
 		else  { perror ("Directory not exist.");
  		return EXIT_FAILURE;
		}

	bool exist = true; 

	

        
do{ 	bzero( message, sizeof(message));
	recv(nsockfd, message, MAXSIZE, 0);

	char filename[30];
    	strcpy(filename, "/home/"); 
    	strcat(filename, buf); 
    	strcat(filename, "/FILE_OF_SERVER/");
    	strcat(filename, message);
    
	
	FILE *file = fopen(filename, "r");
	if(file==NULL)
        {
            strcpy(message," NO such file in server.Please key in the correct filename with extension.");
	    send(nsockfd, message, MAXSIZE, 0);  
	    exist = false; 
	   
        }  
	
	if(exist == true )
	{
	 bzero( message, sizeof(message));
	 int nread = fread(message,1,256,file);
	 send(nsockfd, message, nread, 0);
	}

	bzero( message, sizeof(message));
	strcpy(message,"Sucessfully Downloaded. [/q to quit]");

	}while(exist == false);}
	
	
	
	 if(!strcmp(message, "2"))
	
		
{	 bzero( message, sizeof(message));
	 recv(nsockfd, message, MAXSIZE, 0);

	char filename[30];
	strcpy(filename, "/home/"); 
	strcat(filename, buf); 
	strcat(filename, "/FILE_OF_SERVER/");
	strcat(filename, message);
		

	FILE *file;
   	 file = fopen(filename, "ab"); 
	bzero( message, sizeof(message));
	rec = recv(nsockfd, message, MAXSIZE, 0);
	fwrite(message,1,rec,file);
	fclose(file);

	bzero( message, sizeof(message));
	strcat(message,"Congratulation you have sent the file you want. [/q to quit]");
	}
	
	 
	if(!strcmp(message, "3"))
	{
		bzero( message, sizeof(message));
		strcat(message,"Congratulation you have maked the directory. [/q to quit]");
	}

	if(!strcmp(message, "4"))
	{
		bzero( message, sizeof(message));
		strcat(message,"Congratulation you have deleted directory. [/q to quit]");
	}

	if(n==0)
	{
	close(nsockfd);
	break;
	}
	message[n]=0;
	 send(nsockfd, message, MAXSIZE, 0);
	}while(strcmp(message1, "/q"));

	printf("\nClient %s disconnected now.\n", individual_address);
	exit(0);
	}
	close(nsockfd);
	}
	
	return 0;
}
