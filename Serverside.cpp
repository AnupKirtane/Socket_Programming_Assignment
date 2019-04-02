#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>

#define SIZE 1


int main(int argc, char *argv[])

{
	int sockfd, portno, newsockfd;                
	socklen_t clilen;                             //variable of type 'socklen_t' to store client address length
	struct sockaddr_in server_addr, client_addr;  //struct 'type sockaddr_in' to store Server 'server_addr' details & Client details 'client_addr'
	char buffer[4096];                            //buffer used for data from file stream
	char serv_msg[256];                           //buffer used for string inverting operation
	char temp_data;                                 
	 
	
	if (argc < 2)                                   
	{
		perror ("Port no not provided");           //Usage is "Executable file  Portno."
		exit(-1);
	}
	
	//socket() function call
	sockfd=socket(AF_INET, SOCK_STREAM, 0);        //initial server socket descriptor
	printf("Socket ID is %d \n",sockfd);
	
	if (sockfd < 0)
	{
		perror ("Socket error \n");
		exit(-1);
	}
	
	portno = atoi(argv[1]);  //casting portno from ASCII to INTEGER
	
	//STRUCT SOCKADDR_IN for Server & Client
	bzero ((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;           
	server_addr.sin_addr.s_addr= INADDR_ANY;                                          
	server_addr.sin_port=htons(portno);
	
	//bind() function call
	bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));    //BIND()   
	if (bind < 0)
	{
		perror ("Bind error");
		exit(-1);
	}
	//listen() function call
	listen(sockfd, 5);                                                     //LISTEN() with maximum 5 clients waiting
	if (listen < 0)
	{
		perror ("Listen error");
		exit(-1);
	}
	printf("Listening active on port no %d\n",portno);
	
	clilen=sizeof(client_addr);
  	//accept() function call
	newsockfd=accept(sockfd, (struct sockaddr *) &client_addr, &clilen);      //new(updated) client socket descriptor
	if (newsockfd < 0)
    
 	perror ("ERROR on accept\n");
 	else
 	{
	 printf("Connection established");
 	}
 	
 	//File operations using fread and frwrite
 	FILE *fp;                                        //creating a file pointer
 	fp = fdopen(newsockfd,"r");                      //opening filestream in read mode
 	
 	//fread()
 	if (fp == NULL)
 	{
 		printf("File access error for read operation");
 		exit(-1);
	}
 	
 	fread(buffer, sizeof(buffer), SIZE, fp);
	{
	 	printf ("Data received from client is: %s \n", buffer);
	}
 	
 	fp = fdopen(newsockfd,"w");                       //opening filestream in write mode
 	//fwrite()
 	if (fp == NULL)
	{
	 	printf("File access error for write operation");
	}
	
	 strcpy(serv_msg, buffer);                        //copying string from buffer[] to serv_msg[]
	 printf("Original data received from client: %s \n", serv_msg);
	 int i = 0;
	 int j;
	 j=strlen(serv_msg)-1;
	 
	 while(i<j)                                       //loop inverting string in serv_msg[]
	{
	 	temp_data = serv_msg[i];
	 	serv_msg[i]=serv_msg[j];
	 	serv_msg[j]=temp_data;
	 	i++;
	 	j--;
	}
	 
	 printf("Reversed data is: %s \n", serv_msg);       
	 
	 fwrite(serv_msg, sizeof(serv_msg), SIZE, fp);
	{
	 printf("Data sent is: %s \n", serv_msg);
	}
	 
	fclose(fp);                                       //closing filestream 
	close(newsockfd);                                 //closing new socket id (Client socket)
	close(sockfd);                                    //closing initial socket id (Server socket)
	printf("Connection closed\n");                    //terminating Server-Client connection
  
	return 0;
}
