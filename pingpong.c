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
#include <math.h>
#include "wrappers.h"

extern int errno;

#define MAXDATAGRAMSIZE 65000
#define NUMTRIALS 100000

double elapsed(struct timeval end, struct timeval start) {

	double dUsec = end.tv_usec - start.tv_usec;
	double dsec = 0;

	if(dUsec < 0) {
		dsec += 1;
		dUsec += 1000;
	}
	dsec += end.tv_sec - start.tv_sec;

	return dsec*1000000 + dUsec;

}


int main(int argc, char* argv[]) {

	if (argc < 6) {
		fprintf(stderr, "usage: %s [ msgbytes ] [ host ] [ port ] [ protocol ] [ numTrials ]\n",argv[0]);
		exit(-1);
	}

	int msgbytes = atoi(argv[1]);
	char *host = argv[2];
	int port = atoi(argv[3]);		     // protocol port number		
	char *prot = argv[4];
	int numTrials = atoi(argv[5]);

	if (port <= 0) {	
		fprintf(stderr,"PINGPONG: bad port number %s\n",argv[3]);
		exit(1);
	}

	if(msgbytes < 1 || msgbytes > 65000) {
		fprintf(stderr,"PINGPONG: bad msgbytes size (%d), 0 < msgbytes < 65001\n", msgbytes);
		exit(1);
	}

	if(numTrials < 0 || numTrials > 1000000) {
		fprintf(stderr, "PINGPONG: bad numTrials (%d) , 0 < numTrials < 1001\n", numTrials);
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

	double times [numTrials];
	int i;

	if(strncmp(prot,"UDP", 3) == 0 ) {

		for(i = 0; i < numTrials; i++) {
			gettimeofday(&start, NULL);

			struct sockaddr_in cad; // structure to hold an IP address	

			struct sockaddr_in sad; // structure to hold an ip address	
			memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
			sad.sin_family = AF_INET;	      // set family to internet	
			bcopy(serverptr->ai_addr, (char *) &sad,  serverptr->ai_addrlen);
			sad.sin_port = htons((u_short)port);

			struct sockaddr_in sad2; // structure to hold an ip address	
			memset((char *)&sad2,0,sizeof(sad2)); // clear sockaddr structure	
			sad2.sin_family = AF_INET;	      // set family to internet	
			sad2.sin_addr.s_addr = htonl(INADDR_ANY);
			sad2.sin_port = htons((u_short)port+1);

//			printsin(&sad, "PRINT"," SIN:");
			
			fflush(stdout);
			
			int fsize = sizeof(struct sockaddr);
			int bytes_expected, sd;
			sd = Socket(AF_INET, SOCK_DGRAM, 0);

			Bind(sd, (struct sockaddr *) &sad2, sizeof(sad2));
			int *msg = malloc(msgbytes);
			int *dump = malloc(msgbytes);

//	fprintf(stderr,"PINGPONG: UDP Socket created\n");
//	fprintf(stderr,"PINGPONG: Sending a (%d) sized datagram\n", msgbytes);	

			Sendto(sd, &msgbytes, sizeof(msgbytes), 0,(struct sockaddr *) &sad, sizeof(sad));
			Sendto(sd, msg, sizeof(msg), 0, (struct sockaddr *) &sad, sizeof(sad));

//	fprintf(stderr,"PINGPONG: Datagram sent\n");
//	fprintf(stderr,"PINGPONG: Awaiting the response\n");

			Recvfrom(sd, &bytes_expected, sizeof(bytes_expected),0,(struct sockaddr *)  &cad, (socklen_t *) &fsize);
			Recvfrom(sd, dump, sizeof(dump),0,(struct sockaddr *) &cad, (socklen_t *) &fsize);

			gettimeofday(&end, NULL);			
			times[i] = elapsed(end,start);

//	fprintf(stderr,"PINGPONG: UDP received a %d size packet\n", bytes_expected);
			close(sd);
		}
//		fprintf(stderr,"PINGPONG: Execution done terminating\n");

	} else if(strncmp(prot,"TCP", 3) == 0) {
		char	buf[msgbytes];	     // buffer for data from the server
		for(i = 0; i < msgbytes; i++)
			buf[i] = 'A';
		buf[msgbytes-1] = 0;


		for(i = 0; i < numTrials; i++) {
			gettimeofday(&start, NULL);
			struct sockaddr_in sad; // structure to hold an IP address	
			memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
			sad.sin_family = AF_INET;	      // set family to Internet	
			bcopy(serverptr->ai_addr, (char *) &sad, serverptr->ai_addrlen);
			sad.sin_port = htons((u_short)port);

			int	sd;		   			
			sd = Socket(AF_INET, SOCK_STREAM, 0);

//			fprintf(stderr,"PINGPONG: Connecting to reflector TCP\n");

			Connect(sd, (struct sockaddr *) (&sad), sizeof(struct sockaddr));

//			fprintf(stderr,"PINGPONG: TCP connection established\n");

//			fprintf(stderr,"PINGPONG: Sending a message\n");
//			fprintf(stderr,"PINGPONG: Sending a message (%d) '%s'\n", msgbytes, buf);

			Writen(sd, &msgbytes, sizeof(int));
			Writen(sd, buf, msgbytes);

//			fprintf(stderr,"PINGPONG: Awating respone\n");

			Readn(sd, &msgbytes, sizeof(int));
			Readn(sd, buf, msgbytes);

//			fprintf(stderr,"PINGPONG: Recieved message (%d) '%s'\n", msgbytes, buf);
//			fprintf(stderr,"PINGPONG: Received response terminated\n");

			gettimeofday(&end, NULL);			
			times[i] = elapsed(end,start);


			close(sd);		
		}
	} else {
//		fprintf(stderr,"Improper protocol %s expecting either 'TCP' or 'UDP'\n", argv[2]);
	}


	double average = 0;

	for(i = 0; i < numTrials; i++)
		average += times[i];
	average /= numTrials;

	double stdDev = 0;

	for(i = 0; i < numTrials; i++)
		stdDev += (times[i] - average) * (times[i] - average);

	stdDev /= numTrials;
	stdDev = sqrt(stdDev);

	double error = 1.96 * (stdDev / sqrt(numTrials-1));

//	fprintf(stderr,"PINGPONG: Average of %d trials is %f\n", numTrials, average);
//	fprintf(stderr,"PINGPONG: Standard Deviation is %f\n", stdDev);
//	fprintf(stderr,"PINGPONG: Error is %f\n", error);

	printf("%d %f %f\n", msgbytes, average, error);

	return 1;
}
