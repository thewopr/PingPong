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
#define NUMPACKETS 30
#define MAXDATAGRAMSIZE 65000

int main(int argc, char* argv[]) {

	if (argc < 3) {
		printf("usage: %s [ port ] [ protocol ]\n",argv[0]);
		exit(-1);
	}

	int i = 0;
	int	port = atoi(argv[1]);		     // protocol port number		

	if (port <= 0) {	
		fprintf(stderr,"SOURCE: bad port number %s\n",argv[2]);
		exit(1);
	}

	if(strncmp(argv[2],"UDP", 3) == 0 ) {
		struct sockaddr_in cad; // structure to hold an IP address	
		struct sockaddr_in sad; // structure to hold an IP address	
		memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
		sad.sin_family = AF_INET;	      // set family to Internet	
		sad.sin_addr.s_addr = htonl(INADDR_ANY);
		sad.sin_port = htons((u_short)port);

		int fsize = sizeof(struct sockaddr);
		int bytes_expected;
		int	sd;		     // socket descriptor			
		sd = Socket(AF_INET, SOCK_DGRAM, 0);

		Bind(sd, (struct sockaddr *) &sad, sizeof(sad));
		

		printf("REFLECTOR: UDP Socket created\n");
		
		while(1) {
			printf("Reflector awaiting UDP datagrams\n");	
			Recvfrom(sd, &bytes_expected, sizeof(bytes_expected),0, &cad, (socklen_t *) &fsize);
			printf("RELFLECTOR: UDP received a packet, expecting another of %d bytes\n", bytes_expected);
			int *dump = malloc(bytes_expected);
			Recvfrom(sd, dump, sizeof(dump),0,&cad, (socklen_t *) &fsize);
			printf("REFLECTOR: UDP received a %d size packet\n", bytes_expected);
		}

	} else if(strncmp(argv[2],"TCP", 3) == 0) {

		char	buf[BUFSIZE];	     // buffer for data from the server

		struct sockaddr_in cad; // structure to hold an IP address	
		struct sockaddr_in sad; // structure to hold an IP address	
		memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
		sad.sin_family = AF_INET;	      // set family to Internet	
		sad.sin_addr.s_addr = htonl(INADDR_ANY);
		sad.sin_port = htons((u_short)port);

		int alen;
		int bytes_expected;
		int	sd, sd2;		     // socket descriptor			
		sd = Socket(AF_INET, SOCK_STREAM, 0);

		Bind(sd, (struct sockaddr *) &sad, sizeof(sad));

		printf("REFLECTOR: TCP Socket created\n");

		listen(sd, 1);
		
		for(i = 0; i < NUMPACKETS; i++) {

			memset((char *)&cad,0,sizeof(cad)); // clear sockaddr structure	

			printf("REFLECTOR: Listening for a connection\n");
			alen = sizeof(cad);

			sd2 = accept(sd, (struct sockaddr *) &cad, &alen);		

			printf("REFLECTOR: Accepted a connection\n");
			printsin(&cad, "REFLECTOR", ": ");
			
			printf("REFLECTOR: (%d) Waiting to receive a message\n", i);

			Readn(sd2, &bytes_expected, sizeof(int));
			Readn(sd2, buf, bytes_expected+1);

			printf("REFLECTOR: (%d) Received a message\n", i);
			printf("REFLECTOR: (%d) (%d) '%s'\n", i, bytes_expected, buf);
		}

		close(sd2);
		close(sd);		

	} else {
		printf("Improper protocol %s expecting either 'TCP' or 'UDP'\n", argv[2]);
	}

	return 1;
}
