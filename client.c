#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg)
{
    perror(msg);			/*displays error message*/
    exit(0);
}

int main(int argc, char *argv[])
{
    	int sockfd, portno, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;

    	char buffer[256];
    	if (argc < 3) 
	{
       		fprintf(stderr,"usage %s hostname port\n", argv[0]);
       		exit(0);
    	}
    	
	portno = atoi(argv[2]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);			//creates new socket
    	if (sockfd < 0) 
	{        
		error("ERROR opening socket");
	}	
    
	server = gethostbyname(argv[1]);
    	if (server == NULL) 
	{
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}
    	
	bzero((char *) &serv_addr, sizeof(serv_addr));			//initialize server address to zeros
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);	//copy server address
    	serv_addr.sin_port = htons(portno);				//converting port number to network byte order		
    
	if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
	{        
		error("ERROR connecting");
	}
    	
	printf("Please enter the message: ");
    	bzero(buffer,256);						//initialize buffer to zero
    	fgets(buffer,255,stdin);
    	n = write(sockfd,buffer,strlen(buffer));			//writing to the file
    	
	if (n < 0)
	{ 
        	error("ERROR writing to socket");
	}    	
	bzero(buffer,256);
    	n = read(sockfd,buffer,255);					//reading from client
    	if (n < 0)
	{ 
         	error("ERROR reading from socket");
	}    	
	printf("%s\n",buffer);
    	return 0;
}
