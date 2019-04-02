#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>

#define SIZE 1


int main(int argc, char *argv[])
{
	int sockid, portno;
	struct sockaddr_in server_addr;                    //struct 'type sockaddr_in' to store Server 'server_addr'
	struct hostent *ser;                               //struct 'type hostent' used for gethostbyname()
	char buffer[4096];                                 //buffer used for data from file stream
	char reply[256];                                   //buffer used for string inverting operation
	
	if (argc < 3)
	{
		printf("Hostname or Port no not provided");    //Usage is "Executable file  Server_Hostname  Portno."
		exit(-1);
	}
	//socket() function call
	sockid = socket(AF_INET, SOCK_STREAM, 0);          //client socket descriptor
	if (sockid < 0)
	{
		printf("Socket error");
	}
	printf("Socket ID is %d \n",sockid);
	
	//Recognize IP of Host by gethostbyname()
	ser = gethostbyname(argv[1]);                       //tracing IP address of the Host_Server by its name 
	if (ser==NULL)
	{
		printf("No such host available \n");
		exit(-1);
	}
	
	portno = atoi(argv[2]);                             //casting portno from ASCII to INTEGER
	
	//STRUCT SOCKADDR_IN for Client
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)ser->h_addr, (char *)&server_addr.sin_addr.s_addr, ser->h_length);
    server_addr.sin_port = htons(portno);
    
    printf("Client connected to Server on port no %d \n",portno);
    
    //connect() function call
    int connect_id = connect(sockid, (struct sockaddr *) &server_addr,sizeof(server_addr));
    printf("The connection ID is %d \n",connect_id);
    if (connect_id < 0)
	{
    	printf("Error connecting \n");
    	exit(-1);
	}
	
	//File operations using fread and fwrite
	FILE *fp;                                         //creating a file pointer
	fp=fdopen(sockid,"w");                            //opening filestream in write mode
	
	//fwrite()
	if (fp == NULL)
	{
		printf("File access error for write operation");
		exit(-1);
	}
	
	printf("Enter the data: ");
	scanf("%[^\t\n]s", buffer);                       //scanning string input from client and storing it into buffer[]
	printf("Data been written is: %s \n",buffer);                 
	fwrite(buffer, 1, sizeof(buffer), fp);            
	printf("Data Successfully Sent to Client\n");
	
	//fread()
	fp=fdopen(sockid,"r");                            //opening filestream in read mode
	fread(reply, 1, sizeof(buffer), fp);
	printf("Data received from server is: %s \n",reply);
	
	fclose(fp);	                                      //closing filestream
	
	close(sockid);                                    //closing the client socket id

	return 0;	
}
