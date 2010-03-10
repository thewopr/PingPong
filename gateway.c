#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "lab05.h"

int main(int argc, char **argv)
{
  if(argc <= 1) {
		fprintf(stderr,"usage : %s host-name prob_of_drop\n", argv[0]);
		fprintf(stderr,"example : %s localhost 0.5\n", argv[0]);
		exit(-1);
  }
	
	int socket_in, socket_out, fsize;
	struct sockaddr_in  s_in, s_out;
	struct sockaddr_in s2_in, s2_out, *from;
	struct hostent *sinkptr, *sourceptr;

	double prob = atof(argv[3]);

	message msg_in, msg_out;
	
	sourceptr = gethostbyname(argv[2]);
	
	double prob_loss;

	/* set up socket to receive from source */

	socket_in = socket (AF_INET, SOCK_DGRAM, 0);
	socket_out = socket (AF_INET, SOCK_DGRAM, 0);
	
	bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

	s_in.sin_family = (short)AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
	s_in.sin_port = htons((u_short)SOURCE_TO_GATEWAY);

	bzero((char *) &s2_in, sizeof(s2_in));  /* They say you must do this    */
	s2_in.sin_family = (short)AF_INET;
	s2_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
	s2_in.sin_port = htons((u_short)SINK_TO_GATEWAY);

	Bind(socket_in, (struct sockaddr *)&s_in, sizeof(s_in));
	Bind(socket_out, (struct sockaddr *)&s2_in, sizeof(s2_in));

	printsin( &s_in, "GATEWAY", "Local in socket is:"); 
	fsize = sizeof(struct sockaddr);

	fd_set watch;

	int max_fd = socket_in;
	if(socket_in < socket_out)
		max_fd = socket_out;
	/* set up socket to send to sink */

	sinkptr = gethostbyname(argv[2]);
	sourceptr = gethostbyname(argv[1]);

	for(;;) {
	
		FD_ZERO(&watch);
		FD_SET(socket_in, &watch);
		FD_SET(socket_out, &watch);

		Select(max_fd+1,&watch,NULL,NULL,NULL);

		printf("\n");

		if(FD_ISSET(socket_in, &watch)) {
			printf("GATEWAY: Got something from source\n");
			
			/* wait for datagram from source */
			Recvfrom(socket_in,&msg_in,sizeof(msg_in),0,
					(struct sockaddr *)&from, (socklen_t*) &fsize);

			msg_out = msg_in;

			if (from != NULL) {
				printsin((struct sockaddr_in*)&from, "GATEWAY: ", "Packet from:");

				printf("GATEWAY: Got data ::%c%ld%c\n",
						msg_in.head,
						(long) ntohl(msg_in.body),
						msg_in.tail); 

				fflush(stdout);

				/* throw p-biased coin (p=0.5) to decide what to do with datagram */
				prob_loss = ((float) rand()) / ((float) RAND_MAX);
				if (prob_loss > prob) {
					/* send datagram to sink */
					bzero((char *) &s_out, sizeof(s_out)); 
					
					s_out.sin_family = (short)AF_INET;
					bcopy(sinkptr->h_addr, (char *)&s_out.sin_addr,sinkptr->h_length);			
					s_out.sin_port = htons((u_short)GATEWAY_TO_SINK);

					Sendto(socket_out,&msg_out,sizeof(msg_out),0,
							(struct sockaddr *)&s_out, sizeof(s_out));

					printf("GATEWAY: SENT data ::%c%ld%c\n", 
							msg_out.head,
							(long) ntohl(msg_out.body), 
							msg_out.tail); 
					fflush(stdout);
				}
				else {
					/* send datagram to oblivion */
					printf("GATEWAY: datagram discarded.\n"); fflush(stdout);
				}
			}
		}
	/* wait for datagram from source */

		if(FD_ISSET(socket_out,&watch)) {	
			printf("GATEWAY: Got something from sink\n");
			
			Recvfrom(socket_out,&msg_out,sizeof(msg_out),0,
					(struct sockaddr *)&from, (socklen_t*) &fsize);

			if (from != NULL) {
				printsin((struct sockaddr_in*)&from, "GATEWAY: ", "Packet from:");

				printf("GATEWAY: Got data ::%c%ld%c\n",
						msg_out.head,
						(long) ntohl(msg_out.body),
						msg_out.tail); 

				fflush(stdout);

				/* throw p-biased coin (p=0.5) to decide what to do with datagram */
				prob_loss = ((float) rand()) / ((float) RAND_MAX);
				if (prob_loss > prob) {
					/* send datagram to sink */
					bzero((char *) &s2_out, sizeof(s2_out)); 
					s2_out.sin_family = (short)AF_INET;
					bcopy(sourceptr->h_addr, (char *)&s2_out.sin_addr,sourceptr->h_length);			
					s2_out.sin_port = htons((u_short)GATEWAY_TO_SOURCE);

					Sendto(socket_in,&msg_out,sizeof(msg_out),0,
							(struct sockaddr *)&s2_out, sizeof(s2_out));

					printf("GATEWAY: datagram forwarded to source.\n"); fflush(stdout);
					printf("GATEWAY: SENT data ::%c%ld%c\n", 
							msg_out.head,
							(long) ntohl(msg_out.body), 
							msg_out.tail); 
					fflush(stdout);
				}
				else {
					/* send datagram to oblivion */
					printf("GATEWAY: datagram discarded.\n"); fflush(stdout);
				}
			}	
		}
	}

	return 0;
}
