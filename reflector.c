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
#define MAXDATAGRAMSIZE 65000

int main(int argc, char* argv[]) {

	if (argc < 3) {
		printf("usage: %s [ port ] [ protocol ]\n",argv[0]);
		exit(-1);
	}

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

		struct sockaddr_in sad2; // structure to hold an IP address	
		memset((char *)&sad2,0,sizeof(sad2)); // clear sockaddr structure	
		sad2.sin_family = AF_INET;	      // set family to Internet	
		sad2.sin_addr.s_addr = htonl(INADDR_ANY);
		sad2.sin_port = htons((u_short)port+1);


		int fsize = sizeof(struct sockaddr);
		int	sd;		     // socket descriptor			
		sd = Socket(AF_INET, SOCK_DGRAM, 0);

		Bind(sd, (struct sockaddr *) &sad, sizeof(sad));
		

		printf("REFLECTOR: UDP Socket created\n");
		
		while(1) {
	
			int bytes_expected;
			printf("REFLECTOR: Awaiting UDP datagrams\n");	
			Recvfrom(sd, &bytes_expected, sizeof(bytes_expected),0,(struct sockaddr *)  &cad, (socklen_t *) &fsize);
			printsin(&cad, "REFLECTOR", ": ");
			printf("REFLECTOR: UDP received a packet, expecting another of %d bytes\n", bytes_expected);

			int *dump = malloc(bytes_expected);
			Recvfrom(sd, dump, sizeof(dump),0,(struct sockaddr *)&cad, (socklen_t *) &fsize);

			printf("REFLECTOR: UDP received a %d size packet\n", bytes_expected);

			printf("REFLECTOR: Responding with the same datagram\n");
			
			cad.sin_port = htons((u_short)port+1);
			printsin(&cad, "REFLECTOR",":");	
			
			Sendto(sd, &bytes_expected, sizeof(bytes_expected), 0, (struct sockaddr *) &cad, sizeof(cad) );
			Sendto(sd, dump, sizeof(dump), 0, (struct sockaddr *) &cad, sizeof(cad) );

			printf("REFLECTOR: Response sent\n");

		}

	} else if(strncmp(argv[2],"TCP", 3) == 0) {

		char	buf[MAXDATAGRAMSIZE];	     // buffer for data from the server

		struct sockaddr_in cad; // structure to hold an IP address	
		struct sockaddr_in sad; // structure to hold an IP address	
		memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
		sad.sin_family = AF_INET;	      // set family to Internet	
		sad.sin_addr.s_addr = htonl(INADDR_ANY);
		sad.sin_port = htons((u_short)port);

		u_int alen;
		int bytes_expected;
		int	sd, sd2;		     // socket descriptor			
		sd = Socket(AF_INET, SOCK_STREAM, 0);

		Bind(sd, (struct sockaddr *) &sad, sizeof(sad));

//		printf("REFLECTOR: TCP Socket created\n");

		listen(sd, 1);
		
		while(1) {

			memset((char *)&cad,0,sizeof(cad)); // clear sockaddr structure	

//			printf("REFLECTOR: Listening for a connection\n");
			alen = sizeof(cad);

			sd2 = accept(sd, (struct sockaddr *) &cad, &alen);		

//			printf("REFLECTOR: Accepted a connection\n");
			
//			printf("REFLECTOR: Waiting to receive a message\n");

			Readn(sd2, &bytes_expected, sizeof(int));
			Readn(sd2, buf, bytes_expected);

//			printf("REFLECTOR: Received a message\n");
//			printf("REFLECTOR: (%d) '%s'\n", bytes_expected, buf);

			printf("REFLECTOR: Replying with the same message\n");

			Writen(sd2, &bytes_expected, sizeof(int));
			Writen(sd2, buf, bytes_expected);

//			printf("REFLECTOR: Reply message sent\n");
			close(sd2);
		}

		close(sd);		

	} else {
		printf("Improper protocol %s expecting either 'TCP' or 'UDP'\n", argv[2]);
	}

	return 1;
}
