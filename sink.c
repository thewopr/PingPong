/*
 * CSCI 363 Computer Networks
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wrappers.h"

extern int errno;

#define BUFSIZE 1000
#define NAMELEN 257

int main(int argc, char* argv[]) {

	struct sockaddr_in sad; // structure to hold an IP address	
	struct sockaddr_in cad; // structure to hold an IP address	

	struct addrinfo hint;
	struct addrinfo *serverptr;

	int	sd, sd2;		     // socket descriptor			
	int	port;		     // protocol port number		
	char	*host;		     // pointer to host name		
	char	buf[BUFSIZE];	     // buffer for data from the server
	char buf2[BUFSIZE];
	int   bytes_expected;

	int alen;

	memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
	sad.sin_family = AF_INET;	      // set family to Internet	

	int i; 
	for(i = 65; i < BUFSIZE + 65; i++)
		buf2[i-65] = i % 256;

	if (argc < 3) {
		printf("usage: %s [ host ] [ port ]\n",argv[0]);
		exit(-1);
	}

	host = argv[1];		
	port = atoi(argv[2]);	

	if (port <= 0) {	
		fprintf(stderr,"SOURCE: bad port number %s\n",argv[2]);
		exit(1);
	}

	//  prepare the hint information
	bzero(&hint, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;

	Getaddrinfo(host, NULL, &hint, &serverptr);

	bcopy(serverptr->ai_addr, (char *)&sad, serverptr->ai_addrlen);

	sad.sin_port = htons((u_short)port);

	// Create a socket. 
	sd = Socket(AF_INET, SOCK_STREAM, 0);

	//Bind the socket to the specififed port
	Bind(sd, (struct sockaddr *) &sad, sizeof(sad));

	printf("SINK: Socket created\n");

	listen(sd, 5);
	alen = sizeof(cad);

	printf("SINK: Waiting for a connection\n");
	
	sd2 = accept(sd, (struct sockaddr *) &cad, &alen);		

	printf("SINK: Waiting to receive a message\n");

	Readn(sd2, &bytes_expected, sizeof(int));
	Readn(sd2, buf2, bytes_expected+1);

	printf("SINK: Received a message\n");
	printf("SINK: (%d) '%s'\n", bytes_expected, buf2);

	Readn(sd2, &bytes_expected, sizeof(int));
	Readn(sd2, buf2, bytes_expected+1);

	printf("SINK: Received a message\n");
	printf("SINK: (%d) '%s'\n", bytes_expected, buf2);

	printf("SINK: Responding to Source\n");

	strcpy(buf, "message recieved");
	bytes_expected = strlen(buf);
	
	Writen(sd, &bytes_expected, sizeof(int));
	Writen(sd, buf, bytes_expected);

	close(sd2);
	close(sd);		
	return 1;
}
