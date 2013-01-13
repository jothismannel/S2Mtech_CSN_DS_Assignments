#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);					/*displays error message*/
    exit(1);
}

int main(int argc, char *argv[])
{
     	int sockfd, newsockfd, portno, clilen;
     	char buffer[256];
    	struct sockaddr_in serv_addr, cli_addr;
     	int n;
     	if (argc < 2) 
	{
        	fprintf(stderr,"ERROR, no port provided\n");
        	exit(1);
     	}

     	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//creates new socket
     	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	
     	bzero((char *) &serv_addr, sizeof(serv_addr));		//initialize server address to zeros
     	portno = atoi(argv[1]);
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);			//converting port number to network byte order
     	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{ 
              error("ERROR on binding");
	}
	while(1)  
	{   
		listen(sockfd,5);				//causes the process to block until a client connects to the server 
     		clilen = sizeof(cli_addr);
     		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     		
		if (newsockfd < 0)
		{
 
          		error("ERROR on accept");
		}
     		bzero(buffer,256);				//initialize buffer to zero
     		n = read(newsockfd,buffer,255);			//reading from the socket
     		if (n < 0)
		{
			 error("ERROR reading from socket");
     		}	
		printf("Here is the message: %s\n",buffer);
     		n = write(newsockfd,"I got your message",18);	//writing to the socket
     		if (n < 0)
		{
			 error("ERROR writing to socket");
		}
	}
     return 0; 
}
