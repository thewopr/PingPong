/*
 * CSCI 363 Computer Networks
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wrappers.h"

extern int errno;

#define MAXDATAGRAMSIZE 65000

double elapsed(struct timeval end, struct timeval start) {

	double dUsec = end.tv_usec - start.tv_usec;
	double dsec = 0;

	if(dUsec < 0) {
		dsec += 1;
		dUsec += 1000;
	}
	dsec += end.tv_sec - start.tv_sec;

	return dsec + (dUsec / 1000000);

}


int main(int argc, char* argv[]) {

	if (argc < 5) {
		printf("usage: %s [ msgbytes ] [ host ] [ port ] [ protocol ]\n",argv[0]);
		exit(-1);
	}

	int msgbytes = atoi(argv[1]);
	char *host = argv[2];
	int port = atoi(argv[3]);		     // protocol port number		
	char *prot = argv[4];

	if (port <= 0) {	
		fprintf(stderr,"PINGPONG: bad port number %s\n",argv[3]);
		exit(1);
	}

	if(msgbytes < 1 || msgbytes > 65000) {
		fprintf(stderr,"PINGPONG: bad msgbytes size (%d), 0 < msgbytes < 65001 \n", msgbytes);
		exit(1);
	}
	
	struct addrinfo *serverptr;
	struct addrinfo hint;
	
	//  prepare the hint information
	bzero(&hint, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;

	Getaddrinfo(host, NULL, &hint, &serverptr);

	struct timeval start, end;
	gettimeofday(&start, NULL);
	

	if(strncmp(prot,"UDP", 3) == 0 ) {


		struct sockaddr_in cad; // structure to hold an IP address	
		
		struct sockaddr_in sad; // structure to hold an ip address	
		memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
		sad.sin_family = AF_INET;	      // set family to internet	
		bcopy(serverptr->ai_addr, (char *) &sad,  serverptr->ai_addrlen);
		sad.sin_port = htons((u_short)port);

		struct sockaddr_in sad2; // structure to hold an ip address	
		memset((char *)&sad2,0,sizeof(sad2)); // clear sockaddr structure	
		sad2.sin_family = AF_INET;	      // set family to internet	
		bcopy(serverptr->ai_addr, (char *) &sad2,  serverptr->ai_addrlen);
		sad2.sin_port = htons((u_short)port+1);

		int fsize = sizeof(struct sockaddr);
		int bytes_expected, sd;
		sd = Socket(AF_INET, SOCK_DGRAM, 0);

		Bind(sd, (struct sockaddr *) &sad2, sizeof(sad2));
		int *msg = malloc(msgbytes);
		int *dump = malloc(msgbytes);

		printf("PINGPONG: UDP Socket created\n");
		printf("PINGPONG: Sending a (%d) sized datagram\n", msgbytes);	

		Sendto(sd, &msgbytes, sizeof(msgbytes), 0,(struct sockaddr *) &sad, sizeof(sad));
		Sendto(sd, msg, sizeof(msg), 0, (struct sockaddr *) &sad, sizeof(sad));

		printf("PINGPONG: Datagram sent\n");
		printf("PINGPONG: Awaiting the response\n");

		Recvfrom(sd, &bytes_expected, sizeof(bytes_expected),0,(struct sockaddr *)  &cad, (socklen_t *) &fsize);
		Recvfrom(sd, dump, sizeof(dump),0,(struct sockaddr *) &cad, (socklen_t *) &fsize);

		printf("PINGPONG: UDP received a %d size packet\n", bytes_expected);
		close(sd);
		printf("PINGPONG: Execution done terminating\n");

	} else if(strncmp(prot,"TCP", 3) == 0) {

		char	buf[msgbytes];	     // buffer for data from the server
		int i;
		for(i = 0; i < msgbytes; i++)
			buf[i] = 'A';
		buf[msgbytes-1] = 0;

		struct sockaddr_in sad; // structure to hold an IP address	
		memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
		sad.sin_family = AF_INET;	      // set family to Internet	
		bcopy(serverptr->ai_addr, (char *) &sad, serverptr->ai_addrlen);
		sad.sin_port = htons((u_short)port);

		printsin(&sad, "PINGPONG", ":");

		int	sd;		   			
		sd = Socket(AF_INET, SOCK_STREAM, 0);

		printf("PINGPONG: Connecting to reflector TCP\n");

		Connect(sd, (struct sockaddr *) (&sad), sizeof(struct sockaddr));

		printf("PINGPONG: TCP connection established\n");

		printf("PINGPONG: Sending a message\n");
		printf("PINGPONG: Sending a message (%d) '%s'\n", msgbytes, buf);

		Writen(sd, &msgbytes, sizeof(int));
		Writen(sd, buf, msgbytes);

		printf("PINGPONG: Awating respone\n");

		Readn(sd, &msgbytes, sizeof(int));
		Readn(sd, buf, msgbytes);
	
		printf("PINGPONG: Recieved message (%d) '%s'\n", msgbytes, buf);
		printf("PINGPONG: Received response terminated\n");
		close(sd);		

	} else {
		printf("Improper protocol %s expecting either 'TCP' or 'UDP'\n", argv[2]);
	}

	gettimeofday(&end, NULL);
	printf("PINGPONG: Elapsed time %f second\n", elapsed(end, start));


	return 1;
}
